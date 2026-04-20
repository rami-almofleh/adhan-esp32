#include "audio_manager.h"

#include "app_state.h"

#include <AudioOutputI2S.h>
#include <SD.h>
#include <esp_system.h>

namespace {

constexpr uint8_t kAudioEnablePin = 4;
constexpr int kAudioBclkPin = 32;
constexpr int kAudioLrclkPin = 25;
constexpr int kAudioDataPin = 21;
constexpr long kAthkarWindowStartSeconds = 8L * 3600L;
constexpr long kAthkarWindowEndSeconds = 21L * 3600L;
constexpr long kAthkarMinIntervalSeconds = 30L * 60L;
constexpr long kAthkarMaxIntervalSeconds = 45L * 60L;
constexpr long kAthkarBusyRetrySeconds = 60L;
constexpr long kAthkarStartRetrySeconds = 5L * 60L;
constexpr size_t kMaxAthkarFiles = 32;

AudioSource currentSource = AUDIO_SOURCE_NONE;
String currentPath = "";
String athkarFiles[kMaxAthkarFiles];
size_t athkarFileCount = 0;
int lastAthkarIndex = -1;
int athkarScheduleDay = -1;
long athkarNextTriggerSeconds = -1;
bool athkarScheduleValid = false;
bool initialized = false;

uint8_t priority_for_source(AudioSource source) {
  switch (source) {
    case AUDIO_SOURCE_ADHAN:
      return 4;
    case AUDIO_SOURCE_ADHAN_PREVIEW:
      return 3;
    case AUDIO_SOURCE_ATHKAR:
      return 2;
    case AUDIO_SOURCE_SYSTEM_START:
      return 1;
    case AUDIO_SOURCE_NONE:
    default:
      return 0;
  }
}

void set_audio_enabled(bool enabled) {
  digitalWrite(kAudioEnablePin, enabled ? LOW : HIGH);
}

Screen get_adhan_return_screen() {
  return screenBeforeAzan == SCREEN_AZAN ? SCREEN_HOME : screenBeforeAzan;
}

void clear_runtime_state() {
  currentSource = AUDIO_SOURCE_NONE;
  currentPath = "";
  isPlaying = false;
}

void destroy_active_audio() {
  if (mp3 != nullptr) {
    mp3->stop();
    delete mp3;
    mp3 = nullptr;
  }

  if (file != nullptr) {
    delete file;
    file = nullptr;
  }

  set_audio_enabled(false);
}

void stop_active_audio_internal(bool restore_screen) {
  const AudioSource stoppedSource = currentSource;

  destroy_active_audio();
  clear_runtime_state();

  if (restore_screen && stoppedSource == AUDIO_SOURCE_ADHAN && currentScreen == SCREEN_AZAN) {
    changeScreen(get_adhan_return_screen());
  }
}

String basename_from_path(const String& path) {
  int slashPos = path.lastIndexOf('/');
  return slashPos >= 0 ? path.substring(slashPos + 1) : path;
}

bool is_mp3_file(const String& fileName) {
  if (fileName.length() == 0 || fileName.startsWith(".")) {
    return false;
  }

  String lower = basename_from_path(fileName);
  lower.toLowerCase();
  return lower.endsWith(".mp3");
}

void append_candidate_if_unique(String* candidates, size_t capacity, size_t& count, const String& candidate) {
  if (candidate.length() == 0) {
    return;
  }

  if (count >= capacity) {
    return;
  }

  for (size_t i = 0; i < count; i++) {
    if (candidates[i] == candidate) {
      return;
    }
  }

  candidates[count++] = candidate;
}

AudioFileSourceSD* try_open_candidate(const String& path) {
  AudioFileSourceSD* source = new AudioFileSourceSD(path.c_str());
  if (source != nullptr && source->isOpen()) {
    Serial.printf("Audio: verwende %s\n", path.c_str());
    return source;
  }

  delete source;
  return nullptr;
}

AudioFileSourceSD* open_with_candidates(const String* candidates, size_t count) {
  for (size_t i = 0; i < count; i++) {
    AudioFileSourceSD* source = try_open_candidate(candidates[i]);
    if (source != nullptr) {
      return source;
    }
  }

  return nullptr;
}

AudioFileSourceSD* open_preview_or_adhan_file(const char* filePath, bool allowDefaultAdhanFallback) {
  String candidates[10];
  size_t count = 0;
  const String requested = filePath == nullptr ? "" : String(filePath);

  append_candidate_if_unique(candidates, 10, count, requested);

  if (requested.length() > 0) {
    const String fileName = basename_from_path(requested);
    append_candidate_if_unique(candidates, 10, count, String("/Adhan/") + fileName);
    append_candidate_if_unique(candidates, 10, count, String("/adhan/") + fileName);
    append_candidate_if_unique(candidates, 10, count, String("/") + fileName);
    append_candidate_if_unique(candidates, 10, count, fileName);
  }

  if (allowDefaultAdhanFallback) {
    append_candidate_if_unique(candidates, 10, count, "/Adhan/adhan.mp3");
    append_candidate_if_unique(candidates, 10, count, "/adhan/adhan.mp3");
    append_candidate_if_unique(candidates, 10, count, "/adhan.mp3");
    append_candidate_if_unique(candidates, 10, count, "adhan.mp3");
  }

  return open_with_candidates(candidates, count);
}

AudioFileSourceSD* open_athkar_file(const char* filePath) {
  String candidates[6];
  size_t count = 0;
  const String requested = filePath == nullptr ? "" : String(filePath);

  append_candidate_if_unique(candidates, 6, count, requested);

  if (requested.length() > 0) {
    const String fileName = basename_from_path(requested);
    append_candidate_if_unique(candidates, 6, count, String("/athkar/") + fileName);
    append_candidate_if_unique(candidates, 6, count, String("/Athkar/") + fileName);
    append_candidate_if_unique(candidates, 6, count, String("/") + fileName);
    append_candidate_if_unique(candidates, 6, count, fileName);
  }

  return open_with_candidates(candidates, count);
}

AudioFileSourceSD* open_system_file(const char* filePath) {
  String candidates[4];
  size_t count = 0;
  const String requested = filePath == nullptr ? "" : String(filePath);

  append_candidate_if_unique(candidates, 4, count, requested);

  if (requested.length() > 0) {
    const String fileName = basename_from_path(requested);
    append_candidate_if_unique(candidates, 4, count, String("/") + fileName);
    append_candidate_if_unique(candidates, 4, count, fileName);
  }

  append_candidate_if_unique(candidates, 4, count, "/system_start.mp3");
  return open_with_candidates(candidates, count);
}

bool start_audio(AudioSource source, const char* filePath) {
  if (!sdCardOk || out == nullptr) {
    Serial.println("Audio: Start nicht moeglich, SD oder Ausgang fehlt");
    return false;
  }

  const uint8_t requestedPriority = priority_for_source(source);
  const uint8_t activePriority = priority_for_source(currentSource);
  const String requestedPath = filePath == nullptr ? "" : String(filePath);

  if (currentSource != AUDIO_SOURCE_NONE) {
    if (requestedPriority < activePriority) {
      return false;
    }

    if (currentSource == source && currentPath == requestedPath && isPlaying) {
      return true;
    }

    stop_active_audio_internal(false);
  }

  AudioFileSourceSD* nextFile = nullptr;
  switch (source) {
    case AUDIO_SOURCE_SYSTEM_START:
      nextFile = open_system_file(filePath);
      break;
    case AUDIO_SOURCE_ADHAN_PREVIEW:
      nextFile = open_preview_or_adhan_file(filePath, false);
      break;
    case AUDIO_SOURCE_ADHAN:
      nextFile = open_preview_or_adhan_file(filePath, true);
      break;
    case AUDIO_SOURCE_ATHKAR:
      nextFile = open_athkar_file(filePath);
      break;
    case AUDIO_SOURCE_NONE:
    default:
      break;
  }

  if (nextFile == nullptr) {
    Serial.println("Audio: Quelldatei konnte nicht geoeffnet werden");
    return false;
  }

  AudioGeneratorMP3* nextMp3 = new AudioGeneratorMP3();
  set_audio_enabled(true);

  if (!nextMp3->begin(nextFile, out)) {
    Serial.println("Audio: MP3 Start fehlgeschlagen");
    set_audio_enabled(false);
    delete nextMp3;
    delete nextFile;
    return false;
  }

  file = nextFile;
  mp3 = nextMp3;
  currentSource = source;
  currentPath = requestedPath;
  isPlaying = true;
  audio_manager_apply_volume(appSettings.volumeLevel);
  return true;
}

void clear_athkar_files() {
  for (size_t i = 0; i < athkarFileCount; i++) {
    athkarFiles[i] = "";
  }
  athkarFileCount = 0;
}

bool load_athkar_directory(const char* folderPath) {
  File dir = SD.open(folderPath);
  if (!dir || !dir.isDirectory()) {
    if (dir) {
      dir.close();
    }
    return false;
  }

  File entry = dir.openNextFile();
  while (entry && athkarFileCount < kMaxAthkarFiles) {
    if (!entry.isDirectory()) {
      const String name = basename_from_path(entry.name());
      if (is_mp3_file(name)) {
        athkarFiles[athkarFileCount++] = String(folderPath) + "/" + name;
      }
    }
    entry.close();
    entry = dir.openNextFile();
  }

  dir.close();
  return athkarFileCount > 0;
}

void refresh_athkar_files() {
  clear_athkar_files();

  if (!sdCardOk) {
    athkarScheduleValid = false;
    return;
  }

  if (!load_athkar_directory("/athkar")) {
    load_athkar_directory("/Athkar");
  }

  athkarScheduleValid = false;
  Serial.printf("Athkar Dateien gefunden: %u\n", static_cast<unsigned>(athkarFileCount));
}

long now_seconds_of_day() {
  return globalTime.tm_hour * 3600L + globalTime.tm_min * 60L + globalTime.tm_sec;
}

void invalidate_athkar_schedule() {
  athkarScheduleValid = false;
  athkarNextTriggerSeconds = -1;
}

void schedule_next_athkar(long baseSeconds) {
  const long intervalSeconds = random(kAthkarMinIntervalSeconds, kAthkarMaxIntervalSeconds + 1);
  const long nextTrigger = baseSeconds + intervalSeconds;
  athkarScheduleDay = globalTime.tm_yday;

  if (nextTrigger >= kAthkarWindowEndSeconds) {
    invalidate_athkar_schedule();
    return;
  }

  athkarNextTriggerSeconds = nextTrigger;
  athkarScheduleValid = true;
  Serial.printf(
      "Athkar naechster Start in %ld Sekunden um %02ld:%02ld:%02ld\n",
      intervalSeconds,
      nextTrigger / 3600L,
      (nextTrigger % 3600L) / 60L,
      nextTrigger % 60L);
}

void schedule_athkar_retry(long nowSeconds, long retrySeconds) {
  const long nextTrigger = nowSeconds + retrySeconds;
  athkarScheduleDay = globalTime.tm_yday;

  if (nextTrigger >= kAthkarWindowEndSeconds) {
    invalidate_athkar_schedule();
    return;
  }

  athkarNextTriggerSeconds = nextTrigger;
  athkarScheduleValid = true;
}

int pick_random_athkar_index() {
  if (athkarFileCount == 0) {
    return -1;
  }

  int index = random(static_cast<long>(athkarFileCount));
  if (athkarFileCount > 1) {
    while (index == lastAthkarIndex) {
      index = random(static_cast<long>(athkarFileCount));
    }
  }

  return index;
}

void handle_athkar_schedule() {
  if (currentStatus != ST_RUNNING || !timeValid || athkarFileCount == 0) {
    return;
  }

  const long nowSeconds = now_seconds_of_day();
  if (athkarScheduleDay != globalTime.tm_yday) {
    invalidate_athkar_schedule();
    athkarScheduleDay = globalTime.tm_yday;
  }

  if (nowSeconds >= kAthkarWindowEndSeconds) {
    invalidate_athkar_schedule();
    return;
  }

  if (!athkarScheduleValid) {
    schedule_next_athkar(nowSeconds < kAthkarWindowStartSeconds ? kAthkarWindowStartSeconds : nowSeconds);
    return;
  }

  if (nowSeconds < kAthkarWindowStartSeconds || nowSeconds < athkarNextTriggerSeconds) {
    return;
  }

  if (audio_manager_is_any_active()) {
    schedule_athkar_retry(nowSeconds, kAthkarBusyRetrySeconds);
    return;
  }

  const int athkarIndex = pick_random_athkar_index();
  if (athkarIndex < 0) {
    schedule_athkar_retry(nowSeconds, kAthkarStartRetrySeconds);
    return;
  }

  if (start_audio(AUDIO_SOURCE_ATHKAR, athkarFiles[athkarIndex].c_str())) {
    lastAthkarIndex = athkarIndex;
    schedule_next_athkar(nowSeconds);
  } else {
    schedule_athkar_retry(nowSeconds, kAthkarStartRetrySeconds);
  }
}

}  // namespace

