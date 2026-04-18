#include "azan_screen_ui.h"

#include "ui.h"

namespace {

bool initialized = false;

}

void azan_screen_ui_init(lv_event_cb_t stop_cb) {
  if (initialized || ui_Screen_AzanScreen == NULL) return;

  lv_obj_set_size(ui_Screen_AzanScreen, 320, 240);
  lv_obj_add_flag(ui_Screen_AzanScreen, LV_OBJ_FLAG_CLICKABLE);
  lv_obj_add_event_cb(ui_Screen_AzanScreen, stop_cb, LV_EVENT_CLICKED, NULL);

  lv_obj_set_style_pad_left(ui_Screen_AzanScreen, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(ui_Screen_AzanScreen, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(ui_Screen_AzanScreen, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(ui_Screen_AzanScreen, 16, LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(ui_Screen_AzanScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_background);
  ui_object_set_themeable_style_property(ui_Screen_AzanScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_background);

  ui_object_set_themeable_style_property(ui_AzanScreen_Container_Container14, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_secondary);
  ui_object_set_themeable_style_property(ui_AzanScreen_Container_Container14, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_secondary);
  lv_obj_set_style_border_width(ui_AzanScreen_Container_Container14, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_radius(ui_AzanScreen_Container_Container14, 18, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_width(ui_AzanScreen_Container_Container14, lv_pct(100));
  lv_obj_set_height(ui_AzanScreen_Container_Container14, 184);
  lv_obj_set_style_pad_left(ui_AzanScreen_Container_Container14, 14, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(ui_AzanScreen_Container_Container14, 14, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(ui_AzanScreen_Container_Container14, 14, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(ui_AzanScreen_Container_Container14, 14, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_row(ui_AzanScreen_Container_Container14, 12, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_text_font(ui_AzanScreen_Label_PrayerTitle, &lv_font_montserrat_42, LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(ui_AzanScreen_Label_Label8, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_title);
  ui_object_set_themeable_style_property(ui_AzanScreen_Label_Label8, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_OPA, _ui_theme_alpha_title);

  initialized = true;
}

void azan_screen_ui_set_prayer_title(const char* title) {
  if (ui_AzanScreen_Label_PrayerTitle == NULL) return;
  lv_label_set_text(ui_AzanScreen_Label_PrayerTitle, title);
  lv_obj_invalidate(ui_AzanScreen_Label_PrayerTitle);
}

void azan_screen_ui_set_hint(const char* text) {
  if (ui_AzanScreen_Label_Label8 == NULL) return;
  lv_label_set_text(ui_AzanScreen_Label_Label8, text);
  lv_obj_invalidate(ui_AzanScreen_Label_Label8);
}
