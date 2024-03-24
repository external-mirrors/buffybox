use anyhow::Context;
use cstr_core::{cstr, CStr, CString};
use lvgl::input_device::InputDriver;
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
    label.set_text(cstr!("Signing in..."))?;

    tokio::spawn(async move {
        let url = Url::parse(&homeserver);
        if url.is_err() {
            let _ = load_sign_in_screen(homeserver.into(), username.into(), password.into());
            return;
        }
        let url = url.unwrap();

        let data_dir = dirs::data_dir();
        if data_dir.is_none() {
            println!("Could not determine data dir");
            let _ = load_sign_in_screen(homeserver.into(), username.into(), password.into());
            return;
        }
        let session_file = data_dir.as_ref().unwrap().join("dummy.session");
        let db_path = data_dir.as_ref().unwrap().join("dummy.db");

        let db_passphrase = thread_rng()
            .sample_iter(Alphanumeric)
            .take(32)
            .map(char::from)
            .collect::<String>();

        let client = Client::builder()
            .homeserver_url(url)
            .sqlite_store(&db_path, Some(&db_passphrase))
            .build()
            .await;
        if client.is_err() {
            println!("Could not create client {:?}", client.err());
            let _ = load_sign_in_screen(homeserver.into(), username.into(), password.into());
            return;
        }
        let client = client.unwrap();

        if store_session(&session_file, &client, &db_path, &db_passphrase)
            .await
            .is_err()
        {
            println!("Could not store session");
            let _ = load_sign_in_screen(homeserver.into(), username.into(), password.into());
            return;
        }

        if client
            .matrix_auth()
            .login_username(&username, &password)
            .send()
            .await
            .is_err()
        {
            let _ = load_sign_in_screen(homeserver.into(), username.into(), password.into());
            return;
        }

        if client.sync_once(SyncSettings::default()).await.is_err() {
            println!("Could not perform initial sync");
            let _ = load_sign_in_screen(homeserver.into(), username.into(), password.into());
            return;
        }

        let _ = load_room_list_screen(client);
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

#[derive(Serialize, Deserialize)]
struct SessionPickle {
    homeserver: String,

    db_path: PathBuf,
    db_passphrase: String,

    session: MatrixSession,

    #[serde(skip_serializing_if = "Option::is_none")]
    sync_token: Option<String>,
}

impl SessionPickle {
    fn write_to_file(self: Self, file: &Path) -> anyhow::Result<()> {
        let json = serde_json::to_string(&self)?;
        fs::write(file, json)?;
        Ok(())
    }

    fn read_from_file(file: &Path) -> anyhow::Result<Self> {
        let json = fs::read_to_string(file)?;
        Ok(serde_json::from_str::<Self>(&json)?)
    }
}

async fn store_session(
    file: &Path,
    client: &Client,
    db_path: &PathBuf,
    db_passphrase: &String,
) -> anyhow::Result<()> {
    let pickle = SessionPickle {
        homeserver: client.homeserver().to_string(),
        db_path: db_path.to_path_buf(),
        db_passphrase: db_passphrase.to_string(),
        session: client
            .matrix_auth()
            .session()
            .context("Client should have a session when logged in")?,
        sync_token: None,
    };
    pickle.write_to_file(file)?;
    Ok(())
}

async fn restore_session(file: &Path) -> anyhow::Result<Client> {
    let session_pickle = SessionPickle::read_from_file(&file)?;
    let client = Client::builder()
        .homeserver_url(session_pickle.homeserver)
        .sqlite_store(session_pickle.db_path, Some(&session_pickle.db_passphrase))
        .build()
        .await?;
    client.restore_session(session_pickle.session).await?;
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
        let data_dir =
            dirs::data_dir().expect("Should be able to find the operating system's data directory");
        println!("Using data dir {:?}", data_dir);
        let session_file = data_dir.join("dummy.session");

        if session_file.exists() {
            if let Ok(client) = restore_session(&session_file).await {
                _ = load_room_list_screen(client);
                return;
            }
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
