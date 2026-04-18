#pragma once
#include <Arduino.h>
#include <lvgl.h>
#include <AudioOutputI2S.h>
#include <AudioGeneratorMP3.h>
#include <AudioFileSourceSD.h>

enum SystemStatus { ST_START, ST_INIT_SD, ST_CONNECT_WIFI, ST_GET_DATA, ST_RUNNING };

enum Screen {
  SCREEN_START,
  SCREEN_HOME,
  SCREEN_PRAYERS,
  SCREEN_SETTINGS,
  SCREEN_AZAN
};

enum B_Hardware {
  H_TOUCH,
  H_TFT,
  H_SD
};

struct AppSettings {
  uint8_t volumeLevel;
  bool darkMode;
  uint8_t adhanSoundIndex;
};

struct Prayer {
  String name;
  String time;
  long seconds;
};

extern bool isPlaying;
extern bool sdCardOk;
extern SystemStatus currentStatus;
extern Screen currentScreen;
extern Screen screenBeforeAzan;
extern Prayer prayers[5];
extern int nextPrayerIndex;
extern struct tm globalTime;
extern bool timeValid;
extern AppSettings appSettings;
extern AudioOutputI2S* out;
extern AudioGeneratorMP3* mp3;
extern AudioFileSourceSD* file;
extern AudioFileSourceSD* toneFile;
extern AudioGeneratorMP3* mp3Tone;
extern AudioFileSourceSD* systemStartAudioFile;
extern AudioGeneratorMP3* systemStartAudioMp3;
extern const uint8_t TFT_H_CS;
extern const uint8_t SD_CS;
extern const uint8_t TOUCH_CS;

void changeScreen(Screen newScreen);
void initHardware(B_Hardware hardewareTitle);
