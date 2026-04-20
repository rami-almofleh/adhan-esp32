#pragma once

#include <Arduino.h>

enum AudioSource {
  AUDIO_SOURCE_NONE,
  AUDIO_SOURCE_SYSTEM_START,
  AUDIO_SOURCE_ATHKAR,
  AUDIO_SOURCE_ADHAN_PREVIEW,
  AUDIO_SOURCE_ADHAN
};

void audio_manager_init();
void audio_manager_loop();
void audio_manager_on_sd_ready(bool available);
void audio_manager_apply_volume(uint8_t volume_level);

bool audio_manager_play_preview(const char* file_path);
bool audio_manager_play_system_start(const char* file_path);
bool audio_manager_play_adhan_file(const char* file_path);
void audio_manager_stop_active();
void audio_manager_stop_source(AudioSource source);

bool audio_manager_is_any_active();
bool audio_manager_is_source_active(AudioSource source);
AudioSource audio_manager_current_source();
const char* audio_manager_current_path();
