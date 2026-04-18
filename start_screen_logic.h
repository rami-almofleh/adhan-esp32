#pragma once

void start_screen_init();
void start_screen_loop();
bool fetchPrayerTimes();
void updateStatus(const char* text);
long timeToSeconds(String t);
void initWIFI();
void initSD();
void initAudio();
