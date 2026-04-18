#include "app_state.h"
#include "ui.h"

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
AppSettings appSettings = {6, false, 0};
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
  if (newScreen == SCREEN_AZAN && currentScreen != SCREEN_AZAN) {
    screenBeforeAzan = currentScreen;
  }

  switch (newScreen) {
    case SCREEN_HOME:
      lv_scr_load_anim(ui_Screen_HomeScreen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, false);
      break;

    case SCREEN_PRAYERS:
      lv_scr_load_anim(ui_Screen_PrayersScreen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, false);
      break;

    case SCREEN_SETTINGS:
      lv_scr_load_anim(ui_Screen_SettingsScreen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, false);
      break;

    case SCREEN_AZAN:
      lv_scr_load_anim(ui_Screen_AzanScreen, LV_SCR_LOAD_ANIM_FADE_ON, 300, 0, false);
      break;
  }

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
