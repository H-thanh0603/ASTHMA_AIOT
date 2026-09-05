#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>
#include <driver/i2s.h>

// =========================
// OLED
// =========================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_SDA 8
#define OLED_SCL 9
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// =========================
// INMP441
// =========================

#define I2S_PORT I2S_NUM_0

#define I2S_SCK 14
#define I2S_WS 15
#define I2S_SD 16

#define SAMPLE_RATE 16000
#define BUFFER_SIZE 256

int32_t samples[BUFFER_SIZE];

// =========================
// SETUP
// =========================

void setup() {

  Serial.begin(115200);

  delay(1000);

  Serial.println();
  Serial.println("============================");
  Serial.println("INMP441 + OLED TEST");
  Serial.println("============================");

  // =========================
  // I2C OLED
  // =========================

  Wire.begin(OLED_SDA, OLED_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {

    Serial.println("OLED ERROR!");

    while (1)
      ;
  }

  Serial.println("OLED OK!");

  // =========================
  // OLED START SCREEN
  // =========================

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("INMP441");

  display.setTextSize(1);
  display.setCursor(0, 28);
  display.println("Initializing...");

  display.display();

  // =========================
  // I2S CONFIG
  // =========================

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

      .fixed_mclk = 0};

  // =========================
  // I2S PIN CONFIG
  // =========================

  i2s_pin_config_t pin_config = {

      .bck_io_num = I2S_SCK,

      .ws_io_num = I2S_WS,

      .data_out_num = I2S_PIN_NO_CHANGE,

      .data_in_num = I2S_SD};

  // =========================
  // INSTALL I2S DRIVER
  // =========================

  esp_err_t result;

  result = i2s_driver_install(I2S_PORT, &config, 0, NULL);

  if (result != ESP_OK) {

    Serial.println("I2S DRIVER ERROR!");

    display.clearDisplay();

    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("INMP441 ERROR");
    display.println();
    display.println("I2S DRIVER FAIL");

    display.display();

    while (1)
      ;
  }

  result = i2s_set_pin(I2S_PORT, &pin_config);

  if (result != ESP_OK) {

    Serial.println("I2S PIN ERROR!");

    while (1)
      ;
  }

  i2s_zero_dma_buffer(I2S_PORT);

  Serial.println("INMP441 OK!");
  Serial.println("Audio sampling: 16 kHz");

  // =========================
  // READY
  // =========================

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("MIC OK");

  display.setTextSize(1);
  display.setCursor(0, 30);
  display.println("INMP441");
  display.println("16 kHz");

  display.display();

  delay(1500);
}

// =========================
// LOOP
// =========================

void loop() {

  size_t bytesRead = 0;

  // =========================
  // READ AUDIO
  // =========================

  esp_err_t result =
      i2s_read(I2S_PORT, samples, sizeof(samples), &bytesRead, portMAX_DELAY);

  if (result != ESP_OK) {

    Serial.println("I2S READ ERROR");

    return;
  }

  int sampleCount = bytesRead / sizeof(int32_t);

  // =========================
  // CALCULATE AUDIO LEVEL
  // =========================

  long long sum = 0;

  int32_t peak = 0;

  for (int i = 0; i < sampleCount; i++) {

    // INMP441 24-bit data
    int32_t sample = samples[i] >> 14;

    // Absolute value
    int32_t absolute = abs(sample);

    if (absolute > peak) {
      peak = absolute;
    }

    sum += (long long)sample * sample;
  }

  // =========================
  // RMS
  // =========================

  float rms = 0;

  if (sampleCount > 0) {

    rms = sqrt((float)sum / sampleCount);
  }

  // =========================
  // SERIAL
  // =========================

  Serial.print("RMS: ");
  Serial.print(rms);

  Serial.print(" | PEAK: ");
  Serial.println(peak);

  // =========================
  // OLED
  // =========================

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("MIC TEST");

  display.setTextSize(1);

  display.setCursor(0, 25);
  display.print("RMS : ");
  display.println(rms, 0);

  display.setCursor(0, 38);
  display.print("PEAK: ");
  display.println(peak);

  // =========================
  // SIMPLE LEVEL BAR
  // =========================

  int barWidth = map(constrain(peak, 0, 10000), 0, 10000, 0, 120);

  display.drawRect(0, 53, 128, 10, SSD1306_WHITE);

  display.fillRect(2, 55, barWidth, 6, SSD1306_WHITE);

  display.display();

  delay(100);
}