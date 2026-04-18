#include "home_screen_logic.h"
#include "app_state.h"
#include "ui.h"
#include <ArduinoJson.h>
#include <stdio.h>
#include <time.h>
#include <lvgl.h>
#include <WiFi.h>

// -------------------------
// States & Flags
// -------------------------
static bool initialized = false;
static long nextDiff = 0;
static int lastTriggeredPrayer = -1;
static int lastTriggerDayOfYear = -1;
static unsigned long lastDayCheck = 0;
static int lastSec = -1;
static long lastDiff = -1;
static int lastIndex = -1;

// -------------------------
// Initialization
// -------------------------
void home_screen_init() {
  if (initialized) return;

  // Buttons klickbar machen
  lv_obj_add_flag(ui_HomeScreen_Button_PrayersButton, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_flag(ui_HomeScreen_Button_SettingsButton, LV_OBJ_FLAG_CLICKABLE);

  // Event-Callbacks
  lv_obj_add_event_cb(ui_HomeScreen_Button_PrayersButton, go_to_prayers_screen, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(ui_HomeScreen_Button_SettingsButton, go_to_settings_screen, LV_EVENT_CLICKED, NULL);

  // Initiale Daten anzeigen
  if (timeValid) {
    drawClock();
    drawNextPrayer();
    drawDate();
  }

  playHomeTone();

  // Status-Icons direkt einmal aktualisieren
  updateStatusIcons();

  initialized = true;
}


// -------------------------
// Initialize WLAN & SD Icons
// -------------------------
void updateStatusIcons() {
  if (ui_HomeScreen_Label_WLANStateLabel == NULL || ui_HomeScreen_Label_SDStateLabel == NULL) return;
  setIconColor(ui_HomeScreen_Label_WLANStateLabel, WiFi.status() == WL_CONNECTED);
  setIconColor(ui_HomeScreen_Label_SDStateLabel, sdCardOk);
}

void setIconColor(lv_obj_t* label, bool active) {
  if (label == NULL) return;

  lv_obj_set_style_text_color(
      label,
      active ? lv_color_hex(0x2DA041) : lv_color_hex(0xBBBBBB),
      LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_invalidate(label);
}

// -------------------------
// Callbacks
// -------------------------
void go_to_prayers_screen(lv_event_t* e) {
  Serial.printf("clicked go_to_prayers_screen\n");
  changeScreen(SCREEN_PRAYERS);
}

void go_to_settings_screen(lv_event_t* e) {
  Serial.printf("clicked go_to_settings_screen\n");
  changeScreen(SCREEN_SETTINGS);
}

// -------------------------
// Home Screen Loop
// -------------------------
void home_screen_loop() {
  if (ui_Screen_HomeScreen == NULL) return;

  handleHomeToneTick();

  if (!initialized) {
    home_screen_init();
  }

  // Check if the screen is currently active
  if (lv_scr_act() != ui_Screen_HomeScreen) return;
  if (!timeValid) return;

  static unsigned long last1s = 0;
  if (millis() - last1s >= 1000) {
    last1s += 1000;

    drawClock();
    drawNextPrayer();
  }

  // Datum einmal pro Minute
  if (lastDayCheck == 0 || millis() - lastDayCheck > 60000) {
    lastDayCheck = millis();
    drawDate();
  }
}

// -------------------------
// Adhan Logic
// -------------------------
void checkAdhanLogic() {
  static unsigned long lastCheck = 0;
  if (millis() - lastCheck < 1000) return;
  lastCheck = millis();

  if (!timeValid) return;

  if (lastTriggerDayOfYear != globalTime.tm_yday) {
    lastTriggerDayOfYear = globalTime.tm_yday;
    lastTriggeredPrayer = -1;
  }

  long now = globalTime.tm_hour * 3600 + globalTime.tm_min * 60 + globalTime.tm_sec;

  for (int i = 0; i < 5; i++) {
    long diff = now - prayers[i].seconds;

    if (diff >= 0 && diff < 10 && lastTriggeredPrayer != i) {
      lastTriggeredPrayer = i;
      Serial.printf("ADHAN ZEIT! Es ist %s (%s)\n", prayers[i].name.c_str(), prayers[i].time.c_str());

      if (!isPlaying) {
        nextPrayerIndex = i;
        changeScreen(SCREEN_AZAN);
      }
    }
  }
}

// -------------------------
// Clock
// -------------------------
void drawClock() {
  if (ui_HomeScreen_Label_ClockLabel == NULL) return;

  if (globalTime.tm_sec == lastSec) return;
  lastSec = globalTime.tm_sec;

  char buf[10];
  snprintf(buf, sizeof(buf), "%02d:%02d:%02d",
           globalTime.tm_hour,
           globalTime.tm_min,
           globalTime.tm_sec);

  lv_label_set_text(ui_HomeScreen_Label_ClockLabel, buf);
  lv_obj_invalidate(ui_HomeScreen_Label_ClockLabel);
  // lv_refr_now(NULL);
}

// -------------------------
// Datum
// -------------------------
void drawDate() {
  if (ui_HomeScreen_Label_DateLabel == NULL) return;
  static int lastDay = -1;

  if (globalTime.tm_mday == lastDay) return;

  lastDay = globalTime.tm_mday;

  char buf[32];
  snprintf(buf, sizeof(buf), "%02d.%02d.%04d",
           globalTime.tm_mday,
           globalTime.tm_mon + 1,
           globalTime.tm_year + 1900);

  lv_label_set_text(ui_HomeScreen_Label_DateLabel, buf);
}

// -------------------------
// Next Prayer
// -------------------------
void calculateNextPrayer() {
  long now = globalTime.tm_hour * 3600 + globalTime.tm_min * 60 + globalTime.tm_sec;
  nextDiff = 999999;

  for (int i = 0; i < 5; i++) {
    long diff = prayers[i].seconds - now;
    if (diff >= 0 && diff < nextDiff) {
      nextDiff = diff;
      nextPrayerIndex = i;
    }
  }

  if (nextDiff == 999999) {
    nextPrayerIndex = 0;
    nextDiff = (24 * 3600 - now) + prayers[0].seconds;
  }
}

void drawNextPrayer() {
  calculateNextPrayer();

  if (nextDiff == lastDiff && nextPrayerIndex == lastIndex) return;
  lastDiff = nextDiff;
  lastIndex = nextPrayerIndex;

  char diffStr[16];
  if (nextDiff < 3600) {
    int minutes = nextDiff / 60;
    int seconds = nextDiff % 60;
    snprintf(diffStr, sizeof(diffStr), "%02d:%02d", minutes, seconds);
  } else {
    int hours = nextDiff / 3600;
    int minutes = (nextDiff % 3600) / 60;
    snprintf(diffStr, sizeof(diffStr), "%02d:%02d", hours, minutes);
  }

  char text[64];
  snprintf(text, sizeof(text), "%s\nin %s",
           prayers[nextPrayerIndex].name.c_str(),
           diffStr);

  if (ui_HomeScreen_Label_DiffLabel == NULL) return;
  lv_label_set_text(ui_HomeScreen_Label_DiffLabel, text);
}

void handleHomeToneTick() {
  if (systemStartAudioMp3 && systemStartAudioMp3->isRunning()) {
    if (!systemStartAudioMp3->loop()) {
      systemStartAudioMp3->stop();
      delete systemStartAudioMp3;
      systemStartAudioMp3 = nullptr;

      delete systemStartAudioFile;
      systemStartAudioFile = nullptr;
    }
  }
}

void playHomeTone() {
  // Wenn schon ein Ton läuft, stoppen und freigeben
  if (systemStartAudioMp3) {
    systemStartAudioMp3->stop();
    delete systemStartAudioMp3;
    systemStartAudioMp3 = nullptr;
  }
  if (systemStartAudioFile) {
    delete systemStartAudioFile;
    systemStartAudioFile = nullptr;
  }

  // Neues MP3-File laden
  systemStartAudioFile = new AudioFileSourceSD("/system_start.mp3");
  systemStartAudioMp3 = new AudioGeneratorMP3();

  if (!systemStartAudioMp3->begin(systemStartAudioFile, out)) {
    Serial.println("MP3 Tone start failed!");
    delete systemStartAudioMp3;
    systemStartAudioMp3 = nullptr;
    delete systemStartAudioFile;
    systemStartAudioFile = nullptr;
    return;
  }

  Serial.println("Tone gestartet!");
}
