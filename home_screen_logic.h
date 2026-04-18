#pragma once
#include <lvgl.h>

void home_screen_init();
void home_screen_loop();

void go_to_prayers_screen(lv_event_t* e);
void go_to_settings_screen(lv_event_t* e);

void drawClock();
void drawDate();
void drawNextPrayer();
void updateTime();
void checkAdhanLogic();
void calculateNextPrayer();
void setIconColor(lv_obj_t* label, bool active);
void updateStatusIcons();
void handleHomeToneTick();
void playHomeTone();