#include "prayers_screen_logic.h"
#include "app_state.h"
#include "ui.h"

static bool initialized = false;

// Forward declaration
void updatePrayerLabels();
static void go_to_home_screen(lv_event_t* e);

void prayers_screen_init() {
  if (initialized) return;

  if (ui_PrayersScreen_Button_HomeButton != NULL) {
    lv_obj_add_event_cb(ui_PrayersScreen_Button_HomeButton, go_to_home_screen, LV_EVENT_CLICKED, NULL);
  }

  updatePrayerLabels();

  initialized = true;
}

void prayers_screen_loop() {
  if (!initialized) {
    prayers_screen_init();
  }
}

// -------------------------
// BUTTON → HOME
// -------------------------
static void go_to_home_screen(lv_event_t* e) {
  changeScreen(SCREEN_HOME);
}

// -------------------------
// GEBETSZEITEN SETZEN
// -------------------------
void updatePrayerLabels() {
  // Hole das Gebet, das als nächstes fällig ist
  for (int i = 0; i < 5; i++) {
    if (i == nextPrayerIndex) {
      // Hebe das nächste Gebet hervor
      // Beispiel: Färbe das Gebet grün oder benutze eine andere Stiländerung
      if (i == 0) { // Fajr
        ui_object_set_themeable_style_property(ui_PrayersScreen_Label_PrayerLabel1, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_primary);
        ui_object_set_themeable_style_property(ui_PrayersScreen_Label_ClockLabel6, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_primary);
      } else if (i == 1) { // Dhuhr
        ui_object_set_themeable_style_property(ui_PrayersScreen_Label_PrayerLabel2, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_primary);
        ui_object_set_themeable_style_property(ui_PrayersScreen_Label_ClockLabel3, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_primary);
      } else if (i == 2) { // Asr
        ui_object_set_themeable_style_property(ui_PrayersScreen_Label_PrayerLabel3, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_primary);
        ui_object_set_themeable_style_property(ui_PrayersScreen_Label_ClockLabel5, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_primary);
      } else if (i == 3) { // Maghrib
        ui_object_set_themeable_style_property(ui_PrayersScreen_Label_PrayerLabel4, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_primary);
        ui_object_set_themeable_style_property(ui_PrayersScreen_Label_ClockLabel8, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_primary);
      } else if (i == 4) { // Isha
        ui_object_set_themeable_style_property(ui_PrayersScreen_Label_PrayerLabel5, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_primary);
        ui_object_set_themeable_style_property(ui_PrayersScreen_Label_ClockLabel10, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_primary);
      }
    } else {
      // Andere Gebete wieder in der normalen Farbe
      if (i == 0) { // Fajr
        ui_object_set_themeable_style_property(ui_PrayersScreen_Label_PrayerLabel1, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_title);
        ui_object_set_themeable_style_property(ui_PrayersScreen_Label_ClockLabel6, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_title);
      } else if (i == 1) { // Dhuhr
        ui_object_set_themeable_style_property(ui_PrayersScreen_Label_PrayerLabel2, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_title);
        ui_object_set_themeable_style_property(ui_PrayersScreen_Label_ClockLabel3, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_title);
      } else if (i == 2) { // Asr
        ui_object_set_themeable_style_property(ui_PrayersScreen_Label_PrayerLabel3, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_title);
        ui_object_set_themeable_style_property(ui_PrayersScreen_Label_ClockLabel5, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_title);
      } else if (i == 3) { // Maghrib
        ui_object_set_themeable_style_property(ui_PrayersScreen_Label_PrayerLabel4, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_title);
        ui_object_set_themeable_style_property(ui_PrayersScreen_Label_ClockLabel8, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_title);
      } else if (i == 4) { // Isha
        ui_object_set_themeable_style_property(ui_PrayersScreen_Label_PrayerLabel5, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_title);
        ui_object_set_themeable_style_property(ui_PrayersScreen_Label_ClockLabel10, LV_PART_MAIN| LV_STATE_DEFAULT, LV_STYLE_TEXT_COLOR, _ui_theme_color_title);
      }
    }
  }

  // Update die Zeiten der Gebete wie vorher
  if (ui_PrayersScreen_Label_ClockLabel6 != NULL)
    lv_label_set_text(ui_PrayersScreen_Label_ClockLabel6, prayers[0].time.c_str()); // Fajr

  if (ui_PrayersScreen_Label_ClockLabel3 != NULL)
    lv_label_set_text(ui_PrayersScreen_Label_ClockLabel3, prayers[1].time.c_str()); // Duhur

  if (ui_PrayersScreen_Label_ClockLabel5 != NULL)
    lv_label_set_text(ui_PrayersScreen_Label_ClockLabel5, prayers[2].time.c_str()); // Asr

  if (ui_PrayersScreen_Label_ClockLabel8 != NULL)
    lv_label_set_text(ui_PrayersScreen_Label_ClockLabel8, prayers[3].time.c_str()); // Maghrib

  if (ui_PrayersScreen_Label_ClockLabel10 != NULL)
    lv_label_set_text(ui_PrayersScreen_Label_ClockLabel10, prayers[4].time.c_str()); // Isha
}