#line 1 "/home/tnd/Documents/Project_Dien_Tu_Vien_Thong/ASTHMA_AIOT/firmware/main/max30102_sensor.h"
#ifndef MAX30102_SENSOR_H
#define MAX30102_SENSOR_H

#include <Arduino.h>

bool max30102_init();
void max30102_update();

long max30102_getIR();

float max30102_getBPM();
int max30102_getAverageBPM();

bool max30102_hasFinger();

#endif
