#ifndef INMP441_SENSOR_H
#define INMP441_SENSOR_H

#include <Arduino.h>

bool inmp441_init();
void inmp441_update();

float inmp441_getRMS();
int32_t inmp441_getPeak();

#endif
