#include "settings_screen_logic.h"

#include "app_state.h"
#include "home_screen_logic.h"
#include "settings_screen_ui.h"
#include "ui.h"

#include <AudioFileSourceSD.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>
#include <FS.h>
#include <Preferences.h>
#include <SD.h>
#include <esp_system.h>

extern AudioOutputI2S* out;

namespace {

constexpr uint8_t kMinVolume = 0;
constexpr uint8_t kMaxVolume = 10;
constexpr size_t kMaxAdhanFiles = 24;
constexpr char kPrefsNamespace[] = "mawaqit";

struct AdhanOption {
  String filePath;
  String displayName;
};

Preferences preferences;
bool initialized = false;
bool adhanFilesLoaded = false;
bool previewActive = false;
int previewOptionIndex = -1;
AdhanOption adhanOptions[kMaxAdhanFiles];
size_t adhanOptionCount = 0;

AudioFileSourceSD* open_audio_file(const char* file_path) {
  if (file_path == nullptr || file_path[0] == '\0') {
    Serial.println("Audio: leerer Dateipfad");
    return nullptr;
  }

  AudioFileSourceSD* source = new AudioFileSourceSD(file_path);
  if (source && source->isOpen()) {
    Serial.printf("Audio: verwende %s\n", file_path);
    return source;
  }
  delete source;

  String path(file_path);
  int slash_pos = path.lastIndexOf('/');
  String file_name = slash_pos >= 0 ? path.substring(slash_pos + 1) : path;
  const String fallbacks[] = {
      String("/Adhan/") + file_name,
      String("/adhan/") + file_name,
      String("/") + file_name,
      file_name};

  for (const String& candidate : fallbacks) {
    source = new AudioFileSourceSD(candidate.c_str());
    if (source && source->isOpen()) {
      Serial.printf("Audio: Fallback %s\n", candidate.c_str());
      return source;
    }
    delete source;
  }

  Serial.printf("Audio: Datei nicht gefunden %s\n", file_path);
  return nullptr;
}

String sanitize_display_name(const String& file_name) {
  String base = file_name;
  int slash_pos = base.lastIndexOf('/');
  if (slash_pos >= 0) base = base.substring(slash_pos + 1);

  String lower = base;
  lower.toLowerCase();
  if (lower.endsWith(".mp3")) {
    base.remove(base.length() - 4);
  }
  return base;
}

bool is_mp3_file(const String& file_name) {
  String base_name = file_name;
  int slash_pos = base_name.lastIndexOf('/');
  if (slash_pos >= 0) {
    base_name = base_name.substring(slash_pos + 1);
  }

  if (base_name.length() == 0 || base_name.startsWith(".")) {
    return false;
  }

  String lower = base_name;
  lower.toLowerCase();
  return lower.endsWith(".mp3");
}

void clear_adhan_options() {
  for (size_t i = 0; i < adhanOptionCount; i++) {
    adhanOptions[i].filePath = "";
    adhanOptions[i].displayName = "";
  }
  adhanOptionCount = 0;
}

void sort_adhan_options() {
  for (size_t i = 0; i < adhanOptionCount; i++) {
    for (size_t j = i + 1; j < adhanOptionCount; j++) {
      String left = adhanOptions[i].displayName;
      String right = adhanOptions[j].displayName;
      left.toLowerCase();
      right.toLowerCase();
      if (right < left) {
        AdhanOption tmp = adhanOptions[i];
        adhanOptions[i] = adhanOptions[j];
        adhanOptions[j] = tmp;
      }
    }
  }
}

bool load_adhan_directory(const char* folder_path) {
  File dir = SD.open(folder_path);
  if (!dir || !dir.isDirectory()) {
    if (dir) dir.close();
    return false;
  }

  File entry = dir.openNextFile();
  while (entry && adhanOptionCount < kMaxAdhanFiles) {
    if (!entry.isDirectory()) {
      String name = entry.name();
      if (is_mp3_file(name)) {
        int slash_pos = name.lastIndexOf('/');
        String base_name = slash_pos >= 0 ? name.substring(slash_pos + 1) : name;
        adhanOptions[adhanOptionCount].filePath = String(folder_path) + "/" + base_name;
        adhanOptions[adhanOptionCount].displayName = sanitize_display_name(base_name);
        adhanOptionCount++;
      } else {
        Serial.printf("Adhan Datei ignoriert: %s\n", name.c_str());
      }
    }
    entry.close();
    entry = dir.openNextFile();
  }

  dir.close();
  return adhanOptionCount > 0;
}

void save_settings() {
  if (!preferences.begin(kPrefsNamespace, false)) return;
  preferences.putUChar("volume", appSettings.volumeLevel);
  preferences.putBool("dark", appSettings.darkMode);
  preferences.putString("adhan_file", appSettings.adhanSoundFile.c_str());
  preferences.end();
}

void stop_preview_audio_internal() {
  if (mp3Tone) {
    mp3Tone->stop();
    delete mp3Tone;
    mp3Tone = nullptr;
  }
  if (toneFile) {
    delete toneFile;
    toneFile = nullptr;
  }
  previewActive = false;
  previewOptionIndex = -1;
}

bool start_audio_file(const char* file_path, int option_index) {
  if (!sdCardOk || out == nullptr || file_path == nullptr || file_path[0] == '\0') {
    Serial.println("Audio: Start nicht moeglich (SD/out/Pfad)");
    stop_preview_audio_internal();
    return false;
  }

  stop_preview_audio_internal();
  toneFile = open_audio_file(file_path);
  if (toneFile == nullptr) {
    return false;
  }

  mp3Tone = new AudioGeneratorMP3();
  if (!mp3Tone->begin(toneFile, out)) {
    Serial.printf("Audio: MP3 Start fehlgeschlagen %s\n", file_path);
    delete mp3Tone;
    mp3Tone = nullptr;
    delete toneFile;
    toneFile = nullptr;
    return false;
  }

  previewActive = true;
  previewOptionIndex = option_index;
  update_audio_gain();
  return true;
}

int find_selected_adhan_index() {
  for (size_t i = 0; i < adhanOptionCount; i++) {
    if (adhanOptions[i].filePath == appSettings.adhanSoundFile) {
      return static_cast<int>(i);
    }
  }
  return 0;
}

String selected_adhan_label() {
  int index = find_selected_adhan_index();
  if (index >= 0 && static_cast<size_t>(index) < adhanOptionCount) {
    return adhanOptions[index].displayName;
  }
  return "Keine Datei";
}

void ensure_default_adhan_selection() {
  if (adhanOptionCount == 0) {
    appSettings.adhanSoundFile = "";
    return;
  }
  for (size_t i = 0; i < adhanOptionCount; i++) {
    if (adhanOptions[i].filePath == appSettings.adhanSoundFile) return;
  }
  appSettings.adhanSoundFile = adhanOptions[0].filePath;
}

void refresh_adhan_files() {
  String previous_file = appSettings.adhanSoundFile;
  clear_adhan_options();

  if (sdCardOk) {
    if (!load_adhan_directory("/Adhan")) {
      load_adhan_directory("/adhan");
    }
    sort_adhan_options();
  }

  ensure_default_adhan_selection();
  adhanFilesLoaded = adhanOptionCount > 0;
  Serial.printf("Adhan Dateien gefunden: %u\n", static_cast<unsigned>(adhanOptionCount));

  if (previous_file != appSettings.adhanSoundFile) {
    Serial.printf("Adhan Default/Selection gesetzt: %s\n", appSettings.adhanSoundFile.c_str());
    save_settings();
  }
}

void update_theme_label() {
  settings_screen_ui_set_theme_label(appSettings.darkMode ? "Theme: Dunkel" : "Theme: Hell");
}

void sync_settings_ui() {
  if (!initialized) return;
  settings_screen_ui_set_volume(appSettings.volumeLevel);
  settings_screen_ui_set_theme(appSettings.darkMode);
  update_theme_label();
  settings_screen_ui_set_adhan_text(selected_adhan_label().c_str());
}

void apply_theme_to_system() {
  lv_disp_t* disp = lv_disp_get_default();
  if (disp != nullptr) {
    lv_theme_t* theme = lv_theme_default_init(
        disp,
        lv_palette_main(LV_PALETTE_GREEN),
        lv_palette_main(LV_PALETTE_RED),
        appSettings.darkMode,
        LV_FONT_DEFAULT);
    lv_disp_set_theme(disp, theme);
  }

  ui_theme_set(appSettings.darkMode ? UI_THEME_DARK : UI_THEME_LIGHT);
  sync_settings_ui();
  updateStatusIcons();
  if (ui_Screen_SettingsScreen) lv_obj_invalidate(ui_Screen_SettingsScreen);
  if (ui_Screen_AdhanSelectScreen) lv_obj_invalidate(ui_Screen_AdhanSelectScreen);
  if (ui_Screen_HomeScreen) lv_obj_invalidate(ui_Screen_HomeScreen);
  if (ui_Screen_PrayersScreen) lv_obj_invalidate(ui_Screen_PrayersScreen);
  if (ui_Screen_AzanScreen) lv_obj_invalidate(ui_Screen_AzanScreen);
}

void go_to_home_screen(lv_event_t* e) {
  LV_UNUSED(e);
  changeScreen(SCREEN_HOME);
}

}  // namespace

