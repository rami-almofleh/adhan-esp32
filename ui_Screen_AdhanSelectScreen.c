#include "ui.h"

lv_obj_t *ui_Screen_AdhanSelectScreen = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Title = NULL;
lv_obj_t *ui_AdhanSelectScreen_Container_List = NULL;
lv_obj_t *ui_AdhanSelectScreen_Container_Row1 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Container_Row2 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Container_Row3 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Container_Row4 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Container_Row5 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Container_Row6 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Container_Row7 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Container_Row8 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Container_Row9 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Container_Row10 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Checkbox_Row1 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Checkbox_Row2 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Checkbox_Row3 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Checkbox_Row4 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Checkbox_Row5 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Checkbox_Row6 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Checkbox_Row7 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Checkbox_Row8 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Checkbox_Row9 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Checkbox_Row10 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Row1 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Row2 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Row3 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Row4 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Row5 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Row6 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Row7 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Row8 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Row9 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Row10 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Button_Row1 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Button_Row2 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Button_Row3 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Button_Row4 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Button_Row5 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Button_Row6 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Button_Row7 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Button_Row8 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Button_Row9 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Button_Row10 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Button_Stop1 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Button_Stop2 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Button_Stop3 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Button_Stop4 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Button_Stop5 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Button_Stop6 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Button_Stop7 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Button_Stop8 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Button_Stop9 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Button_Stop10 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Play1 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Play2 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Play3 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Play4 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Play5 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Play6 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Play7 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Play8 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Play9 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Play10 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Stop1 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Stop2 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Stop3 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Stop4 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Stop5 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Stop6 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Stop7 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Stop8 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Stop9 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Stop10 = NULL;
lv_obj_t *ui_AdhanSelectScreen_Button_Back = NULL;
lv_obj_t *ui_AdhanSelectScreen_Label_Back = NULL;

