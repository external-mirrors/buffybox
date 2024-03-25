use anyhow::Context;
use cstr_core::{cstr, CStr, CString};
use lvgl::input_device::InputDriver;
use lvgl::style::Style;
use lvgl::widgets::*;
use lvgl::{lv_drv_disp_sdl, lv_drv_input_pointer_sdl, LvResult, NativeObject};
use lvgl::{Align, DrawBuffer, Widget};
use lvgl_sys::{
    lv_disp_get_default, lv_disp_get_hor_res, lv_disp_get_scr_act, lv_disp_get_ver_res,
    lv_disp_load_scr, lv_keyboard_set_textarea, lv_obj_create, lv_obj_del, lv_textarea_get_text,
};
use matrix_sdk::config::SyncSettings;
use matrix_sdk::matrix_auth::MatrixSession;
use matrix_sdk::reqwest::Url;
use matrix_sdk::Client;
use rand::distributions::Alphanumeric;
use rand::{thread_rng, Rng};
use serde::{Deserialize, Serialize};
use std::cmp::max;
use std::fs;
use std::path::{Path, PathBuf};
use std::thread::sleep;
use std::time::Duration;
use std::time::Instant;

fn blank_screen() {
    unsafe {
        let old_screen = lv_disp_get_scr_act(lv_disp_get_default());
        let new_screen = lv_obj_create(core::ptr::null_mut());
        lv_disp_load_scr(new_screen);
        lv_obj_del(old_screen);
    }
}

fn load_sign_in_screen(
    homeserver: Option<String>,
    username: Option<String>,
    password: Option<String>,
) -> LvResult<()> {
    blank_screen();

    let mut homeserver_ta = Textarea::new()?;
    homeserver_ta.set_align(Align::Center, 0, -110);
    homeserver_ta.set_width(300);
    homeserver_ta.set_height(40);
    homeserver_ta.set_placeholder_text(cstr!("Homeserver"))?;
    homeserver_ta.set_text(
        CString::new(homeserver.unwrap_or("".into()))
            .unwrap_or(cstr!("").into())
            .as_ref(),
    )?;

    let mut username_ta = Textarea::new()?;
    username_ta.set_align(Align::Center, 0, -65);
    username_ta.set_width(300);
    username_ta.set_height(40);
    username_ta.set_placeholder_text(cstr!("Username"))?;
    username_ta.set_text(
        CString::new(username.unwrap_or("".into()))
            .unwrap_or(cstr!("").into())
            .as_ref(),
    )?;

    let mut password_ta = Textarea::new()?;
    password_ta.set_align(Align::Center, 0, -20);
    password_ta.set_width(300);
    password_ta.set_height(40);
    password_ta.set_placeholder_text(cstr!("Password"))?;
    password_ta.set_password_mode(true)?;
    password_ta.set_text(
        CString::new(password.unwrap_or("".into()))
            .unwrap_or(cstr!("").into())
            .as_ref(),
    )?;

    let mut sign_in_btn = Btn::new()?;
    sign_in_btn.set_align(Align::Center, 0, 25);
    sign_in_btn.set_width(300);
    sign_in_btn.set_height(40);

    let mut sign_in_btn_label = Label::create(&mut sign_in_btn)?;
    sign_in_btn_label.set_align(Align::Center, 0, 0);
    sign_in_btn_label.set_text(cstr!("Sign In"))?;

    let hor_res = max(0, unsafe { lv_disp_get_hor_res(lv_disp_get_default()) });
    let ver_res = max(0, unsafe { lv_disp_get_ver_res(lv_disp_get_default()) });

    let mut keyboard = Keyboard::new()?;
    keyboard.set_width(hor_res as u32);
    keyboard.set_height((0.33 * ver_res as f32) as u32);
    keyboard.set_align(Align::BottomMid, 0, 0);

    let keyboard_arc = std::sync::Arc::new(keyboard);

    let keyboard_arc_for_homeserver = std::sync::Arc::clone(&keyboard_arc);
    homeserver_ta.on_event(move |textarea, event| {
        if let lvgl::Event::Clicked = event {
            unsafe {
                lv_keyboard_set_textarea(
                    keyboard_arc_for_homeserver.raw().as_ptr(),
                    textarea.raw().as_ptr(),
                );
            }
        }
    })?;

    let keyboard_arc_for_username = std::sync::Arc::clone(&keyboard_arc);
    username_ta.on_event(move |textarea, event| {
        if let lvgl::Event::Clicked = event {
            unsafe {
                lv_keyboard_set_textarea(
                    keyboard_arc_for_username.raw().as_ptr(),
                    textarea.raw().as_ptr(),
                );
            }
        }
    })?;

    let keyboard_arc_for_password = std::sync::Arc::clone(&keyboard_arc);
    password_ta.on_event(move |textarea, event| {
        if let lvgl::Event::Clicked = event {
            unsafe {
                lv_keyboard_set_textarea(
                    keyboard_arc_for_password.raw().as_ptr(),
                    textarea.raw().as_ptr(),
                );
            }
        }
    })?;

    sign_in_btn.on_event(move |_btn, event| {
        if let lvgl::Event::Clicked = event {
            let homeserver = unsafe {
                CStr::from_ptr(lv_textarea_get_text(homeserver_ta.raw().as_ptr()))
                    .to_string_lossy()
                    .into_owned()
            };
            let username = unsafe {
                CStr::from_ptr(lv_textarea_get_text(username_ta.raw().as_ptr()))
                    .to_string_lossy()
                    .into_owned()
            };
            let password = unsafe {
                CStr::from_ptr(lv_textarea_get_text(password_ta.raw().as_ptr()))
                    .to_string_lossy()
                    .into_owned()
            };
            let _ = load_sign_in_loading_screen(homeserver, username, password);
        }
    })?;

    Ok(())
}