void load_settings() {
  appSettings.volumeLevel = 6;
  appSettings.darkMode = false;
  appSettings.adhanSoundFile = "";

  if (!preferences.begin(kPrefsNamespace, true)) return;
  appSettings.volumeLevel = preferences.getUChar("volume", 6);
  appSettings.darkMode = preferences.getBool("dark", false);
  appSettings.adhanSoundFile = preferences.getString("adhan_file", "");
  preferences.end();

  if (appSettings.volumeLevel > kMaxVolume) {
    appSettings.volumeLevel = 6;
  }
}

void apply_settings() {
  apply_theme_to_system();
  update_audio_gain();
  if (!adhanFilesLoaded && sdCardOk) {
    refresh_adhan_files();
  }
  sync_settings_ui();
}

void settings_screen_init() {
  if (initialized) return;

  settings_screen_ui_init(
      reset_app,
      go_to_home_screen,
      volume_decrease,
      volume_increase,
      theme_switch_event_handler,
      settings_screen_open_adhan_selector);

  initialized = true;
  refresh_adhan_files();
  sync_settings_ui();
  update_audio_gain();
}

void settings_screen_loop() {
  if (!initialized) {
    settings_screen_init();
  }
  if (!adhanFilesLoaded && sdCardOk) {
    refresh_adhan_files();
    sync_settings_ui();
  }
  handleToneTick();
}

