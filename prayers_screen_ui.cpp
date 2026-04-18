#include "prayers_screen_ui.h"

#include "ui.h"

namespace {

bool initialized = false;

void style_row(lv_obj_t* row, bool highlighted) {
  if (row == NULL) return;

  ui_object_set_themeable_style_property(row, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_secondary);
  ui_object_set_themeable_style_property(row, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_secondary);
  lv_obj_set_style_radius(row, 12, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(row, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(row, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(row, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(row, 2, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_width(row, lv_pct(88));
  lv_obj_set_height(row, 26);

  if (highlighted) {
    ui_object_set_themeable_style_property(row, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_primary);
    lv_obj_set_style_bg_opa(row, 48, LV_PART_MAIN | LV_STATE_DEFAULT);
  }
  lv_obj_set_style_border_width(row, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
}

}  // namespace

void prayers_screen_ui_init(lv_event_cb_t home_cb) {
  if (initialized || ui_Screen_PrayersScreen == NULL) return;

  lv_obj_set_style_pad_all(ui_Screen_PrayersScreen, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(ui_Screen_PrayersScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_background);
  ui_object_set_themeable_style_property(ui_Screen_PrayersScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_background);
  lv_obj_set_width(ui_PrayersScreen_Container_Container1, 272);
  lv_obj_set_height(ui_PrayersScreen_Container_Container1, 182);
  lv_obj_set_align(ui_PrayersScreen_Container_Container1, LV_ALIGN_TOP_MID);
  lv_obj_set_x(ui_PrayersScreen_Container_Container1, 0);
  lv_obj_set_y(ui_PrayersScreen_Container_Container1, 10);
  lv_obj_set_style_pad_left(ui_PrayersScreen_Container_Container1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(ui_PrayersScreen_Container_Container1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(ui_PrayersScreen_Container_Container1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(ui_PrayersScreen_Container_Container1, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_row(ui_PrayersScreen_Container_Container1, 6, LV_PART_MAIN | LV_STATE_DEFAULT);

  style_row(ui_PrayersScreen_Container_Container4, false);
  style_row(ui_PrayersScreen_Container_Container5, false);
  style_row(ui_PrayersScreen_Container_Container6, true);
  style_row(ui_PrayersScreen_Container_Container7, false);
  style_row(ui_PrayersScreen_Container_Container8, false);

  lv_obj_set_width(ui_PrayersScreen_Container_Container12, 272);
  lv_obj_set_height(ui_PrayersScreen_Container_Container12, 40);
  lv_obj_set_align(ui_PrayersScreen_Container_Container12, LV_ALIGN_BOTTOM_MID);
  lv_obj_set_x(ui_PrayersScreen_Container_Container12, 0);
  lv_obj_set_y(ui_PrayersScreen_Container_Container12, -8);
  lv_obj_set_style_pad_left(ui_PrayersScreen_Container_Container12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(ui_PrayersScreen_Container_Container12, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_width(ui_PrayersScreen_Button_HomeButton, 96);
  lv_obj_set_height(ui_PrayersScreen_Button_HomeButton, 30);
  lv_obj_set_style_radius(ui_PrayersScreen_Button_HomeButton, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
  ui_object_set_themeable_style_property(ui_PrayersScreen_Button_HomeButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_primary);
  ui_object_set_themeable_style_property(ui_PrayersScreen_Button_HomeButton, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_primary);
  lv_obj_set_style_border_width(ui_PrayersScreen_Button_HomeButton, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(ui_PrayersScreen_Button_HomeButton, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_label_set_text(ui_PrayersScreen_Label_Label2, "Home");
  lv_obj_set_style_text_color(ui_PrayersScreen_Label_Label2, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_font(ui_PrayersScreen_Label_Label2, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_clear_flag(ui_PrayersScreen_Label_Label2, LV_OBJ_FLAG_HIDDEN);

  lv_obj_add_event_cb(ui_PrayersScreen_Button_HomeButton, home_cb, LV_EVENT_CLICKED, NULL);
  initialized = true;
}

void prayers_screen_ui_refresh(const Prayer* prayers, int highlighted_index) {
  for (int i = 0; i < 5; i++) {
    lv_obj_t* row = NULL;
    lv_obj_t* nameLabel = NULL;
    lv_obj_t* timeLabel = NULL;

    switch (i) {
      case 0:
        row = ui_PrayersScreen_Container_Container4;
        nameLabel = ui_PrayersScreen_Label_PrayerLabel1;
        timeLabel = ui_PrayersScreen_Label_ClockLabel6;
        break;
      case 1:
        row = ui_PrayersScreen_Container_Container5;
        nameLabel = ui_PrayersScreen_Label_PrayerLabel2;
        timeLabel = ui_PrayersScreen_Label_ClockLabel3;
        break;
      case 2:
        row = ui_PrayersScreen_Container_Container6;
        nameLabel = ui_PrayersScreen_Label_PrayerLabel3;
        timeLabel = ui_PrayersScreen_Label_ClockLabel5;
        break;
      case 3:
        row = ui_PrayersScreen_Container_Container7;
        nameLabel = ui_PrayersScreen_Label_PrayerLabel4;
        timeLabel = ui_PrayersScreen_Label_ClockLabel8;
        break;
      default:
        row = ui_PrayersScreen_Container_Container8;
        nameLabel = ui_PrayersScreen_Label_PrayerLabel5;
        timeLabel = ui_PrayersScreen_Label_ClockLabel10;
        break;
    }

    lv_label_set_text(nameLabel, prayers[i].name.c_str());
    lv_label_set_text(timeLabel, prayers[i].time.c_str());

    bool highlighted = i == highlighted_index;
    style_row(row, highlighted);
    lv_obj_set_style_text_font(nameLabel, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_font(timeLabel, &lv_font_montserrat_18, LV_PART_MAIN | LV_STATE_DEFAULT);

    ui_object_set_themeable_style_property(
        nameLabel,
        LV_PART_MAIN | LV_STATE_DEFAULT,
        LV_STYLE_TEXT_COLOR,
        highlighted ? _ui_theme_color_primary : _ui_theme_color_title);
    ui_object_set_themeable_style_property(
        timeLabel,
        LV_PART_MAIN | LV_STATE_DEFAULT,
        LV_STYLE_TEXT_COLOR,
        highlighted ? _ui_theme_color_primary : _ui_theme_color_title);
  }
}
