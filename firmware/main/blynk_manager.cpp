// Luu y: secrets.h PHAI duoc include truoc BlynkSimpleEsp32.h
// vi BLYNK_TEMPLATE_ID / BLYNK_AUTH_TOKEN phai dinh nghia san

#define BLYNK_PRINT Serial

#include "secrets.h"

#include <BlynkSimpleEsp32.h>

#include "blynk_manager.h"

#include "wifi_manager.h"

// Push data moi 3 giay - tranh vuot rate limit cua Blynk
#define SEND_INTERVAL_MS 3000

// Virtual pins (tao trong Blynk Console -> Datastreams)
// V0 = Heart Rate (BPM)
// V1 = SpO2 (chua lam - buoc 2 trong ke hoach)
// V2 = Audio RMS
// V3 = Audio Status (chuoi)

struct {

  int avgBpm;

  bool finger;

  float rms;

  uint32_t audioRate;

} blynkCache = {0, false, 0, 0};

static unsigned long lastSendMs = 0;

// =========================
// INIT
// =========================

bool blynk_init() {

  Serial.println("[BLYNK] Configuring...");

  Blynk.config(BLYNK_AUTH_TOKEN);

  // Khong goi Blynk.connect() o day de tranh block.
  // Blynk.run() trong blynk_update() se tu ket noi khi co WiFi.
  Serial.println("[BLYNK] OK (se ket noi trong loop)");

  return true;
}

// =========================
// SET DATA
// =========================

void blynk_setHealth(int avgBpm, bool fingerDetected, float rms,
                     uint32_t audioRate) {

  blynkCache.avgBpm = avgBpm;

  blynkCache.finger = fingerDetected;

  blynkCache.rms = rms;

  blynkCache.audioRate = audioRate;
}

// =========================
// UPDATE
// =========================

void blynk_update() {

  if (!wifi_isConnected()) {

    return;
  }

  Blynk.run();

  if (!Blynk.connected()) {

    return;
  }

  if (millis() - lastSendMs < SEND_INTERVAL_MS) {

    return;
  }

  lastSendMs = millis();

  int bpm = (blynkCache.finger && blynkCache.avgBpm > 0) ? blynkCache.avgBpm : 0;

  Blynk.virtualWrite(V0, bpm);

  Blynk.virtualWrite(V2, (int)blynkCache.rms);

  bool audioOk = (blynkCache.audioRate > 14000 && blynkCache.audioRate < 18000);

  Blynk.virtualWrite(V3, audioOk ? "OK" : "NO DATA");
}

// =========================
// GETTERS
// =========================

bool blynk_isConnected() { return Blynk.connected(); }