void theme_switch_event_handler(lv_event_t* e) {
  if (settings_screen_ui_is_syncing()) return;
  lv_obj_t* obj = lv_event_get_target(e);
  appSettings.darkMode = lv_obj_has_state(obj, LV_STATE_CHECKED);
  apply_theme_to_system();
  save_settings();
}

void volume_increase(lv_event_t* e) {
  LV_UNUSED(e);
  if (appSettings.volumeLevel < kMaxVolume) {
    appSettings.volumeLevel++;
    update_volume_display();
    update_audio_gain();
    save_settings();
    playTone();
  }
}

void volume_decrease(lv_event_t* e) {
  LV_UNUSED(e);
  if (appSettings.volumeLevel > kMinVolume) {
    appSettings.volumeLevel--;
    update_volume_display();
    update_audio_gain();
    save_settings();
    playTone();
  }
}

void update_volume_display() {
  settings_screen_ui_set_volume(appSettings.volumeLevel);
}

void update_audio_gain() {
  if (out != nullptr) {
    out->SetGain(appSettings.volumeLevel / 10.0f);
  }
}

void handleToneTick() {
  if (mp3Tone) {
    if (mp3Tone->isRunning()) {
      if (!mp3Tone->loop()) {
        stop_preview_audio_internal();
      }
    } else {
      stop_preview_audio_internal();
    }
  }
}

void playTone() {
  if (currentScreen != SCREEN_SETTINGS) return;
  if (start_audio_file("/beep.mp3", -1)) return;
  if (appSettings.adhanSoundFile.length() > 0) {
    start_audio_file(appSettings.adhanSoundFile.c_str(), -1);
  }
}

void settings_screen_stop_preview() {
  stop_preview_audio_internal();
}

void reset_app(lv_event_t* e) {
  LV_UNUSED(e);
  settings_screen_stop_preview();
  esp_restart();
}

size_t settings_screen_get_adhan_option_count() {
  return adhanOptionCount;
}

String settings_screen_get_adhan_option_label(size_t index) {
  if (index >= adhanOptionCount) return "";
  return adhanOptions[index].displayName;
}

bool settings_screen_is_selected_adhan_option(size_t index) {
  return index < adhanOptionCount && adhanOptions[index].filePath == appSettings.adhanSoundFile;
}

bool settings_screen_is_previewing_option(size_t index) {
  return previewActive && previewOptionIndex == static_cast<int>(index);
}

bool settings_screen_select_adhan_option(size_t index) {
  if (index >= adhanOptionCount) return false;
  appSettings.adhanSoundFile = adhanOptions[index].filePath;
  save_settings();
  sync_settings_ui();
  return true;
}

bool settings_screen_play_preview_for_option(size_t index) {
  if (index >= adhanOptionCount) {
    Serial.printf("Audio: ungueltiger Auswahlindex %u\n", static_cast<unsigned>(index));
    return false;
  }
  Serial.printf("Audio: Vorschau fuer %s\n", adhanOptions[index].filePath.c_str());
  return start_audio_file(adhanOptions[index].filePath.c_str(), static_cast<int>(index));
}

void settings_screen_reload_adhan_files() {
  refresh_adhan_files();
  sync_settings_ui();
}

String settings_screen_get_effective_adhan_file() {
  if ((!adhanFilesLoaded || adhanOptionCount == 0) && sdCardOk) {
    refresh_adhan_files();
  }

  if (appSettings.adhanSoundFile.length() > 0) {
    return appSettings.adhanSoundFile;
  }

  if (adhanOptionCount > 0) {
    appSettings.adhanSoundFile = adhanOptions[0].filePath;
    save_settings();
    Serial.printf("Adhan Fallback auf erste Datei: %s\n", appSettings.adhanSoundFile.c_str());
    return appSettings.adhanSoundFile;
  }

  return "";
}

void settings_screen_open_adhan_selector(lv_event_t* e) {
  LV_UNUSED(e);
  settings_screen_stop_preview();
  changeScreen(SCREEN_ADHAN_SELECT);
}
