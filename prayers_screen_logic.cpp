#include "prayers_screen_logic.h"

#include "app_state.h"
#include "prayers_screen_ui.h"
#include "ui.h"

static bool initialized = false;

namespace {

void go_to_home_screen(lv_event_t* e) {
  LV_UNUSED(e);
  changeScreen(SCREEN_HOME);
}

}  // namespace

void prayers_screen_init() {
  if (initialized) return;

  prayers_screen_ui_init(go_to_home_screen);
  prayers_screen_ui_refresh(prayers, nextPrayerIndex);
  initialized = true;
}

void prayers_screen_loop() {
  static int lastShownPrayerIndex = -1;

  if (!initialized) {
    prayers_screen_init();
  }

  if (lv_scr_act() != ui_Screen_PrayersScreen) {
    return;
  }

  if (lastShownPrayerIndex != nextPrayerIndex) {
    prayers_screen_ui_refresh(prayers, nextPrayerIndex);
    lastShownPrayerIndex = nextPrayerIndex;
  }
}
