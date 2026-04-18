#pragma once

#include <Arduino.h>
#include <lvgl.h>

void reset_app(lv_event_t* e);

void settings_screen_init();
void settings_screen_loop();
void load_settings();
void apply_settings();
void volume_increase(lv_event_t* e);
void volume_decrease(lv_event_t* e);
void theme_switch_event_handler(lv_event_t* e);
void update_volume_display();
void update_audio_gain();
void playTone();
void handleToneTick();
void settings_screen_stop_preview();
void settings_screen_reload_adhan_files();
String settings_screen_get_effective_adhan_file();

size_t settings_screen_get_adhan_option_count();
String settings_screen_get_adhan_option_label(size_t index);
bool settings_screen_is_selected_adhan_option(size_t index);
bool settings_screen_is_previewing_option(size_t index);
bool settings_screen_select_adhan_option(size_t index);
bool settings_screen_play_preview_for_option(size_t index);
void settings_screen_open_adhan_selector(lv_event_t* e);