void audio_manager_init() {
  if (initialized) {
    return;
  }

  pinMode(kAudioEnablePin, OUTPUT);
  set_audio_enabled(false);

  if (out == nullptr) {
    AudioOutputI2S* i2sOut = new AudioOutputI2S();
    i2sOut->SetPinout(kAudioBclkPin, kAudioLrclkPin, kAudioDataPin);
    i2sOut->begin();
    out = i2sOut;
  }

  audio_manager_apply_volume(appSettings.volumeLevel);
  randomSeed(esp_random());
  initialized = true;
}

void audio_manager_loop() {
  if (mp3 != nullptr) {
    if (mp3->isRunning()) {
      if (!mp3->loop()) {
        stop_active_audio_internal(true);
      }
    } else {
      stop_active_audio_internal(true);
    }
  }

  handle_athkar_schedule();
}

void audio_manager_on_sd_ready(bool available) {
  sdCardOk = available;
  if (!available) {
    clear_athkar_files();
    invalidate_athkar_schedule();
    return;
  }

  refresh_athkar_files();
}

void audio_manager_apply_volume(uint8_t volumeLevel) {
  if (out != nullptr) {
    out->SetGain(volumeLevel / 10.0f);
  }
}

bool audio_manager_play_preview(const char* filePath) {
  return start_audio(AUDIO_SOURCE_ADHAN_PREVIEW, filePath);
}

bool audio_manager_play_system_start(const char* filePath) {
  return start_audio(AUDIO_SOURCE_SYSTEM_START, filePath);
}

bool audio_manager_play_adhan_file(const char* filePath) {
  return start_audio(AUDIO_SOURCE_ADHAN, filePath);
}

void audio_manager_stop_active() {
  stop_active_audio_internal(true);
}

void audio_manager_stop_source(AudioSource source) {
  if (currentSource != source) {
    return;
  }

  stop_active_audio_internal(true);
}

bool audio_manager_is_any_active() {
  return currentSource != AUDIO_SOURCE_NONE && isPlaying;
}

bool audio_manager_is_source_active(AudioSource source) {
  return currentSource == source && isPlaying;
}

AudioSource audio_manager_current_source() {
  return currentSource;
}

const char* audio_manager_current_path() {
  return currentPath.c_str();
}
