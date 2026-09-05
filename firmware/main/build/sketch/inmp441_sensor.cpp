#line 1 "/home/tnd/Documents/Project_Dien_Tu_Vien_Thong/ASTHMA_AIOT/firmware/main/inmp441_sensor.cpp"
#include "inmp441_sensor.h"

#include <driver/i2s.h>
#include <math.h>

// =========================
// I2S
// =========================

#define I2S_PORT I2S_NUM_0

#define I2S_SCK 14
#define I2S_WS 15
#define I2S_SD 16

// =========================
// AUDIO
// =========================

#define SAMPLE_RATE 16000
#define BUFFER_SIZE 256

int32_t samples[BUFFER_SIZE];

float rmsValue = 0;

int32_t peakValue = 0;

// =========================
// INIT
// =========================

bool inmp441_init() {

  Serial.println();
  Serial.println("[INMP441] Initializing...");

  i2s_config_t config = {

      .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX),

      .sample_rate = SAMPLE_RATE,

      .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,

      .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,

      .communication_format = I2S_COMM_FORMAT_I2S,

      .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,

      .dma_buf_count = 8,

      .dma_buf_len = 64,

      .use_apll = false,

      .tx_desc_auto_clear = false,

      .fixed_mclk = 0

  };

  i2s_pin_config_t pin_config = {

      .bck_io_num = I2S_SCK,

      .ws_io_num = I2S_WS,

      .data_out_num = I2S_PIN_NO_CHANGE,

      .data_in_num = I2S_SD

  };

  esp_err_t result;

  result = i2s_driver_install(I2S_PORT, &config, 0, NULL);

  if (result != ESP_OK) {

    Serial.println("[INMP441] Driver install failed!");

    return false;
  }

  result = i2s_set_pin(I2S_PORT, &pin_config);

  if (result != ESP_OK) {

    Serial.println("[INMP441] Pin configuration failed!");

    return false;
  }

  i2s_zero_dma_buffer(I2S_PORT);

  Serial.println("[INMP441] OK");
  Serial.println("[INMP441] Sample rate: 16 kHz");

  return true;
}

// =========================
// UPDATE
// =========================

void inmp441_update() {

  size_t bytesRead = 0;

  esp_err_t result =
      i2s_read(I2S_PORT, samples, sizeof(samples), &bytesRead, 0);

  if (result != ESP_OK || bytesRead == 0) {

    return;
  }

  int sampleCount = bytesRead / sizeof(int32_t);

  long double sum = 0;

  int32_t peak = 0;

  for (int i = 0; i < sampleCount; i++) {

    int32_t sample = samples[i] >> 14;

    int32_t absolute = abs(sample);

    if (absolute > peak) {
      peak = absolute;
    }

    sum += (long double)sample * sample;
  }

  if (sampleCount > 0) {

    rmsValue = sqrt((double)sum / sampleCount);
  }

  peakValue = peak;
}

// =========================
// GETTERS
// =========================

float inmp441_getRMS() { return rmsValue; }

int32_t inmp441_getPeak() { return peakValue; }
