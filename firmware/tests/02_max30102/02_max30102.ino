#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "MAX30105.h"
#include "heartRate.h"

// =========================
// OLED
// =========================

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_SDA 8
#define OLED_SCL 9
#define OLED_ADDR 0x3C

Adafruit_SSD1306 display(
  SCREEN_WIDTH,
  SCREEN_HEIGHT,
  &Wire,
  -1
);

// =========================
// MAX30102
// =========================

MAX30105 particleSensor;

#define FINGER_THRESHOLD 50000

// Heart rate
const byte RATE_SIZE = 4;

byte rates[RATE_SIZE];
byte rateSpot = 0;

long lastBeat = 0;

float beatsPerMinute;
int beatAvg;

// =========================
// SETUP
// =========================

void setup() {

  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("============================");
  Serial.println("MAX30102 + OLED TEST");
  Serial.println("============================");

  // =========================
  // I2C
  // =========================

  Wire.begin(OLED_SDA, OLED_SCL);

  // =========================
  // OLED
  // =========================

  if (!display.begin(
        SSD1306_SWITCHCAPVCC,
        OLED_ADDR
      )) {

    Serial.println("OLED ERROR!");

    while (1);
  }

  Serial.println("OLED OK!");

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("MAX30102");

  display.setTextSize(1);
  display.setCursor(0, 30);
  display.println("Initializing...");

  display.display();

  // =========================
  // MAX30102
  // =========================

  if (!particleSensor.begin(
        Wire,
        I2C_SPEED_FAST
      )) {

    Serial.println("MAX30102 NOT FOUND!");

    display.clearDisplay();

    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("MAX30102 ERROR!");
    display.println();
    display.println("Sensor not found");

    display.display();

    while (1);
  }

  Serial.println("MAX30102 OK!");

  // =========================
  // SENSOR CONFIG
  // =========================

  particleSensor.setup();

  particleSensor.setPulseAmplitudeRed(0x1F);

  particleSensor.setPulseAmplitudeIR(0x1F);

  particleSensor.setPulseAmplitudeGreen(0);

  // =========================
  // READY
  // =========================

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("READY");

  display.setTextSize(1);
  display.setCursor(0, 30);
  display.println("Place finger");

  display.display();

  delay(1500);
}

// =========================
// LOOP
// =========================

void loop() {

  long irValue =
    particleSensor.getIR();

  // =========================
  // NO FINGER
  // =========================

  if (irValue < FINGER_THRESHOLD) {

    Serial.print("IR: ");
    Serial.print(irValue);

    Serial.println(" | No finger");

    display.clearDisplay();

    display.setTextColor(SSD1306_WHITE);

    display.setTextSize(2);
    display.setCursor(0, 0);
    display.println("MAX30102");

    display.setTextSize(1);

    display.setCursor(0, 28);
    display.println("Place finger");

    display.setCursor(0, 43);
    display.print("IR: ");
    display.println(irValue);

    display.display();

    delay(100);

    return;
  }

  // =========================
  // HEART BEAT DETECTION
  // =========================

  if (checkForBeat(irValue)) {

    long delta =
      millis() - lastBeat;

    lastBeat = millis();

    beatsPerMinute =
      60.0 / (delta / 1000.0);

    if (
      beatsPerMinute > 20 &&
      beatsPerMinute < 255
    ) {

      rates[rateSpot++] =
        (byte)beatsPerMinute;

      rateSpot %= RATE_SIZE;

      beatAvg = 0;

      for (
        byte x = 0;
        x < RATE_SIZE;
        x++
      ) {

        beatAvg += rates[x];
      }

      beatAvg /= RATE_SIZE;
    }
  }

  // =========================
  // SERIAL
  // =========================

  Serial.print("IR: ");
  Serial.print(irValue);

  Serial.print(" | BPM: ");
  Serial.print(beatsPerMinute);

  Serial.print(" | AVG BPM: ");
  Serial.println(beatAvg);

  // =========================
  // OLED
  // =========================

  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);

  display.setTextSize(2);
  display.setCursor(0, 0);
  display.println("HEART");

  display.setTextSize(1);

  display.setCursor(0, 28);
  display.print("BPM: ");

  if (beatAvg > 0) {
    display.println(beatAvg);
  } else {
    display.println("--");
  }

  display.setCursor(0, 42);
  display.print("IR: ");
  display.println(irValue);

  display.setCursor(0, 55);

  if (irValue >= FINGER_THRESHOLD) {
    display.print("Finger detected");
  }

  display.display();

  delay(50);
}