static void create_row(
    lv_obj_t *parent,
    lv_obj_t **row,
    lv_obj_t **checkbox,
    lv_obj_t **label,
    lv_obj_t **play_button,
    lv_obj_t **play_label,
    lv_obj_t **stop_button,
    lv_obj_t **stop_label)
{
    *row = lv_obj_create(parent);
    lv_obj_remove_style_all(*row);
    lv_obj_set_width(*row, lv_pct(100));
    lv_obj_set_height(*row, 36);
    lv_obj_set_flex_flow(*row, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(*row, LV_FLEX_ALIGN_SPACE_BETWEEN, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_clear_flag(*row, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_style_pad_column(*row, 4, 0);
    lv_obj_set_style_pad_left(*row, 2, 0);
    lv_obj_set_style_pad_right(*row, 2, 0);

    *checkbox = lv_checkbox_create(*row);
    lv_checkbox_set_text(*checkbox, "");

    *label = lv_label_create(*row);
    lv_label_set_text(*label, "Datei");
    lv_obj_set_width(*label, 104);

    *stop_button = lv_btn_create(*row);
    lv_obj_set_width(*stop_button, 52);
    lv_obj_set_height(*stop_button, 28);
    lv_obj_clear_flag(*stop_button, LV_OBJ_FLAG_SCROLLABLE);

    *stop_label = lv_label_create(*stop_button);
    lv_label_set_text(*stop_label, "Stop");
    lv_obj_center(*stop_label);

    *play_button = lv_btn_create(*row);
    lv_obj_set_width(*play_button, 82);
    lv_obj_set_height(*play_button, 28);
    lv_obj_clear_flag(*play_button, LV_OBJ_FLAG_SCROLLABLE);

    *play_label = lv_label_create(*play_button);
    lv_label_set_text(*play_label, "Abspielen");
    lv_obj_center(*play_label);
}

void ui_Screen_AdhanSelectScreen_screen_init(void)
{
    ui_Screen_AdhanSelectScreen = lv_obj_create(NULL);
    lv_obj_clear_flag(ui_Screen_AdhanSelectScreen, LV_OBJ_FLAG_SCROLLABLE);
    ui_object_set_themeable_style_property(ui_Screen_AdhanSelectScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_COLOR, _ui_theme_color_background);
    ui_object_set_themeable_style_property(ui_Screen_AdhanSelectScreen, LV_PART_MAIN | LV_STATE_DEFAULT, LV_STYLE_BG_OPA, _ui_theme_alpha_background);

    ui_AdhanSelectScreen_Label_Title = lv_label_create(ui_Screen_AdhanSelectScreen);
    lv_obj_set_align(ui_AdhanSelectScreen_Label_Title, LV_ALIGN_TOP_MID);
    lv_obj_set_y(ui_AdhanSelectScreen_Label_Title, 10);
    lv_label_set_text(ui_AdhanSelectScreen_Label_Title, "Adhan Ton");

    ui_AdhanSelectScreen_Container_List = lv_obj_create(ui_Screen_AdhanSelectScreen);
    lv_obj_remove_style_all(ui_AdhanSelectScreen_Container_List);
    lv_obj_set_width(ui_AdhanSelectScreen_Container_List, 304);
    lv_obj_set_height(ui_AdhanSelectScreen_Container_List, 170);
    lv_obj_set_align(ui_AdhanSelectScreen_Container_List, LV_ALIGN_TOP_MID);
    lv_obj_set_y(ui_AdhanSelectScreen_Container_List, 38);
    lv_obj_set_flex_flow(ui_AdhanSelectScreen_Container_List, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(ui_AdhanSelectScreen_Container_List, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_START);
    lv_obj_add_flag(ui_AdhanSelectScreen_Container_List, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scroll_dir(ui_AdhanSelectScreen_Container_List, LV_DIR_VER);
    lv_obj_set_style_pad_row(ui_AdhanSelectScreen_Container_List, 4, 0);
    lv_obj_set_style_pad_left(ui_AdhanSelectScreen_Container_List, 2, 0);
    lv_obj_set_style_pad_right(ui_AdhanSelectScreen_Container_List, 2, 0);
    lv_obj_set_style_pad_top(ui_AdhanSelectScreen_Container_List, 4, 0);
    lv_obj_set_style_pad_bottom(ui_AdhanSelectScreen_Container_List, 4, 0);

    create_row(ui_AdhanSelectScreen_Container_List, &ui_AdhanSelectScreen_Container_Row1, &ui_AdhanSelectScreen_Checkbox_Row1, &ui_AdhanSelectScreen_Label_Row1, &ui_AdhanSelectScreen_Button_Row1, &ui_AdhanSelectScreen_Label_Play1, &ui_AdhanSelectScreen_Button_Stop1, &ui_AdhanSelectScreen_Label_Stop1);
    create_row(ui_AdhanSelectScreen_Container_List, &ui_AdhanSelectScreen_Container_Row2, &ui_AdhanSelectScreen_Checkbox_Row2, &ui_AdhanSelectScreen_Label_Row2, &ui_AdhanSelectScreen_Button_Row2, &ui_AdhanSelectScreen_Label_Play2, &ui_AdhanSelectScreen_Button_Stop2, &ui_AdhanSelectScreen_Label_Stop2);
    create_row(ui_AdhanSelectScreen_Container_List, &ui_AdhanSelectScreen_Container_Row3, &ui_AdhanSelectScreen_Checkbox_Row3, &ui_AdhanSelectScreen_Label_Row3, &ui_AdhanSelectScreen_Button_Row3, &ui_AdhanSelectScreen_Label_Play3, &ui_AdhanSelectScreen_Button_Stop3, &ui_AdhanSelectScreen_Label_Stop3);
    create_row(ui_AdhanSelectScreen_Container_List, &ui_AdhanSelectScreen_Container_Row4, &ui_AdhanSelectScreen_Checkbox_Row4, &ui_AdhanSelectScreen_Label_Row4, &ui_AdhanSelectScreen_Button_Row4, &ui_AdhanSelectScreen_Label_Play4, &ui_AdhanSelectScreen_Button_Stop4, &ui_AdhanSelectScreen_Label_Stop4);
    create_row(ui_AdhanSelectScreen_Container_List, &ui_AdhanSelectScreen_Container_Row5, &ui_AdhanSelectScreen_Checkbox_Row5, &ui_AdhanSelectScreen_Label_Row5, &ui_AdhanSelectScreen_Button_Row5, &ui_AdhanSelectScreen_Label_Play5, &ui_AdhanSelectScreen_Button_Stop5, &ui_AdhanSelectScreen_Label_Stop5);
    create_row(ui_AdhanSelectScreen_Container_List, &ui_AdhanSelectScreen_Container_Row6, &ui_AdhanSelectScreen_Checkbox_Row6, &ui_AdhanSelectScreen_Label_Row6, &ui_AdhanSelectScreen_Button_Row6, &ui_AdhanSelectScreen_Label_Play6, &ui_AdhanSelectScreen_Button_Stop6, &ui_AdhanSelectScreen_Label_Stop6);
    create_row(ui_AdhanSelectScreen_Container_List, &ui_AdhanSelectScreen_Container_Row7, &ui_AdhanSelectScreen_Checkbox_Row7, &ui_AdhanSelectScreen_Label_Row7, &ui_AdhanSelectScreen_Button_Row7, &ui_AdhanSelectScreen_Label_Play7, &ui_AdhanSelectScreen_Button_Stop7, &ui_AdhanSelectScreen_Label_Stop7);
    create_row(ui_AdhanSelectScreen_Container_List, &ui_AdhanSelectScreen_Container_Row8, &ui_AdhanSelectScreen_Checkbox_Row8, &ui_AdhanSelectScreen_Label_Row8, &ui_AdhanSelectScreen_Button_Row8, &ui_AdhanSelectScreen_Label_Play8, &ui_AdhanSelectScreen_Button_Stop8, &ui_AdhanSelectScreen_Label_Stop8);
    create_row(ui_AdhanSelectScreen_Container_List, &ui_AdhanSelectScreen_Container_Row9, &ui_AdhanSelectScreen_Checkbox_Row9, &ui_AdhanSelectScreen_Label_Row9, &ui_AdhanSelectScreen_Button_Row9, &ui_AdhanSelectScreen_Label_Play9, &ui_AdhanSelectScreen_Button_Stop9, &ui_AdhanSelectScreen_Label_Stop9);
    create_row(ui_AdhanSelectScreen_Container_List, &ui_AdhanSelectScreen_Container_Row10, &ui_AdhanSelectScreen_Checkbox_Row10, &ui_AdhanSelectScreen_Label_Row10, &ui_AdhanSelectScreen_Button_Row10, &ui_AdhanSelectScreen_Label_Play10, &ui_AdhanSelectScreen_Button_Stop10, &ui_AdhanSelectScreen_Label_Stop10);

    ui_AdhanSelectScreen_Button_Back = lv_btn_create(ui_Screen_AdhanSelectScreen);
    lv_obj_set_width(ui_AdhanSelectScreen_Button_Back, 108);
    lv_obj_set_height(ui_AdhanSelectScreen_Button_Back, 32);
    lv_obj_set_align(ui_AdhanSelectScreen_Button_Back, LV_ALIGN_BOTTOM_MID);
    lv_obj_set_y(ui_AdhanSelectScreen_Button_Back, -8);
    lv_obj_clear_flag(ui_AdhanSelectScreen_Button_Back, LV_OBJ_FLAG_SCROLLABLE);
    ui_AdhanSelectScreen_Label_Back = lv_label_create(ui_AdhanSelectScreen_Button_Back);
    lv_label_set_text(ui_AdhanSelectScreen_Label_Back, "Zurueck");
    lv_obj_center(ui_AdhanSelectScreen_Label_Back);
}

void ui_Screen_AdhanSelectScreen_screen_destroy(void)
{
    if (ui_Screen_AdhanSelectScreen) lv_obj_del(ui_Screen_AdhanSelectScreen);

    ui_Screen_AdhanSelectScreen = NULL;
    ui_AdhanSelectScreen_Label_Title = NULL;
    ui_AdhanSelectScreen_Container_List = NULL;
    ui_AdhanSelectScreen_Container_Row1 = NULL;
    ui_AdhanSelectScreen_Container_Row2 = NULL;
    ui_AdhanSelectScreen_Container_Row3 = NULL;
    ui_AdhanSelectScreen_Container_Row4 = NULL;
    ui_AdhanSelectScreen_Container_Row5 = NULL;
    ui_AdhanSelectScreen_Container_Row6 = NULL;
    ui_AdhanSelectScreen_Container_Row7 = NULL;
    ui_AdhanSelectScreen_Container_Row8 = NULL;
    ui_AdhanSelectScreen_Container_Row9 = NULL;
    ui_AdhanSelectScreen_Container_Row10 = NULL;
    ui_AdhanSelectScreen_Checkbox_Row1 = NULL;
    ui_AdhanSelectScreen_Checkbox_Row2 = NULL;
    ui_AdhanSelectScreen_Checkbox_Row3 = NULL;
    ui_AdhanSelectScreen_Checkbox_Row4 = NULL;
    ui_AdhanSelectScreen_Checkbox_Row5 = NULL;
    ui_AdhanSelectScreen_Checkbox_Row6 = NULL;
    ui_AdhanSelectScreen_Checkbox_Row7 = NULL;
    ui_AdhanSelectScreen_Checkbox_Row8 = NULL;
    ui_AdhanSelectScreen_Checkbox_Row9 = NULL;
    ui_AdhanSelectScreen_Checkbox_Row10 = NULL;
    ui_AdhanSelectScreen_Label_Row1 = NULL;
    ui_AdhanSelectScreen_Label_Row2 = NULL;
    ui_AdhanSelectScreen_Label_Row3 = NULL;
    ui_AdhanSelectScreen_Label_Row4 = NULL;
    ui_AdhanSelectScreen_Label_Row5 = NULL;
    ui_AdhanSelectScreen_Label_Row6 = NULL;
    ui_AdhanSelectScreen_Label_Row7 = NULL;
    ui_AdhanSelectScreen_Label_Row8 = NULL;
    ui_AdhanSelectScreen_Label_Row9 = NULL;
    ui_AdhanSelectScreen_Label_Row10 = NULL;
    ui_AdhanSelectScreen_Button_Row1 = NULL;
    ui_AdhanSelectScreen_Button_Row2 = NULL;
    ui_AdhanSelectScreen_Button_Row3 = NULL;
    ui_AdhanSelectScreen_Button_Row4 = NULL;
    ui_AdhanSelectScreen_Button_Row5 = NULL;
    ui_AdhanSelectScreen_Button_Row6 = NULL;
    ui_AdhanSelectScreen_Button_Row7 = NULL;
    ui_AdhanSelectScreen_Button_Row8 = NULL;
    ui_AdhanSelectScreen_Button_Row9 = NULL;
    ui_AdhanSelectScreen_Button_Row10 = NULL;
    ui_AdhanSelectScreen_Button_Stop1 = NULL;
    ui_AdhanSelectScreen_Button_Stop2 = NULL;
    ui_AdhanSelectScreen_Button_Stop3 = NULL;
    ui_AdhanSelectScreen_Button_Stop4 = NULL;
    ui_AdhanSelectScreen_Button_Stop5 = NULL;
    ui_AdhanSelectScreen_Button_Stop6 = NULL;
    ui_AdhanSelectScreen_Button_Stop7 = NULL;
    ui_AdhanSelectScreen_Button_Stop8 = NULL;
    ui_AdhanSelectScreen_Button_Stop9 = NULL;
    ui_AdhanSelectScreen_Button_Stop10 = NULL;
    ui_AdhanSelectScreen_Label_Play1 = NULL;
    ui_AdhanSelectScreen_Label_Play2 = NULL;
    ui_AdhanSelectScreen_Label_Play3 = NULL;
    ui_AdhanSelectScreen_Label_Play4 = NULL;
    ui_AdhanSelectScreen_Label_Play5 = NULL;
    ui_AdhanSelectScreen_Label_Play6 = NULL;
    ui_AdhanSelectScreen_Label_Play7 = NULL;
    ui_AdhanSelectScreen_Label_Play8 = NULL;
    ui_AdhanSelectScreen_Label_Play9 = NULL;
    ui_AdhanSelectScreen_Label_Play10 = NULL;
    ui_AdhanSelectScreen_Label_Stop1 = NULL;
    ui_AdhanSelectScreen_Label_Stop2 = NULL;
    ui_AdhanSelectScreen_Label_Stop3 = NULL;
    ui_AdhanSelectScreen_Label_Stop4 = NULL;
    ui_AdhanSelectScreen_Label_Stop5 = NULL;
    ui_AdhanSelectScreen_Label_Stop6 = NULL;
    ui_AdhanSelectScreen_Label_Stop7 = NULL;
    ui_AdhanSelectScreen_Label_Stop8 = NULL;
    ui_AdhanSelectScreen_Label_Stop9 = NULL;
    ui_AdhanSelectScreen_Label_Stop10 = NULL;
    ui_AdhanSelectScreen_Button_Back = NULL;
    ui_AdhanSelectScreen_Label_Back = NULL;
}
