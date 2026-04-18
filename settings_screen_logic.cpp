#include "settings_screen_logic.h"

#include "app_state.h"
#include "home_screen_logic.h"
#include "ui.h"

#include <Preferences.h>
#include <SD.h>
#include <AudioFileSourceSD.h>
#include <AudioGeneratorMP3.h>
#include <AudioOutputI2S.h>
#include <esp_system.h>

extern AudioOutputI2S* out;

namespace {

constexpr uint8_t kMinVolume = 0;
constexpr uint8_t kMaxVolume = 10;
constexpr uint8_t kMaxAdhanSoundIndex = 2;
constexpr char kPrefsNamespace[] = "mawaqit";

Preferences preferences;
bool initialized = false;
bool syncingUi = false;

lv_obj_t* themeRow = nullptr;
lv_obj_t* adhanRow = nullptr;
lv_obj_t* adhanLabel = nullptr;
lv_obj_t* adhanDropdown = nullptr;

lv_obj_t* create_settings_row(lv_obj_t* parent) {
  lv_obj_t* row = lv_obj_create(parent);
  lv_obj_remove_style_all(row);
  lv_obj_set_width(row, lv_pct(100));
  lv_obj_set_height(row, LV_SIZE_CONTENT);
  lv_obj_set_flex_flow(row, LV_FLEX_FLOW_ROW);
  lv_obj_set_flex_align(row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_clear_flag(row, LV_OBJ_FLAG_CLICKABLE | LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_pad_left(row, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(row, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(row, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(row, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_column(row, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
  return row;
}

void save_settings() {
  if (!preferences.begin(kPrefsNamespace, false)) {
    Serial.println("Preferences konnten nicht geoeffnet werden");
    return;
  }

  preferences.putUChar("volume", appSettings.volumeLevel);
  preferences.putBool("dark", appSettings.darkMode);
  preferences.putUChar("adhan", appSettings.adhanSoundIndex);
  preferences.end();
}

void update_theme_label() {
  if (ui_SettingsScreen_Switch_Theme_Label == nullptr) {
    return;
  }

  lv_label_set_text(
      ui_SettingsScreen_Switch_Theme_Label,
      appSettings.darkMode ? "Theme: Dunkel" : "Theme: Hell");
  lv_obj_invalidate(ui_SettingsScreen_Switch_Theme_Label);
}

void restyle_dropdown() {
  if (adhanDropdown == nullptr) {
    return;
  }

  ui_object_set_themeable_style_property(
      adhanDropdown,
      LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_COLOR,
      _ui_theme_color_background);
  ui_object_set_themeable_style_property(
      adhanDropdown,
      LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BG_OPA,
      _ui_theme_alpha_background);
  ui_object_set_themeable_style_property(
      adhanDropdown,
      LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BORDER_COLOR,
      _ui_theme_color_primary);
  ui_object_set_themeable_style_property(
      adhanDropdown,
      LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_BORDER_OPA,
      _ui_theme_alpha_primary);
  ui_object_set_themeable_style_property(
      adhanDropdown,
      LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR,
      _ui_theme_color_title);
  ui_object_set_themeable_style_property(
      adhanDropdown,
      LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA,
      _ui_theme_alpha_title);
  lv_obj_set_style_border_width(adhanDropdown, 1, LV_PART_MAIN | LV_STATE_DEFAULT);
}

void layout_settings_screen() {
  if (ui_Screen_SettingsScreen == nullptr ||
      ui_SettingsScreen_Container_Container11 == nullptr ||
      ui_SettingsScreen_Container_Container18 == nullptr) {
    return;
  }

  lv_obj_add_flag(ui_Screen_SettingsScreen, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_scroll_dir(ui_Screen_SettingsScreen, LV_DIR_VER);
  lv_obj_set_scrollbar_mode(ui_Screen_SettingsScreen, LV_SCROLLBAR_MODE_AUTO);
  lv_obj_set_style_pad_top(ui_Screen_SettingsScreen, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(ui_Screen_SettingsScreen, 18, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_align(ui_SettingsScreen_Container_Container11, LV_ALIGN_TOP_MID);
  lv_obj_set_x(ui_SettingsScreen_Container_Container11, 0);
  lv_obj_set_y(ui_SettingsScreen_Container_Container11, 10);
  lv_obj_set_height(ui_SettingsScreen_Container_Container11, LV_SIZE_CONTENT);
  lv_obj_set_flex_flow(ui_SettingsScreen_Container_Container11, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(
      ui_SettingsScreen_Container_Container11,
      LV_FLEX_ALIGN_START,
      LV_FLEX_ALIGN_START,
      LV_FLEX_ALIGN_START);
  lv_obj_set_style_pad_row(ui_SettingsScreen_Container_Container11, 14, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_update_layout(ui_SettingsScreen_Container_Container11);

  lv_obj_set_align(ui_SettingsScreen_Container_Container18, LV_ALIGN_TOP_MID);
  lv_obj_set_x(ui_SettingsScreen_Container_Container18, 0);
  lv_obj_set_y(
      ui_SettingsScreen_Container_Container18,
      lv_obj_get_y(ui_SettingsScreen_Container_Container11) +
          lv_obj_get_height(ui_SettingsScreen_Container_Container11) + 24);
}

void create_extra_settings_controls() {
  if (ui_SettingsScreen_Container_Container11 == nullptr || themeRow != nullptr) {
    return;
  }

  themeRow = create_settings_row(ui_SettingsScreen_Container_Container11);

  if (ui_SettingsScreen_Switch_Theme_Label != nullptr) {
    lv_obj_set_parent(ui_SettingsScreen_Switch_Theme_Label, themeRow);
    ui_object_set_themeable_style_property(
        ui_SettingsScreen_Switch_Theme_Label,
        LV_PART_MAIN | LV_STATE_DEFAULT,
        LV_STYLE_TEXT_COLOR,
        _ui_theme_color_title);
    ui_object_set_themeable_style_property(
        ui_SettingsScreen_Switch_Theme_Label,
        LV_PART_MAIN | LV_STATE_DEFAULT,
        LV_STYLE_TEXT_OPA,
        _ui_theme_alpha_title);
    lv_obj_set_style_text_font(
        ui_SettingsScreen_Switch_Theme_Label,
        &lv_font_montserrat_16,
        LV_PART_MAIN | LV_STATE_DEFAULT);
  }

  if (ui_SettingsScreen_Switch_Theme != nullptr) {
    lv_obj_set_parent(ui_SettingsScreen_Switch_Theme, themeRow);
    lv_obj_set_width(ui_SettingsScreen_Switch_Theme, 58);
    lv_obj_set_height(ui_SettingsScreen_Switch_Theme, 30);
    lv_obj_set_style_bg_color(
        ui_SettingsScreen_Switch_Theme,
        lv_color_hex(0xD9D9D9),
        LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(
        ui_SettingsScreen_Switch_Theme,
        lv_color_hex(0x2DA041),
        LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_bg_color(
        ui_SettingsScreen_Switch_Theme,
        lv_color_hex(0xC9A227),
        LV_PART_INDICATOR | LV_STATE_CHECKED);
  }

  adhanRow = create_settings_row(ui_SettingsScreen_Container_Container11);

  adhanLabel = lv_label_create(adhanRow);
  lv_label_set_text(adhanLabel, "Adhan Ton");
  ui_object_set_themeable_style_property(
      adhanLabel,
      LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_COLOR,
      _ui_theme_color_title);
  ui_object_set_themeable_style_property(
      adhanLabel,
      LV_PART_MAIN | LV_STATE_DEFAULT,
      LV_STYLE_TEXT_OPA,
      _ui_theme_alpha_title);
  lv_obj_set_style_text_font(adhanLabel, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

  adhanDropdown = lv_dropdown_create(adhanRow);
  lv_dropdown_set_options(adhanDropdown, "Adhan 1\nAdhan 2\nAdhan 3");
  lv_obj_set_width(adhanDropdown, 130);
  lv_obj_add_flag(adhanDropdown, LV_OBJ_FLAG_CLICKABLE);
  restyle_dropdown();
}

void sync_ui_from_settings() {
  syncingUi = true;

  update_volume_display();

  if (ui_SettingsScreen_Switch_Theme != nullptr) {
    if (appSettings.darkMode) {
      lv_obj_add_state(ui_SettingsScreen_Switch_Theme, LV_STATE_CHECKED);
    } else {
      lv_obj_clear_state(ui_SettingsScreen_Switch_Theme, LV_STATE_CHECKED);
    }
  }

  update_theme_label();

  if (adhanDropdown != nullptr) {
    lv_dropdown_set_selected(adhanDropdown, appSettings.adhanSoundIndex);
  }

  syncingUi = false;
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
  restyle_dropdown();
  update_theme_label();
  updateStatusIcons();

  if (lv_scr_act() != nullptr) {
    lv_obj_invalidate(lv_scr_act());
  }
}

void go_to_home_screen(lv_event_t* e) {
  LV_UNUSED(e);
  changeScreen(SCREEN_HOME);
}

void adhan_dropdown_event_handler(lv_event_t* e) {
  if (syncingUi || adhanDropdown == nullptr) {
    return;
  }

  LV_UNUSED(e);
  appSettings.adhanSoundIndex = lv_dropdown_get_selected(adhanDropdown);
  if (appSettings.adhanSoundIndex > kMaxAdhanSoundIndex) {
    appSettings.adhanSoundIndex = 0;
  }

  save_settings();
}

}  // namespace

void load_settings() {
  appSettings.volumeLevel = 6;
  appSettings.darkMode = false;
  appSettings.adhanSoundIndex = 0;

  if (!preferences.begin(kPrefsNamespace, true)) {
    Serial.println("Preferences konnten nicht gelesen werden");
    return;
  }

  appSettings.volumeLevel = preferences.getUChar("volume", 6);
  appSettings.darkMode = preferences.getBool("dark", false);
  appSettings.adhanSoundIndex = preferences.getUChar("adhan", 0);
  preferences.end();

  if (appSettings.volumeLevel > kMaxVolume) {
    appSettings.volumeLevel = 6;
  }
  if (appSettings.adhanSoundIndex > kMaxAdhanSoundIndex) {
    appSettings.adhanSoundIndex = 0;
  }
}

void apply_settings() {
  apply_theme_to_system();
  update_audio_gain();

  if (initialized) {
    sync_ui_from_settings();
    layout_settings_screen();
  }
}

void settings_screen_init() {
  if (initialized) {
    return;
  }

  create_extra_settings_controls();

  if (ui_SettingsScreen_Button_ResetButton2 != nullptr) {
    lv_obj_add_event_cb(ui_SettingsScreen_Button_ResetButton2, reset_app, LV_EVENT_CLICKED, nullptr);
  }

  if (ui_SettingsScreen_Button_HomeButton != nullptr) {
    lv_obj_add_event_cb(ui_SettingsScreen_Button_HomeButton, go_to_home_screen, LV_EVENT_CLICKED, nullptr);
  }

  if (ui_SettingsScreen_Button_SoundPlusButton1 != nullptr) {
    lv_obj_add_flag(ui_SettingsScreen_Button_SoundPlusButton1, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(
        ui_SettingsScreen_Button_SoundPlusButton1, volume_increase, LV_EVENT_CLICKED, nullptr);
  }

  if (ui_SettingsScreen_Button_SoundMinusButton != nullptr) {
    lv_obj_add_flag(ui_SettingsScreen_Button_SoundMinusButton, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(
        ui_SettingsScreen_Button_SoundMinusButton, volume_decrease, LV_EVENT_CLICKED, nullptr);
  }

  if (ui_SettingsScreen_Switch_Theme != nullptr) {
    lv_obj_add_flag(ui_SettingsScreen_Switch_Theme, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(
        ui_SettingsScreen_Switch_Theme, theme_switch_event_handler, LV_EVENT_VALUE_CHANGED, nullptr);
  }

  if (adhanDropdown != nullptr) {
    lv_obj_add_event_cb(adhanDropdown, adhan_dropdown_event_handler, LV_EVENT_VALUE_CHANGED, nullptr);
  }

  layout_settings_screen();
  sync_ui_from_settings();
  update_audio_gain();

  initialized = true;
}

void theme_switch_event_handler(lv_event_t* e) {
  if (syncingUi) {
    return;
  }

  lv_obj_t* obj = lv_event_get_target(e);
  appSettings.darkMode = lv_obj_has_state(obj, LV_STATE_CHECKED);

  apply_theme_to_system();
  save_settings();
}

void reset_app(lv_event_t* e) {
  LV_UNUSED(e);
  Serial.printf("clicked reset_app\n");
  esp_restart();
}

void settings_screen_loop() {
  if (!initialized) {
    settings_screen_init();
  }

  handleToneTick();
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
  if (ui_SettingsScreen_Label_SoundLevelLabel != nullptr) {
    char buf[10];
    snprintf(buf, sizeof(buf), "%u", appSettings.volumeLevel);
    lv_label_set_text(ui_SettingsScreen_Label_SoundLevelLabel, buf);
    lv_obj_invalidate(ui_SettingsScreen_Label_SoundLevelLabel);
  }
}

void update_audio_gain() {
  if (out != nullptr) {
    out->SetGain(appSettings.volumeLevel / 10.0f);
  }

  Serial.printf("Volume: %u (Gain: %.2f)\n", appSettings.volumeLevel, appSettings.volumeLevel / 10.0f);
}

void handleToneTick() {
  if (mp3Tone && mp3Tone->isRunning()) {
    if (!mp3Tone->loop()) {
      mp3Tone->stop();
      delete mp3Tone;
      mp3Tone = nullptr;

      delete toneFile;
      toneFile = nullptr;
    }
  }
}

void playTone() {
  if (!sdCardOk || out == nullptr) {
    return;
  }

  if (mp3Tone) {
    mp3Tone->stop();
    delete mp3Tone;
    mp3Tone = nullptr;
  }
  if (toneFile) {
    delete toneFile;
    toneFile = nullptr;
  }

  toneFile = new AudioFileSourceSD("/beep.mp3");
  mp3Tone = new AudioGeneratorMP3();

  if (!mp3Tone->begin(toneFile, out)) {
    Serial.println("MP3 Tone start failed!");
    delete mp3Tone;
    mp3Tone = nullptr;
    delete toneFile;
    toneFile = nullptr;
    return;
  }

  update_audio_gain();
  Serial.println("Tone gestartet!");
}
