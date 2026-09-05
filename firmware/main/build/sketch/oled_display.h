#line 1 "/home/tnd/Documents/Project_Dien_Tu_Vien_Thong/ASTHMA_AIOT/firmware/main/oled_display.h"
#ifndef OLED_DISPLAY_H
#define OLED_DISPLAY_H

#include <Arduino.h>

bool oled_init();

void oled_showMessage(const char *title, const char *message);

void oled_showError(const char *message);

void oled_showReady();

void oled_update(float bpm, int averageBpm, long irValue, bool fingerDetected,

                 float rms, int32_t peak);

#endif
