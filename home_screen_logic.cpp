#include "home_screen_logic.h"

#include "audio_manager.h"
#include "app_state.h"
#include "azan_screen_logic.h"
#include "home_screen_ui.h"
#include "ui.h"

#include <ArduinoJson.h>
#include <WiFi.h>
#include <lvgl.h>
#include <stdio.h>
#include <time.h>

static bool initialized = false;
static long nextDiff = 0;
static int lastTriggeredPrayer = -1;
static int lastTriggerDayOfYear = -1;
static unsigned long lastDayCheck = 0;
static int lastSec = -1;
static long lastDiff = -1;
static int lastIndex = -1;

void home_screen_init() {
  if (initialized) return;

  home_screen_ui_init(go_to_prayers_screen, go_to_settings_screen);

  if (timeValid) {
    drawClock();
    drawNextPrayer();
    drawDate();
  }

  updateStatusIcons();
  initialized = true;
}

void updateStatusIcons() {
  home_screen_ui_set_status(WiFi.status() == WL_CONNECTED, sdCardOk);
}

void go_to_prayers_screen(lv_event_t* e) {
  LV_UNUSED(e);
  changeScreen(SCREEN_PRAYERS);
}

void go_to_settings_screen(lv_event_t* e) {
  LV_UNUSED(e);
  changeScreen(SCREEN_SETTINGS);
}

void home_screen_loop() {
  static bool wasActive = false;

  if (ui_Screen_HomeScreen == NULL) return;

  handleHomeToneTick();

  if (!initialized) {
    home_screen_init();
  }

  if (lv_scr_act() != ui_Screen_HomeScreen) {
    wasActive = false;
    return;
  }

  if (!wasActive) {
    home_screen_ui_reset_actions();
    wasActive = true;
  }

  home_screen_ui_tick();
  if (!timeValid) return;

  static unsigned long last1s = 0;
  if (millis() - last1s >= 1000) {
    last1s += 1000;
    drawClock();
    drawNextPrayer();
  }

  if (lastDayCheck == 0 || millis() - lastDayCheck > 60000) {
    lastDayCheck = millis();
    drawDate();
  }
}

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
      nextPrayerIndex = i;
      startAdhan();
    }
  }
}

void drawClock() {
  if (globalTime.tm_sec == lastSec) return;
  lastSec = globalTime.tm_sec;

  char buf[10];
  snprintf(buf, sizeof(buf), "%02d:%02d:%02d", globalTime.tm_hour, globalTime.tm_min, globalTime.tm_sec);
  home_screen_ui_set_clock_text(buf);
}

void drawDate() {
  static int lastDay = -1;
  if (globalTime.tm_mday == lastDay) return;
  lastDay = globalTime.tm_mday;

  char buf[32];
  snprintf(buf, sizeof(buf), "%02d.%02d.%04d", globalTime.tm_mday, globalTime.tm_mon + 1, globalTime.tm_year + 1900);
  home_screen_ui_set_date_text(buf);
}

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
  snprintf(text, sizeof(text), "%s\nin %s", prayers[nextPrayerIndex].name.c_str(), diffStr);
  home_screen_ui_set_next_prayer_text(text);
}

void handleHomeToneTick() {
  // Audio wird global im Audio-Manager getaktet.
}

void playHomeTone() {
  audio_manager_play_system_start("/system_start.mp3");
}
