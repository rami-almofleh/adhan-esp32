#include "azan_screen_logic.h"

#include "app_state.h"
#include "azan_screen_ui.h"
#include "settings_screen_logic.h"
#include "ui.h"

#include <AudioFileSourceSD.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>
#include <SD.h>

static bool initialized = false;
static bool adhanStarted = false;

namespace {

AudioFileSourceSD* try_open(const char* path) {
  AudioFileSourceSD* candidate = new AudioFileSourceSD(path);
  if (candidate && candidate->isOpen()) {
    Serial.printf("Adhan: verwende %s\n", path);
    return candidate;
  }
  delete candidate;
  return nullptr;
}

AudioFileSourceSD* open_adhan_file() {
  String effective_file = settings_screen_get_effective_adhan_file();
  if (effective_file.length() > 0) {
    AudioFileSourceSD* selected = try_open(effective_file.c_str());
    if (selected) {
      return selected;
    }

    String selected_path = effective_file;
    int slash_pos = selected_path.lastIndexOf('/');
    String file_name = slash_pos >= 0 ? selected_path.substring(slash_pos + 1) : selected_path;
    const String selected_fallbacks[] = {
        String("/Adhan/") + file_name,
        String("/adhan/") + file_name,
        String("/") + file_name,
        file_name};

    for (const String& candidate_path : selected_fallbacks) {
      selected = try_open(candidate_path.c_str());
      if (selected) {
        return selected;
      }
    }
  }

  static const char* fallbackFiles[] = {
      "/Adhan/adhan.mp3",
      "/adhan/adhan.mp3",
      "/adhan.mp3",
      "adhan.mp3"};

  for (uint8_t i = 0; i < 4; i++) {
    AudioFileSourceSD* candidate = try_open(fallbackFiles[i]);
    if (candidate) {
      return candidate;
    }
  }

  Serial.println("Adhan: keine Audio-Datei gefunden");
  return nullptr;
}

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

  if (!adhanStarted) {
    startAdhan();
    adhanStarted = isPlaying;
    if (!adhanStarted) {
      changeScreen(get_return_screen());
      return;
    }
  }

  handleAudioTick();
}

void startAdhan() {
  azan_screen_ui_set_prayer_title(prayers[nextPrayerIndex].name.c_str());
  azan_screen_ui_set_hint("Tippen zum Stoppen");

  file = open_adhan_file();
  if (!file) {
    return;
  }

  mp3 = new AudioGeneratorMP3();
  if (mp3->begin(file, out)) {
    isPlaying = true;
  } else {
    Serial.println("Adhan: MP3 Start fehlgeschlagen");
    delete mp3;
    mp3 = nullptr;
    delete file;
    file = nullptr;
  }
}

void stopAdhan() {
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
  changeScreen(get_return_screen());
}

void handleAudioTick() {
  if (isPlaying && mp3 && mp3->isRunning()) {
    if (!mp3->loop()) {
      stopAdhan();
    }
  }
}
