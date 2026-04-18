#pragma once
#include <lvgl.h>

void azan_screen_init();
void azan_screen_loop();

void startAdhan();
void stopAdhan();
void handleAudioTick();