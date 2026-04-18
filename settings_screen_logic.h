#pragma once
#include <lvgl.h>

void reset_app(lv_event_t* e);

void settings_screen_init();
void settings_screen_loop();

// Neue Funktionen für Lautstärkeänderung
void volume_increase(lv_event_t* e);  // Lautstärke erhöhen
void volume_decrease(lv_event_t* e);  // Lautstärke verringern
void theme_switch_event_handler(lv_event_t * e);


// settings_screen_logic.h
void update_volume_display();
void update_audio_gain();
void playTone();
void handleToneTick();
