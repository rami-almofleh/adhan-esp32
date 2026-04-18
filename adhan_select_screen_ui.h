#pragma once

#include <Arduino.h>
#include <lvgl.h>

void adhan_select_screen_ui_init(lv_event_cb_t select_cb, lv_event_cb_t play_cb, lv_event_cb_t back_cb);
void adhan_select_screen_ui_refresh(size_t count, const String* labels, int selected_index, int preview_index);
