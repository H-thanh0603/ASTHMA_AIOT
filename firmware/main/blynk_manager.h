#ifndef BLYNK_MANAGER_H
#define BLYNK_MANAGER_H

#include <Arduino.h>

// Config Blynk (khong block - ket noi dien ra trong blynk_update)
bool blynk_init();

// Goi trong loop - chay Blynk.run() va tu dong push data moi 3 giay
void blynk_update();

bool blynk_isConnected();

// Cap nhat gia tri moi nhat tu sensor - se duoc push trong blynk_update
void blynk_setHealth(int avgBpm, bool fingerDetected, float rms,
                     uint32_t audioRate);

#endif
