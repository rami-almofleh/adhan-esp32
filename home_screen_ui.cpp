#include "home_screen_ui.h"

#include "ui.h"

#include <Arduino.h>

namespace {

constexpr lv_coord_t kContentHeightDefault = 186;
constexpr uint32_t kIconActive = 0x2DA041;
constexpr uint32_t kIconInactive = 0x9CB0A2;

bool initialized = false;

void set_text(lv_obj_t* label, const char* text) {
  if (label == NULL) return;
  lv_label_set_text(label, text);
  lv_obj_invalidate(label);
}

void style_button(lv_obj_t* button, bool filled) {
  if (button == NULL) return;

  lv_obj_set_width(button, 96);
  lv_obj_set_height(button, 30);
  lv_obj_set_style_radius(button, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  if (filled) {
    ui_object_set_themeable_style_property(button, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_primary);
    ui_object_set_themeable_style_property(button, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_primary);
    lv_obj_set_style_text_color(button, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  } else {
    ui_object_set_themeable_style_property(button, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_background);
    ui_object_set_themeable_style_property(button, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_background);
    ui_object_set_themeable_style_property(button, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_primary);
    ui_object_set_themeable_style_property(button, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA, _ui_theme_alpha_primary);
  }
}

void style_screen() {
  if (ui_Screen_HomeScreen == NULL) return;

  lv_obj_set_style_pad_all(ui_Screen_HomeScreen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(ui_Screen_HomeScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_background);
  ui_object_set_themeable_style_property(ui_Screen_HomeScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_background);

  lv_obj_set_width(ui_HomeScreen_Container_Container9, 296);
  lv_obj_set_height(ui_HomeScreen_Container_Container9, kContentHeightDefault);
  lv_obj_set_align(ui_HomeScreen_Container_Container9, LV_ALIGN_TOP_MID);
  lv_obj_set_x(ui_HomeScreen_Container_Container9, 0);
  lv_obj_set_y(ui_HomeScreen_Container_Container9, 8);
  lv_obj_set_style_pad_left(ui_HomeScreen_Container_Container9, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(ui_HomeScreen_Container_Container9, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(ui_HomeScreen_Container_Container9, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(ui_HomeScreen_Container_Container9, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_row(ui_HomeScreen_Container_Container9, 8, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_height(ui_HomeScreen_Container_WLAN_SD_Container, 10);
  lv_obj_set_style_pad_column(ui_HomeScreen_Container_WLAN_SD_Container, 6, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(ui_HomeScreen_Container_WLAN_SD_Container, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_width(ui_HomeScreen_Label_WLANStateLabel, LV_SIZE_CONTENT);
  lv_obj_set_height(ui_HomeScreen_Label_WLANStateLabel, LV_SIZE_CONTENT);
  lv_obj_set_width(ui_HomeScreen_Label_SDStateLabel, LV_SIZE_CONTENT);
  lv_obj_set_height(ui_HomeScreen_Label_SDStateLabel, LV_SIZE_CONTENT);
  lv_obj_set_style_text_font(ui_HomeScreen_Label_WLANStateLabel, &lv_font_montserrat_8, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(ui_HomeScreen_Label_SDStateLabel, &lv_font_montserrat_8, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_width(ui_HomeScreen_Label_DiffLabel, lv_pct(100));
  lv_obj_set_style_text_align(ui_HomeScreen_Label_DiffLabel, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_line_space(ui_HomeScreen_Label_DiffLabel, 4, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(ui_HomeScreen_Label_DiffLabel, &lv_font_montserrat_42, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_width(ui_HomeScreen_Label_ClockLabel, 296);
  lv_label_set_long_mode(ui_HomeScreen_Label_ClockLabel, LV_LABEL_LONG_CLIP);
  lv_obj_set_align(ui_HomeScreen_Label_ClockLabel, LV_ALIGN_BOTTOM_MID);
  lv_obj_set_x(ui_HomeScreen_Label_ClockLabel, 0);
  lv_obj_set_y(ui_HomeScreen_Label_ClockLabel, -24);
  lv_obj_set_style_text_align(ui_HomeScreen_Label_ClockLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(ui_HomeScreen_Label_ClockLabel, &lv_font_montserrat_24, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_width(ui_HomeScreen_Label_DateLabel, 296);
  lv_obj_set_align(ui_HomeScreen_Label_DateLabel, LV_ALIGN_BOTTOM_MID);
  lv_obj_set_x(ui_HomeScreen_Label_DateLabel, 0);
  lv_obj_set_y(ui_HomeScreen_Label_DateLabel, 0);
  lv_obj_set_style_text_align(ui_HomeScreen_Label_DateLabel, LV_TEXT_ALIGN_CENTER, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(ui_HomeScreen_Label_DateLabel, &lv_font_montserrat_16, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_width(ui_HomeScreen_Container_Container10, 296);
  lv_obj_set_height(ui_HomeScreen_Container_Container10, 30);
  lv_obj_set_align(ui_HomeScreen_Container_Container10, LV_ALIGN_BOTTOM_MID);
  lv_obj_set_x(ui_HomeScreen_Container_Container10, 0);
  lv_obj_set_y(ui_HomeScreen_Container_Container10, -8);
  lv_obj_set_style_pad_left(ui_HomeScreen_Container_Container10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(ui_HomeScreen_Container_Container10, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_column(ui_HomeScreen_Container_Container10, 6, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_text_font(ui_HomeScreen_Label_Label4, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(ui_HomeScreen_Label_Label1, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);

  style_button(ui_HomeScreen_Button_PrayersButton, true);
  style_button(ui_HomeScreen_Button_SettingsButton, false);
}

}  // namespace

void home_screen_ui_init(lv_event_cb_t prayers_cb, lv_event_cb_t settings_cb) {
  if (initialized || ui_Screen_HomeScreen == NULL) return;

  style_screen();

  lv_obj_add_event_cb(ui_HomeScreen_Button_PrayersButton, prayers_cb, LV_EVENT_CLICKED, NULL);
  lv_obj_add_event_cb(ui_HomeScreen_Button_SettingsButton, settings_cb, LV_EVENT_CLICKED, NULL);

  initialized = true;
  home_screen_ui_reset_actions();
}

void home_screen_ui_tick() {
  // Home controls stay immediately usable; no delayed reveal.
}

void home_screen_ui_reset_actions() {
  if (ui_HomeScreen_Container_Container10 == NULL || ui_HomeScreen_Container_Container9 == NULL) return;
  lv_obj_clear_flag(ui_HomeScreen_Container_Container10, LV_OBJ_FLAG_HIDDEN);
  lv_obj_set_height(ui_HomeScreen_Container_Container9, kContentHeightDefault);
  lv_obj_invalidate(ui_Screen_HomeScreen);
}

void home_screen_ui_set_status(bool wifi_connected, bool sd_ready) {
  if (ui_HomeScreen_Label_WLANStateLabel != NULL) {
    lv_obj_set_style_text_color(
        ui_HomeScreen_Label_WLANStateLabel,
        lv_color_hex(wifi_connected ? kIconActive : kIconInactive),
        LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_invalidate(ui_HomeScreen_Label_WLANStateLabel);
  }

  if (ui_HomeScreen_Label_SDStateLabel != NULL) {
    lv_obj_set_style_text_color(
        ui_HomeScreen_Label_SDStateLabel,
        lv_color_hex(sd_ready ? kIconActive : kIconInactive),
        LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_invalidate(ui_HomeScreen_Label_SDStateLabel);
  }
}

void home_screen_ui_set_clock_text(const char* text) {
  set_text(ui_HomeScreen_Label_ClockLabel, text);
}

void home_screen_ui_set_date_text(const char* text) {
  set_text(ui_HomeScreen_Label_DateLabel, text);
}

void home_screen_ui_set_next_prayer_text(const char* text) {
  set_text(ui_HomeScreen_Label_DiffLabel, text);
}
