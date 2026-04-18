#include "settings_screen_ui.h"

#include "ui.h"

#include <stdio.h>

namespace {

bool initialized = false;
bool syncingState = false;

void style_filled_button(lv_obj_t* button) {
  if (button == NULL) return;

  lv_obj_set_style_radius(button, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(button, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_primary);
  ui_object_set_themeable_style_property(button, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_primary);
  lv_obj_set_style_border_width(button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(button, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
}

void style_danger_button(lv_obj_t* button) {
  if (button == NULL) return;

  lv_obj_set_style_radius(button, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(button, lv_color_hex(0xB44242), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(button, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(button, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
}

void style_screen() {
  lv_obj_clear_flag(ui_Screen_SettingsScreen, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_pad_all(ui_Screen_SettingsScreen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(ui_Screen_SettingsScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_background);
  ui_object_set_themeable_style_property(ui_Screen_SettingsScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_background);

  lv_obj_set_parent(ui_SettingsScreen_Container_Container18, ui_SettingsScreen_Container_Container11);
  lv_obj_move_to_index(ui_SettingsScreen_Container_Container18, lv_obj_get_child_cnt(ui_SettingsScreen_Container_Container11) - 1);
  lv_obj_set_x(ui_SettingsScreen_Container_Container18, 0);
  lv_obj_set_y(ui_SettingsScreen_Container_Container18, 0);

  lv_obj_set_width(ui_SettingsScreen_Container_Container11, 296);
  lv_obj_set_height(ui_SettingsScreen_Container_Container11, 224);
  lv_obj_set_align(ui_SettingsScreen_Container_Container11, LV_ALIGN_TOP_MID);
  lv_obj_set_x(ui_SettingsScreen_Container_Container11, 0);
  lv_obj_set_y(ui_SettingsScreen_Container_Container11, 8);
  lv_obj_set_flex_flow(ui_SettingsScreen_Container_Container11, LV_FLEX_FLOW_COLUMN);
  lv_obj_set_flex_align(ui_SettingsScreen_Container_Container11, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
  lv_obj_add_flag(ui_SettingsScreen_Container_Container11, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_scroll_dir(ui_SettingsScreen_Container_Container11, LV_DIR_VER);
  lv_obj_set_scrollbar_mode(ui_SettingsScreen_Container_Container11, LV_SCROLLBAR_MODE_OFF);
  lv_obj_set_style_pad_left(ui_SettingsScreen_Container_Container11, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(ui_SettingsScreen_Container_Container11, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(ui_SettingsScreen_Container_Container11, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(ui_SettingsScreen_Container_Container11, 14, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_row(ui_SettingsScreen_Container_Container11, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(ui_SettingsScreen_Container_Container11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_SettingsScreen_Container_Container11, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_width(ui_SettingsScreen_Container_Container18, lv_pct(100));
  lv_obj_set_height(ui_SettingsScreen_Container_Container18, LV_SIZE_CONTENT);
  lv_obj_set_flex_align(ui_SettingsScreen_Container_Container18, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
  lv_obj_set_style_pad_left(ui_SettingsScreen_Container_Container18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(ui_SettingsScreen_Container_Container18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_column(ui_SettingsScreen_Container_Container18, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(ui_SettingsScreen_Container_Container18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_SettingsScreen_Container_Container18, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_object_set_themeable_style_property(ui_SettingsScreen_Label_SeetingsTitle, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_primary);
  ui_object_set_themeable_style_property(ui_SettingsScreen_Label_Label6, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_primary);
  lv_obj_set_style_text_font(ui_SettingsScreen_Label_SeetingsTitle, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(ui_SettingsScreen_Label_Label6, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_pad_column(ui_SettingsScreen_Container_Container13, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_width(ui_SettingsScreen_Button_SoundMinusButton, 34);
  lv_obj_set_height(ui_SettingsScreen_Button_SoundMinusButton, 30);
  lv_obj_set_width(ui_SettingsScreen_Button_SoundPlusButton1, 34);
  lv_obj_set_height(ui_SettingsScreen_Button_SoundPlusButton1, 30);
  style_filled_button(ui_SettingsScreen_Button_SoundMinusButton);
  style_filled_button(ui_SettingsScreen_Button_SoundPlusButton1);
  lv_obj_set_style_text_font(ui_SettingsScreen_Label_Label9, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(ui_SettingsScreen_Label_Label7, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(ui_SettingsScreen_Label_SoundLevelLabel, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(ui_SettingsScreen_Label_Label9, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(ui_SettingsScreen_Label_Label7, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_width(ui_SettingsScreen_Button_HomeButton, 90);
  lv_obj_set_height(ui_SettingsScreen_Button_HomeButton, 30);
  lv_obj_set_width(ui_SettingsScreen_Button_ResetButton2, 78);
  lv_obj_set_height(ui_SettingsScreen_Button_ResetButton2, 30);
  style_filled_button(ui_SettingsScreen_Button_HomeButton);
  style_danger_button(ui_SettingsScreen_Button_ResetButton2);
  lv_label_set_text(ui_SettingsScreen_Label_Label3, "Home");
  lv_obj_set_style_text_font(ui_SettingsScreen_Label_Label3, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(ui_SettingsScreen_Label_Label5, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(ui_SettingsScreen_Label_Label3, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(ui_SettingsScreen_Label_Label5, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_border_width(ui_SettingsScreen_Container_ThemeRow, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_SettingsScreen_Container_ThemeRow, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(ui_SettingsScreen_Container_AdhanRow, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ui_SettingsScreen_Container_AdhanRow, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_column(ui_SettingsScreen_Container_ThemeRow, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_column(ui_SettingsScreen_Container_AdhanRow, 12, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_width(ui_SettingsScreen_Button_AdhanChangeButton, 96);
  lv_obj_set_height(ui_SettingsScreen_Button_AdhanChangeButton, 30);
  style_filled_button(ui_SettingsScreen_Button_AdhanChangeButton);
  lv_label_set_text(ui_SettingsScreen_Label_AdhanChangeLabel, "Aendern");
  lv_obj_set_style_text_font(ui_SettingsScreen_Label_AdhanChangeLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(ui_SettingsScreen_Label_AdhanChangeLabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_object_set_themeable_style_property(ui_SettingsScreen_Switch_Theme_Label, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_title);
  ui_object_set_themeable_style_property(ui_SettingsScreen_Label_AdhanLabel, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_title);
  lv_obj_set_style_text_font(ui_SettingsScreen_Switch_Theme_Label, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(ui_SettingsScreen_Label_AdhanLabel, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_width(ui_SettingsScreen_Label_AdhanLabel, 168);
  lv_label_set_long_mode(ui_SettingsScreen_Label_AdhanLabel, LV_LABEL_LONG_DOT);

  lv_obj_set_width(ui_SettingsScreen_Switch_Theme, 50);
  lv_obj_set_height(ui_SettingsScreen_Switch_Theme, 24);
  lv_obj_set_style_bg_color(ui_SettingsScreen_Switch_Theme, lv_color_hex(0xCFD8D2), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(ui_SettingsScreen_Switch_Theme, lv_color_hex(0x2DA041), LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(ui_SettingsScreen_Switch_Theme, lv_color_hex(0x1A1F1B), LV_PART_INDICATOR | LV_STATE_CHECKED);
  lv_obj_set_style_outline_width(ui_SettingsScreen_Switch_Theme, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
}

}  // namespace

void settings_screen_ui_init(
    lv_event_cb_t reset_cb,
    lv_event_cb_t home_cb,
    lv_event_cb_t volume_minus_cb,
    lv_event_cb_t volume_plus_cb,
    lv_event_cb_t theme_cb,
    lv_event_cb_t adhan_change_cb) {
  if (initialized || ui_Screen_SettingsScreen == NULL) return;

  style_screen();
  lv_obj_add_event_cb(ui_SettingsScreen_Button_ResetButton2, reset_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(ui_SettingsScreen_Button_HomeButton, home_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(ui_SettingsScreen_Button_SoundMinusButton, volume_minus_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(ui_SettingsScreen_Button_SoundPlusButton1, volume_plus_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(ui_SettingsScreen_Switch_Theme, theme_cb, LV_EVENT_VALUE_CHANGED, NULL);
  lv_obj_add_event_cb(ui_SettingsScreen_Button_AdhanChangeButton, adhan_change_cb, LV_EVENT_CLICKED, NULL);
  initialized = true;
}

void settings_screen_ui_set_volume(uint8_t volume) {
  char buf[8];
  snprintf(buf, sizeof(buf), "%u", volume);
  lv_label_set_text(ui_SettingsScreen_Label_SoundLevelLabel, buf);
}

void settings_screen_ui_set_theme(bool dark_mode) {
  syncingState = true;
  if (dark_mode) lv_obj_add_state(ui_SettingsScreen_Switch_Theme, LV_STATE_CHECKED);
  else lv_obj_clear_state(ui_SettingsScreen_Switch_Theme, LV_STATE_CHECKED);
  syncingState = false;
}

void settings_screen_ui_set_theme_label(const char* text) {
  lv_label_set_text(ui_SettingsScreen_Switch_Theme_Label, text);
}

void settings_screen_ui_set_adhan_text(const char* text) {
  String label = "Adhan Ton";
  if (text != nullptr && text[0] != '\0') {
    label += ": ";
    label += text;
  }
  lv_label_set_text(ui_SettingsScreen_Label_AdhanLabel, label.c_str());
}

bool settings_screen_ui_is_syncing() {
  return syncingState;
}
