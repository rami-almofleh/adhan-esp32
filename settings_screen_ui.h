#pragma once

#include <Arduino.h>
#include <lvgl.h>

void settings_screen_ui_init(
    lv_event_cb_t reset_cb,
    lv_event_cb_t home_cb,
    lv_event_cb_t volume_minus_cb,
    lv_event_cb_t volume_plus_cb,
    lv_event_cb_t theme_cb,
    lv_event_cb_t adhan_change_cb);
void settings_screen_ui_set_volume(uint8_t volume);
void settings_screen_ui_set_theme(bool dark_mode);
void settings_screen_ui_set_theme_label(const char* text);
void settings_screen_ui_set_adhan_text(const char* text);
bool settings_screen_ui_is_syncing();
