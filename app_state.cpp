#include "app_state.h"
#include "settings_screen_logic.h"
#include "ui.h"

#include <TFT_eSPI.h>

extern TFT_eSPI tft;

namespace {

uint16_t screen_background_color() {
  return appSettings.darkMode ? tft.color565(16, 20, 18) : tft.color565(247, 251, 248);
}

void clear_display_for_screen_change() {
  initHardware(H_TFT);
  tft.fillScreen(screen_background_color());
}

}  // namespace

Screen currentScreen = SCREEN_START;
Screen screenBeforeAzan = SCREEN_HOME;
SystemStatus currentStatus = ST_START;

Prayer prayers[] = {
    {"Fajr", "5", 0},
    {"Dhuhr", "12", 30},
    {"Asr", "15", 45},
    {"Maghrib", "18", 15},
    {"Isha", "19", 45}
};
struct tm globalTime;
bool timeValid = false;
bool isPlaying = false;
bool sdCardOk = false;
AppSettings appSettings = {6, false, ""};
AudioGeneratorMP3* mp3 = nullptr;
AudioFileSourceSD* file = nullptr;
AudioOutputI2S* out = nullptr;
AudioFileSourceSD* toneFile = nullptr;
AudioGeneratorMP3* mp3Tone = nullptr;
AudioFileSourceSD* systemStartAudioFile = nullptr;
AudioGeneratorMP3* systemStartAudioMp3 = nullptr;
int nextPrayerIndex = 0;

const uint8_t TOUCH_CS = 33;
const uint8_t SD_CS = 5;
const uint8_t TFT_H_CS = 15;

void changeScreen(Screen newScreen) {
  if (currentScreen == newScreen) {
    return;
  }

  if ((currentScreen == SCREEN_SETTINGS && newScreen != SCREEN_SETTINGS) ||
      (currentScreen == SCREEN_ADHAN_SELECT && newScreen != SCREEN_ADHAN_SELECT)) {
    settings_screen_stop_preview();
  }

  if (newScreen == SCREEN_AZAN && currentScreen != SCREEN_AZAN) {
    screenBeforeAzan = currentScreen;
  }

  lv_obj_t* target = nullptr;
  switch (newScreen) {
    case SCREEN_HOME:
      target = ui_Screen_HomeScreen;
      break;

    case SCREEN_PRAYERS:
      target = ui_Screen_PrayersScreen;
      break;

    case SCREEN_SETTINGS:
      target = ui_Screen_SettingsScreen;
      break;

    case SCREEN_AZAN:
      target = ui_Screen_AzanScreen;
      break;

    case SCREEN_ADHAN_SELECT:
      target = ui_Screen_AdhanSelectScreen;
      break;

    default:
      break;
  }

  if (target == nullptr) {
    return;
  }

  clear_display_for_screen_change();
  lv_scr_load(target);
  lv_obj_invalidate(target);
  lv_refr_now(NULL);
  currentScreen = newScreen;
}

void initHardware(B_Hardware hardewareTitle) {
  switch (hardewareTitle) {
    case H_TOUCH:
      digitalWrite(TFT_H_CS, HIGH);
      digitalWrite(SD_CS, HIGH);
      digitalWrite(TOUCH_CS, LOW);
      break;

    case H_TFT:
      digitalWrite(TOUCH_CS, HIGH);
      digitalWrite(SD_CS, HIGH);
      digitalWrite(TFT_H_CS, LOW);
      break;

    case H_SD:
      digitalWrite(TOUCH_CS, HIGH);
      digitalWrite(TFT_H_CS, HIGH);
      digitalWrite(SD_CS, LOW);
      break;
  }
}
