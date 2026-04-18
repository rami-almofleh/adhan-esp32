#include "prayers_screen_logic.h"

#include "app_state.h"
#include "home_screen_logic.h"
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
  static long lastPrayerSeconds[5] = {-1, -1, -1, -1, -1};
  static bool forceRefresh = true;

  if (!initialized) {
    prayers_screen_init();
  }

  if (lv_scr_act() != ui_Screen_PrayersScreen) {
    forceRefresh = true;
    return;
  }

  calculateNextPrayer();

  bool prayerDataChanged = forceRefresh;
  for (int i = 0; i < 5; i++) {
    if (lastPrayerSeconds[i] != prayers[i].seconds) {
      prayerDataChanged = true;
      lastPrayerSeconds[i] = prayers[i].seconds;
    }
  }

  if (prayerDataChanged || lastShownPrayerIndex != nextPrayerIndex) {
    prayers_screen_ui_refresh(prayers, nextPrayerIndex);
    lastShownPrayerIndex = nextPrayerIndex;
    forceRefresh = false;
  }
}
