#include "settings_screen_logic.h"

#include "audio_manager.h"
#include "app_state.h"
#include "home_screen_logic.h"
#include "settings_screen_ui.h"
#include "ui.h"

#include <FS.h>
#include <Preferences.h>
#include <SD.h>
#include <esp_system.h>

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
AdhanOption adhanOptions[kMaxAdhanFiles];
size_t adhanOptionCount = 0;
String persistedAdhanName = "";

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

String basename_from_path(const String& file_name) {
  String base = file_name;
  int slash_pos = base.lastIndexOf('/');
  if (slash_pos >= 0) {
    base = base.substring(slash_pos + 1);
  }
  return base;
}

bool equals_ignore_case(String left, String right) {
  left.toLowerCase();
  right.toLowerCase();
  return left == right;
}

bool is_mp3_file(const String& file_name) {
  String base_name = basename_from_path(file_name);

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
  persistedAdhanName = basename_from_path(appSettings.adhanSoundFile);
  preferences.putUChar("volume", appSettings.volumeLevel);
  preferences.putBool("dark", appSettings.darkMode);
  preferences.putString("adhan_file", appSettings.adhanSoundFile.c_str());
  preferences.putString("adhan_name", persistedAdhanName.c_str());
  preferences.end();
  Serial.printf(
      "Adhan gespeichert: pfad=%s name=%s\n",
      appSettings.adhanSoundFile.c_str(),
      persistedAdhanName.c_str());
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
    return;
  }

  for (size_t i = 0; i < adhanOptionCount; i++) {
    if (adhanOptions[i].filePath == appSettings.adhanSoundFile) {
      persistedAdhanName = basename_from_path(appSettings.adhanSoundFile);
      return;
    }
  }

  String wanted_name = persistedAdhanName;
  if (wanted_name.length() == 0 && appSettings.adhanSoundFile.length() > 0) {
    wanted_name = basename_from_path(appSettings.adhanSoundFile);
  }

  if (wanted_name.length() > 0) {
    for (size_t i = 0; i < adhanOptionCount; i++) {
      if (equals_ignore_case(basename_from_path(adhanOptions[i].filePath), wanted_name)) {
        appSettings.adhanSoundFile = adhanOptions[i].filePath;
        persistedAdhanName = basename_from_path(appSettings.adhanSoundFile);
        return;
      }
    }
  }

  appSettings.adhanSoundFile = adhanOptions[0].filePath;
  persistedAdhanName = basename_from_path(appSettings.adhanSoundFile);
}

void refresh_adhan_files() {
  String previous_file = appSettings.adhanSoundFile;
  String previous_name = persistedAdhanName;
  clear_adhan_options();

  if (sdCardOk) {
    if (!load_adhan_directory("/Adhan")) {
      load_adhan_directory("/adhan");
    }
    sort_adhan_options();
  }

  adhanFilesLoaded = adhanOptionCount > 0;
  Serial.printf("Adhan Dateien gefunden: %u\n", static_cast<unsigned>(adhanOptionCount));

  if (adhanOptionCount == 0) {
    appSettings.adhanSoundFile = previous_file;
    persistedAdhanName = previous_name;
    return;
  }

  ensure_default_adhan_selection();

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
  persistedAdhanName = "";

  if (!preferences.begin(kPrefsNamespace, true)) return;
  appSettings.volumeLevel = preferences.getUChar("volume", 6);
  appSettings.darkMode = preferences.getBool("dark", false);
  appSettings.adhanSoundFile = preferences.getString("adhan_file", "");
  persistedAdhanName = preferences.getString("adhan_name", "");
  preferences.end();

  Serial.printf(
      "Adhan geladen: pfad=%s name=%s\n",
      appSettings.adhanSoundFile.c_str(),
      persistedAdhanName.c_str());

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
  if (sdCardOk) {
    refresh_adhan_files();
  }
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
  audio_manager_apply_volume(appSettings.volumeLevel);
}

void handleToneTick() {
  // Audio wird global im Audio-Manager getaktet.
}

void playTone() {
  if (currentScreen != SCREEN_SETTINGS) return;
  if (audio_manager_play_preview("/beep.mp3")) return;
  if (appSettings.adhanSoundFile.length() > 0) {
    audio_manager_play_preview(appSettings.adhanSoundFile.c_str());
  }
}

void settings_screen_stop_preview() {
  audio_manager_stop_source(AUDIO_SOURCE_ADHAN_PREVIEW);
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
  return index < adhanOptionCount && audio_manager_is_source_active(AUDIO_SOURCE_ADHAN_PREVIEW) &&
         adhanOptions[index].filePath == audio_manager_current_path();
}

bool settings_screen_select_adhan_option(size_t index) {
  if (index >= adhanOptionCount) return false;
  appSettings.adhanSoundFile = adhanOptions[index].filePath;
  persistedAdhanName = basename_from_path(appSettings.adhanSoundFile);
  Serial.printf("Adhan ausgewaehlt: %s\n", appSettings.adhanSoundFile.c_str());
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
  return audio_manager_play_preview(adhanOptions[index].filePath.c_str());
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
    persistedAdhanName = basename_from_path(appSettings.adhanSoundFile);
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
