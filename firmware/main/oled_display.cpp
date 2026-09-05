#include "oled_display.h"

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

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
// INIT
// =========================

bool oled_init() {

  Serial.println();
  Serial.println("[OLED] Initializing...");

  Wire.begin(OLED_SDA, OLED_SCL);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {

    Serial.println("[OLED] ERROR!");

    return false;
  }

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);

  display.display();

  Serial.println("[OLED] OK");

  return true;
}

// =========================
// MESSAGE
// =========================

void oled_showMessage(const char *title, const char *message) {

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(2);

  display.setCursor(0, 0);

  display.println(title);

  display.setTextSize(1);

  display.setCursor(0, 30);

  display.println(message);

  display.display();
}

// =========================
// ERROR
// =========================

void oled_showError(const char *message) {

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(1);

  display.setCursor(0, 0);

  display.println("SYSTEM ERROR");

  display.println();

  display.println(message);

  display.display();
}

// =========================
// READY
// =========================

void oled_showReady() {

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(2);

  display.setCursor(0, 0);

  display.println("READY");

  display.setTextSize(1);

  display.setCursor(0, 28);

  display.println("MAX30102: OK");

  display.setCursor(0, 40);

  display.println("INMP441 : OK");

  display.setCursor(0, 52);

  display.println("System running");

  display.display();
}

// =========================
// UPDATE
// =========================

void oled_update(float bpm, int averageBpm, long irValue, bool fingerDetected,

                 float rms, int32_t peak) {

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);

  // =========================
  // TITLE
  // =========================

  display.setTextSize(1);

  display.setCursor(0, 0);

  display.println("ASTHMA AIoT");

  display.drawLine(0, 9, 127, 9, SSD1306_WHITE);

  // =========================
  // MAX30102
  // =========================

  display.setCursor(0, 13);

  display.print("HR: ");

  if (fingerDetected && averageBpm > 0) {

    display.print(averageBpm);

    display.println(" BPM");

  } else {

    display.println("-- BPM");
  }

  display.setCursor(0, 25);

  display.print("IR: ");

  display.println(irValue);

  // =========================
  // INMP441
  // =========================

  display.setCursor(0, 37);

  display.print("RMS : ");

  display.println(rms, 0);

  display.setCursor(0, 48);

  display.print("mic: ");

  display.println(peak);

  // =========================
  // STATUS
  // =========================

  display.setCursor(0, 59);

  if (fingerDetected) {

    display.print("FINGER");

  } else {

    display.print("NO FINGER");
  }

  display.display();
}
