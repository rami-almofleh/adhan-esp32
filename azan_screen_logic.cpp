#include "azan_screen_logic.h"

#include "audio_manager.h"
#include "app_state.h"
#include "azan_screen_ui.h"
#include "settings_screen_logic.h"
#include "ui.h"

static bool initialized = false;

namespace {

Screen get_return_screen() {
  return screenBeforeAzan == SCREEN_AZAN ? SCREEN_HOME : screenBeforeAzan;
}

void stop_click_handler(lv_event_t* e) {
  LV_UNUSED(e);
  stopAdhan();
}

}  // namespace

void azan_screen_init() {
  if (initialized) return;
  azan_screen_ui_init(stop_click_handler);
  initialized = true;
}

void azan_screen_loop() {
  if (!initialized) {
    azan_screen_init();
  }
}

void startAdhan() {
  if (audio_manager_is_source_active(AUDIO_SOURCE_ADHAN)) {
    if (currentScreen != SCREEN_AZAN) {
      changeScreen(SCREEN_AZAN);
    }
    return;
  }

  azan_screen_ui_set_prayer_title(prayers[nextPrayerIndex].name.c_str());
  azan_screen_ui_set_hint("Tippen zum Stoppen");
  changeScreen(SCREEN_AZAN);

  const String effectiveFile = settings_screen_get_effective_adhan_file();
  if (!audio_manager_play_adhan_file(effectiveFile.c_str())) {
    changeScreen(get_return_screen());
  }
}

void stopAdhan() {
  audio_manager_stop_source(AUDIO_SOURCE_ADHAN);
}

void handleAudioTick() {
  // Audio wird global im Audio-Manager getaktet.
}
