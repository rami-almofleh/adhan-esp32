#pragma once

#include "app_state.h"

void prayers_screen_ui_init(lv_event_cb_t home_cb);
void prayers_screen_ui_refresh(const Prayer* prayers, int highlighted_index);