fn load_sign_in_loading_screen(
    homeserver: String,
    username: String,
    password: String,
) -> LvResult<()> {
    blank_screen();

    let mut label = Label::new()?;
    label.set_align(Align::Center, 0, 0);
    label.set_text(cstr!("Mmm Mmm Mmm Mmm..."))?;

    tokio::spawn(async move {
        async fn try_login(
            homeserver: &String,
            username: &String,
            password: &String,
        ) -> anyhow::Result<Client> {
            let url = Url::parse(&homeserver)?;

            let db_passphrase = thread_rng()
                .sample_iter(Alphanumeric)
                .take(32)
                .map(char::from)
                .collect::<String>();

            let client = build_client(&url, &Paths::db_dir(), &db_passphrase, None).await?;

            client
                .matrix_auth()
                .login_username(&username, &password)
                .send()
                .await?;

            store_session(
                &Paths::session_file(),
                &client,
                &Paths::db_dir(),
                &db_passphrase,
            )
            .await?;

            client.sync_once(SyncSettings::default()).await?;

            Ok(client)
        }

        match try_login(&homeserver, &username, &password).await {
            Ok(client) => load_room_list_screen(client),
            Err(_) => load_sign_in_screen(homeserver.into(), username.into(), password.into()),
        }
    });

    Ok(())
}

fn load_room_list_screen(client: Client) -> LvResult<()> {
    blank_screen();

    let rooms: Vec<String> = client
        .rooms()
        .iter()
        .filter(|room| room.name().is_some())
        .map(|room| room.name().unwrap())
        .collect();

    let mut heading = Label::new()?;
    heading.set_align(Align::TopLeft, 0, 0);
    heading.set_height(60);
    heading.set_text(cstr!("# Rooms"))?;

    let mut y = 60;
    for room in rooms.iter() {
        let mut label = Label::new()?;
        label.set_align(Align::TopLeft, 0, y);
        label.set_height(40);
        label.set_text(
            CString::new(room.to_string())
                .unwrap_or(cstr!("").into())
                .as_ref(),
        )?;
        y += 40;
    }

    Ok(())
}

struct Paths {}

impl Paths {
    fn data_dir() -> PathBuf {
        dirs::data_dir()
            .expect("Should be able to find the operating system's data directory")
            .join("dummy")
    }

    fn session_file() -> PathBuf {
        Self::data_dir().join("session")
    }

    fn db_dir() -> PathBuf {
        Self::data_dir().join("db")
    }
}

#[derive(Serialize, Deserialize)]
struct SessionPickle {
    homeserver: String,

    db_path: PathBuf,
    db_passphrase: String,

    session: MatrixSession,

    #[serde(skip_serializing_if = "Option::is_none")]
    sync_token: Option<String>,
}

async fn store_session(
    file: &Path,
    client: &Client,
    db_path: &PathBuf,
    db_passphrase: &String,
) -> anyhow::Result<()> {
    let session = client
        .matrix_auth()
        .session()
        .context("Client should have a session when logged in")?;
    let pickle = SessionPickle {
        homeserver: client.homeserver().to_string(),
        db_path: db_path.to_path_buf(),
        db_passphrase: db_passphrase.to_string(),
        session: session,
        sync_token: None,
    };
    fs::write(file, serde_json::to_string(&pickle)?)?;
    Ok(())
}

async fn restore_session(file: &Path) -> anyhow::Result<Client> {
    let pickle = serde_json::from_str::<SessionPickle>(fs::read_to_string(file)?.as_ref())?;
    let client = Client::builder()
        .homeserver_url(pickle.homeserver)
        .sqlite_store(pickle.db_path, Some(&pickle.db_passphrase))
        .build()
        .await?;
    client.restore_session(pickle.session).await?;
    Ok(client)
}

async fn build_client(
    homeserver: &Url,
    db_path: &PathBuf,
    db_passphrase: &String,
    session: Option<MatrixSession>,
) -> anyhow::Result<Client> {
    let client = Client::builder()
        .homeserver_url(homeserver)
        .sqlite_store(db_path, Some(db_passphrase))
        .build()
        .await?;
    if let Some(session) = session {
        client.restore_session(session).await?;
    }
    Ok(client)
}

#[tokio::main]
async fn main() -> LvResult<()> {
    lvgl::init();

    const HOR_RES: u32 = 540;
    const VER_RES: u32 = 960;

    let buffer = DrawBuffer::<{ (HOR_RES * VER_RES) as usize }>::default();
    let display = lv_drv_disp_sdl!(buffer, HOR_RES, VER_RES)?;
    let _input = lv_drv_input_pointer_sdl!(display)?;

    // TODO: Without this the app crashes with a "bus error" on launch
    lvgl::task_handler();

    tokio::spawn(async {
        if Paths::session_file().exists() {
            if let Ok(client) = restore_session(&Paths::session_file()).await {
                _ = load_room_list_screen(client);
                return;
            }
            fs::remove_dir(Paths::data_dir())
                .expect("Should be able to remove corrupt data directory");
        }

        _ = load_sign_in_screen(None, None, None);
    });

    loop {
        let start = Instant::now();
        lvgl::task_handler();
        sleep(Duration::from_millis(5));
        lvgl::tick_inc(Instant::now().duration_since(start));
    }
}
