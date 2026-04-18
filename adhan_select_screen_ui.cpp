#include "adhan_select_screen_ui.h"

#include "ui.h"

namespace {

bool initialized = false;

lv_obj_t* row_at(size_t index) {
  switch (index) {
    case 0: return ui_AdhanSelectScreen_Container_Row1;
    case 1: return ui_AdhanSelectScreen_Container_Row2;
    case 2: return ui_AdhanSelectScreen_Container_Row3;
    case 3: return ui_AdhanSelectScreen_Container_Row4;
    case 4: return ui_AdhanSelectScreen_Container_Row5;
    case 5: return ui_AdhanSelectScreen_Container_Row6;
    case 6: return ui_AdhanSelectScreen_Container_Row7;
    case 7: return ui_AdhanSelectScreen_Container_Row8;
    case 8: return ui_AdhanSelectScreen_Container_Row9;
    default: return ui_AdhanSelectScreen_Container_Row10;
  }
}

lv_obj_t* checkbox_at(size_t index) {
  switch (index) {
    case 0: return ui_AdhanSelectScreen_Checkbox_Row1;
    case 1: return ui_AdhanSelectScreen_Checkbox_Row2;
    case 2: return ui_AdhanSelectScreen_Checkbox_Row3;
    case 3: return ui_AdhanSelectScreen_Checkbox_Row4;
    case 4: return ui_AdhanSelectScreen_Checkbox_Row5;
    case 5: return ui_AdhanSelectScreen_Checkbox_Row6;
    case 6: return ui_AdhanSelectScreen_Checkbox_Row7;
    case 7: return ui_AdhanSelectScreen_Checkbox_Row8;
    case 8: return ui_AdhanSelectScreen_Checkbox_Row9;
    default: return ui_AdhanSelectScreen_Checkbox_Row10;
  }
}

lv_obj_t* label_at(size_t index) {
  switch (index) {
    case 0: return ui_AdhanSelectScreen_Label_Row1;
    case 1: return ui_AdhanSelectScreen_Label_Row2;
    case 2: return ui_AdhanSelectScreen_Label_Row3;
    case 3: return ui_AdhanSelectScreen_Label_Row4;
    case 4: return ui_AdhanSelectScreen_Label_Row5;
    case 5: return ui_AdhanSelectScreen_Label_Row6;
    case 6: return ui_AdhanSelectScreen_Label_Row7;
    case 7: return ui_AdhanSelectScreen_Label_Row8;
    case 8: return ui_AdhanSelectScreen_Label_Row9;
    default: return ui_AdhanSelectScreen_Label_Row10;
  }
}

lv_obj_t* button_at(size_t index) {
  switch (index) {
    case 0: return ui_AdhanSelectScreen_Button_Row1;
    case 1: return ui_AdhanSelectScreen_Button_Row2;
    case 2: return ui_AdhanSelectScreen_Button_Row3;
    case 3: return ui_AdhanSelectScreen_Button_Row4;
    case 4: return ui_AdhanSelectScreen_Button_Row5;
    case 5: return ui_AdhanSelectScreen_Button_Row6;
    case 6: return ui_AdhanSelectScreen_Button_Row7;
    case 7: return ui_AdhanSelectScreen_Button_Row8;
    case 8: return ui_AdhanSelectScreen_Button_Row9;
    default: return ui_AdhanSelectScreen_Button_Row10;
  }
}

lv_obj_t* play_label_at(size_t index) {
  switch (index) {
    case 0: return ui_AdhanSelectScreen_Label_Play1;
    case 1: return ui_AdhanSelectScreen_Label_Play2;
    case 2: return ui_AdhanSelectScreen_Label_Play3;
    case 3: return ui_AdhanSelectScreen_Label_Play4;
    case 4: return ui_AdhanSelectScreen_Label_Play5;
    case 5: return ui_AdhanSelectScreen_Label_Play6;
    case 6: return ui_AdhanSelectScreen_Label_Play7;
    case 7: return ui_AdhanSelectScreen_Label_Play8;
    case 8: return ui_AdhanSelectScreen_Label_Play9;
    default: return ui_AdhanSelectScreen_Label_Play10;
  }
}

void style_button(lv_obj_t* button, bool filled) {
  if (button == NULL) return;
  lv_obj_set_width(button, 60);
  lv_obj_set_height(button, 28);
  lv_obj_set_style_radius(button, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(button, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  if (filled) {
    ui_object_set_themeable_style_property(button, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_primary);
    ui_object_set_themeable_style_property(button, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_primary);
    lv_obj_set_style_text_color(button, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  } else {
    ui_object_set_themeable_style_property(button, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_secondary);
    ui_object_set_themeable_style_property(button, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_secondary);
    ui_object_set_themeable_style_property(button, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_title);
  }
}

void style_screen() {
  ui_object_set_themeable_style_property(ui_Screen_AdhanSelectScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_background);
  ui_object_set_themeable_style_property(ui_Screen_AdhanSelectScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_background);
  ui_object_set_themeable_style_property(ui_AdhanSelectScreen_Label_Title, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_primary);
  lv_obj_set_style_text_font(ui_AdhanSelectScreen_Label_Title, &lv_font_montserrat_20, LV_PART_MAIN | LV_STATE_DEFAULT);

  ui_object_set_themeable_style_property(ui_AdhanSelectScreen_Container_List, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_background);
  ui_object_set_themeable_style_property(ui_AdhanSelectScreen_Container_List, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_background);
  lv_obj_set_style_border_width(ui_AdhanSelectScreen_Container_List, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_scrollbar_mode(ui_AdhanSelectScreen_Container_List, LV_SCROLLBAR_MODE_OFF);
  lv_obj_set_style_pad_row(ui_AdhanSelectScreen_Container_List, 4, LV_PART_MAIN | LV_STATE_DEFAULT);

  for (size_t i = 0; i < 10; i++) {
    lv_obj_t* row = row_at(i);
    lv_obj_t* checkbox = checkbox_at(i);
    lv_obj_t* label = label_at(i);
    lv_obj_t* button = button_at(i);
    if (row == NULL || checkbox == NULL || label == NULL || button == NULL) continue;

    ui_object_set_themeable_style_property(row, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_secondary);
    ui_object_set_themeable_style_property(row, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_secondary);
    lv_obj_set_style_radius(row, 10, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_border_width(row, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_left(row, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_right(row, 8, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_height(row, 32);

    ui_object_set_themeable_style_property(label, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_title);
    lv_obj_set_style_text_font(label, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_width(label, 160);
    lv_label_set_long_mode(label, LV_LABEL_LONG_DOT);

    lv_obj_set_style_border_width(checkbox, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_pad_column(checkbox, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(checkbox, LV_RADIUS_CIRCLE, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_radius(checkbox, LV_RADIUS_CIRCLE, LV_PART_INDICATOR | LV_STATE_CHECKED);
    lv_obj_set_style_width(checkbox, 18, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    lv_obj_set_style_height(checkbox, 18, LV_PART_INDICATOR | LV_STATE_DEFAULT);
    ui_object_set_themeable_style_property(checkbox, LV_PART_INDICATOR | LV_STATE_CHECKED, LV_STYLE_BG_COLOR, _ui_theme_color_primary);

    style_button(button, true);
    lv_obj_set_style_text_font(play_label_at(i), &lv_font_montserrat_12, LV_PART_MAIN | LV_STATE_DEFAULT);
    lv_obj_set_style_text_color(play_label_at(i), lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  }

  style_button(ui_AdhanSelectScreen_Button_Back, true);
  lv_obj_set_width(ui_AdhanSelectScreen_Button_Back, 96);
  lv_obj_set_height(ui_AdhanSelectScreen_Button_Back, 30);
  lv_obj_set_style_text_font(ui_AdhanSelectScreen_Label_Back, &lv_font_montserrat_14, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_text_color(ui_AdhanSelectScreen_Label_Back, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
}

}  // namespace

void adhan_select_screen_ui_init(lv_event_cb_t select_cb, lv_event_cb_t play_cb, lv_event_cb_t back_cb) {
  if (initialized || ui_Screen_AdhanSelectScreen == NULL) return;

  style_screen();
  for (size_t i = 0; i < 10; i++) {
    lv_obj_add_event_cb(checkbox_at(i), select_cb, LV_EVENT_CLICKED, reinterpret_cast<void*>(i));
    lv_obj_add_event_cb(button_at(i), play_cb, LV_EVENT_CLICKED, reinterpret_cast<void*>(i));
  }
  lv_obj_add_event_cb(ui_AdhanSelectScreen_Button_Back, back_cb, LV_EVENT_CLICKED, NULL);
  initialized = true;
}

void adhan_select_screen_ui_refresh(size_t count, const String* labels, int selected_index, int preview_index) {
  for (size_t i = 0; i < 10; i++) {
    lv_obj_t* row = row_at(i);
    lv_obj_t* checkbox = checkbox_at(i);
    lv_obj_t* label = label_at(i);
    lv_obj_t* play_label = play_label_at(i);
    if (row == NULL || checkbox == NULL || label == NULL || play_label == NULL) continue;

    if (i < count) {
      lv_obj_clear_flag(row, LV_OBJ_FLAG_HIDDEN);
      lv_label_set_text(label, labels[i].c_str());
      if (static_cast<int>(i) == selected_index) lv_obj_add_state(checkbox, LV_STATE_CHECKED);
      else lv_obj_clear_state(checkbox, LV_STATE_CHECKED);
      lv_label_set_text(play_label, static_cast<int>(i) == preview_index ? "Stop" : "Spielen");
    } else {
      lv_obj_add_flag(row, LV_OBJ_FLAG_HIDDEN);
    }
  }
}
