use cstr_core::{cstr, CStr};
use lvgl::{lv_drv_disp_sdl, lv_drv_input_pointer_sdl, LvResult, NativeObject};
use lvgl::widgets::*;
use lvgl::{Align, DrawBuffer, Widget};
use lvgl::input_device::InputDriver;
use lvgl_sys::{lv_obj_del, lv_textarea_get_text};
use matrix_sdk::config::SyncSettings;
use matrix_sdk::reqwest::Url;
use matrix_sdk::Client;
use std::thread::sleep;
use std::time::Duration;
use std::time::Instant;

#[tokio::main]
async fn main() -> LvResult<()> {
    const HOR_RES: u32 = 540;
    const VER_RES: u32 = 960;

    let buffer = DrawBuffer::<{ (HOR_RES * VER_RES) as usize }>::default();
    let display = lv_drv_disp_sdl!(buffer, HOR_RES, VER_RES)?;
    let input = lv_drv_input_pointer_sdl!(display)?;

    let mut username_ta = Textarea::new()?;
    username_ta.set_align(Align::Center, 0, -65)?;
    username_ta.set_width(300)?;
    username_ta.set_height(40)?;
    username_ta.set_placeholder_text(cstr!("Username"))?;

    let mut password_ta = Textarea::new()?;
    password_ta.set_align(Align::Center, 0, -20)?;
    password_ta.set_width(300)?;
    password_ta.set_height(40)?;
    password_ta.set_placeholder_text(cstr!("Password"))?;
    password_ta.set_password_mode(true)?;

    let mut sign_in_btn = Btn::new()?;
    sign_in_btn.set_align(Align::Center, 0, 25)?;
    sign_in_btn.set_width(300)?;
    sign_in_btn.set_height(40)?;

    let mut sign_in_btn_label = Label::create(&mut sign_in_btn)?;
    sign_in_btn_label.set_align(Align::Center, 0, 0)?;
    sign_in_btn_label.set_text(cstr!("Sign In"))?;
    
    // !!!
    username_ta.set_text(cstr!(...));
    password_ta.set_text(cstr!(...));
    // !!!

    let client = std::sync::Arc::new(Client::builder()
        .homeserver_url(Url::parse("https://matrix.org").unwrap())
        .build()
        .await
        .unwrap());
    
    sign_in_btn.on_event(|btn, event| {
        if let lvgl::Event::Clicked = event {
            let username = unsafe {
                CStr::from_ptr(lv_textarea_get_text(username_ta.raw().unwrap().as_ptr())).to_string_lossy().into_owned()
            };
            let password = unsafe {
                CStr::from_ptr(lv_textarea_get_text(password_ta.raw().unwrap().as_ptr())).to_string_lossy().into_owned()
            };

            unsafe {
                lv_obj_del(username_ta.raw().unwrap().as_ptr());
                lv_obj_del(password_ta.raw().unwrap().as_ptr());
                lv_obj_del(btn.raw().unwrap().as_ptr());
            }

            let mut label = Label::from("Signing in...");
            label.set_align(Align::Center, 0, 0);

            let spawn_client = std::sync::Arc::clone(&client);
            tokio::spawn(async move {
                _ = spawn_client.matrix_auth().login_username(username, &password).send().await.unwrap();
                label.set_text(cstr!("Performing initial sync..."));
                _ = spawn_client.sync_once(SyncSettings::default()).await.unwrap();
                
                let rooms: Vec<String> = spawn_client.rooms().iter().filter(|room| room.name().is_some()).map(|room| room.name().unwrap()).collect();
                
                unsafe {
                    lv_obj_del(label.raw().unwrap().as_ptr());
                }

                let mut heading = Label::from("# Rooms");
                heading.set_align(Align::TopLeft, 0, 0);
                heading.set_height(60);

                let mut y = 60;
                for room in rooms.iter() {
                    let mut label = Label::from(room);
                    label.set_align(Align::TopLeft, 0, y);
                    label.set_height(40);
                    y += 40;
                }
            });
        }
    })?;

    loop {
        let start = Instant::now();
        lvgl::task_handler();
        sleep(Duration::from_millis(5));
        lvgl::tick_inc(Instant::now().duration_since(start));
    }
}