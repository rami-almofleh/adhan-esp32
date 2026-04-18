#pragma once

#include <lvgl.h>

void azan_screen_ui_init(lv_event_cb_t stop_cb);
void azan_screen_ui_set_prayer_title(const char* title);
void azan_screen_ui_set_hint(const char* text);
