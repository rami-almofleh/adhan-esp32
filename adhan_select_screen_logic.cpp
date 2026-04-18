#include "adhan_select_screen_logic.h"

#include "adhan_select_screen_ui.h"
#include "app_state.h"
#include "settings_screen_logic.h"
#include "ui.h"

#include <stdint.h>

namespace {

bool initialized = false;
int lastSelectedIndex = -2;
int lastPreviewIndex = -2;
size_t lastCount = static_cast<size_t>(-1);

size_t visible_count() {
  size_t count = settings_screen_get_adhan_option_count();
  return count > 10 ? 10 : count;
}

int selected_index() {
  for (size_t i = 0; i < settings_screen_get_adhan_option_count(); i++) {
    if (settings_screen_is_selected_adhan_option(i)) return static_cast<int>(i);
  }
  return -1;
}

int preview_index() {
  for (size_t i = 0; i < settings_screen_get_adhan_option_count(); i++) {
    if (settings_screen_is_previewing_option(i)) return static_cast<int>(i);
  }
  return -1;
}

void refresh_screen() {
  size_t count = visible_count();
  String labels[10];
  for (size_t i = 0; i < count; i++) {
    labels[i] = settings_screen_get_adhan_option_label(i);
  }
  adhan_select_screen_ui_refresh(count, labels, selected_index(), preview_index());
  lastCount = count;
  lastSelectedIndex = selected_index();
  lastPreviewIndex = preview_index();
}

void select_handler(lv_event_t* e) {
  uintptr_t index = reinterpret_cast<uintptr_t>(lv_event_get_user_data(e));
  settings_screen_select_adhan_option(static_cast<size_t>(index));
  refresh_screen();
}

void play_handler(lv_event_t* e) {
  uintptr_t index = reinterpret_cast<uintptr_t>(lv_event_get_user_data(e));
  size_t option_index = static_cast<size_t>(index);
  if (settings_screen_is_previewing_option(option_index)) {
    settings_screen_stop_preview();
  } else {
    settings_screen_play_preview_for_option(option_index);
  }
  refresh_screen();
}

void back_handler(lv_event_t* e) {
  LV_UNUSED(e);
  settings_screen_stop_preview();
  changeScreen(SCREEN_SETTINGS);
}

}  // namespace

void adhan_select_screen_init() {
  if (initialized) return;
  adhan_select_screen_ui_init(select_handler, play_handler, back_handler);
  refresh_screen();
  initialized = true;
}

void adhan_select_screen_loop() {
  if (!initialized) {
    adhan_select_screen_init();
  }

  handleToneTick();

  if (lv_scr_act() != ui_Screen_AdhanSelectScreen) {
    lastCount = static_cast<size_t>(-1);
    lastSelectedIndex = -2;
    lastPreviewIndex = -2;
    return;
  }

  int selected = selected_index();
  int preview = preview_index();
  size_t count = visible_count();
  if (count != lastCount || selected != lastSelectedIndex || preview != lastPreviewIndex) {
    refresh_screen();
  }
}
