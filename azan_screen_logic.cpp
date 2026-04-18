#include "azan_screen_logic.h"
#include "app_state.h"
#include "ui.h"
#include <SD.h>
#include <AudioFileSourceSD.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>

static bool initialized = false;
static bool adhanStarted = false;

// merken welches Gebet gerade läuft
static String currentPrayerName = "";

namespace {

AudioFileSourceSD* openAdhanFile() {
  static const char* adhanCandidates[][4] = {
      {"/adhan1.mp3", "/adhan_1.mp3", "/adhan.mp3", "adhan.mp3"},
      {"/adhan2.mp3", "/adhan_2.mp3", "/adhan.mp3", "adhan.mp3"},
      {"/adhan3.mp3", "/adhan_3.mp3", "/adhan.mp3", "adhan.mp3"}};

  uint8_t soundIndex = appSettings.adhanSoundIndex;
  if (soundIndex > 2) {
    soundIndex = 0;
  }

  for (uint8_t i = 0; i < 4; i++) {
    AudioFileSourceSD* candidate = new AudioFileSourceSD(adhanCandidates[soundIndex][i]);
    if (candidate && candidate->isOpen()) {
      Serial.printf("Adhan Datei: %s\n", adhanCandidates[soundIndex][i]);
      return candidate;
    }
    delete candidate;
  }

  return nullptr;
}

Screen getReturnScreen() {
  return screenBeforeAzan == SCREEN_AZAN ? SCREEN_HOME : screenBeforeAzan;
}

}  // namespace

// -------------------------
// INIT
// -------------------------
void azan_screen_init() {
  if (initialized) return;

  if (ui_Screen_AzanScreen == NULL) return;

  // kompletter Screen klickbar
  lv_obj_add_flag(ui_Screen_AzanScreen, LV_OBJ_FLAG_CLICKABLE);

  lv_obj_add_event_cb(ui_Screen_AzanScreen, [](lv_event_t* e) {
    stopAdhan();
  }, LV_EVENT_CLICKED, NULL);

  initialized = true;
}

// -------------------------
// LOOP
// -------------------------
void azan_screen_loop() {
  if (!initialized) {
    azan_screen_init();
  }

  if (!adhanStarted) {
    startAdhan();
    adhanStarted = isPlaying;
    if (!adhanStarted) {
      changeScreen(getReturnScreen());
      return;
    }
  }

  handleAudioTick();
}

// -------------------------
// START ADHAN
// -------------------------
void startAdhan() {
  Serial.println("Starte Adhan...");

  currentPrayerName = prayers[nextPrayerIndex].name;

  // ✅ RICHTIGE LABELS
  if (ui_AzanScreen_Label_PrayerTitle != NULL) {
    lv_label_set_text(ui_AzanScreen_Label_PrayerTitle, currentPrayerName.c_str());
  }

  if (ui_AzanScreen_Label_Label8 != NULL) {
    lv_label_set_text(ui_AzanScreen_Label_Label8, "Tippen zum Stoppen");
  }

  file = openAdhanFile();

  if (!file) {
    Serial.println("Keine Datei gefunden!");
    return;
  }

  mp3 = new AudioGeneratorMP3();

  if (mp3->begin(file, out)) {
    isPlaying = true;
    Serial.println("Adhan läuft!");
  } else {
    Serial.println("MP3 Start fehlgeschlagen");
    delete mp3;
    mp3 = nullptr;
    delete file;
    file = nullptr;
  }
}

// -------------------------
// STOP ADHAN
// -------------------------
void stopAdhan() {
  Serial.println("Stoppe Adhan");

  if (mp3) {
    mp3->stop();
    delete mp3;
    mp3 = nullptr;
  }

  if (file) {
    delete file;
    file = nullptr;
  }

  isPlaying = false;
  adhanStarted = false;

  changeScreen(getReturnScreen());
}

// -------------------------
// AUDIO LOOP
// -------------------------
void handleAudioTick() {
  if (isPlaying && mp3 && mp3->isRunning()) {

    if (!mp3->loop()) {
      stopAdhan(); // 👉 automatisch zurück
    }
  }
}
