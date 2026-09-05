#line 1 "/home/tnd/Documents/Project_Dien_Tu_Vien_Thong/ASTHMA_AIOT/firmware/main/inmp441_sensor.h"
#ifndef INMP441_SENSOR_H
#define INMP441_SENSOR_H

#include <Arduino.h>

bool inmp441_init();
void inmp441_update();

float inmp441_getRMS();
int32_t inmp441_getPeak();

#endif
