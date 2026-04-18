#pragma once

#include <lvgl.h>

void home_screen_ui_init(lv_event_cb_t prayers_cb, lv_event_cb_t settings_cb);
void home_screen_ui_tick();
void home_screen_ui_reset_actions();
void home_screen_ui_set_status(bool wifi_connected, bool sd_ready);
void home_screen_ui_set_clock_text(const char* text);
void home_screen_ui_set_date_text(const char* text);
void home_screen_ui_set_next_prayer_text(const char* text);
