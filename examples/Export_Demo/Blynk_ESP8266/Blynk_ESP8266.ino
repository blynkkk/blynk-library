/**************************************************************
 * This is a DEMO. You can use it for only development and testing.
 * You should open Setting.h and modify General options.
 *
 * If you would like to add these features to your product,
 * please contact Blynk for Business:
 *
 *                  http://tiny.cc/BlynkB2B
 *
 **************************************************************/

#define DEBUG        // Comment this out to disable debug prints

#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include "BlynkProvisioning.h"

void setup() {
  delay(500);
  Serial.begin(115200);

  BlynkProvisioning.begin();

  // Initialize this example
  example_init();
}

void loop() {
  // This handles the network and cloud connection
  BlynkProvisioning.run();

  // Run this example periodic actions
  example_run();
}


/**************************************************************
 * Example App code
 **************************************************************/

static int sensorValue = 0;

// Get fake sensor value
BLYNK_READ(V0) {
  Blynk.virtualWrite(V0, sensorValue);
}

// Set fake sensor value using slider
BLYNK_WRITE(V1) {
  sensorValue = param.asInt();
}

BLYNK_CONNECTED() {
  Blynk.syncVirtual(V1);
}

// Notification timer
BLYNK_WRITE(V2) {
  if (param.asInt() == 1) {
    Blynk.email("Blynk Test project", "This is test email");
    Blynk.notify("Push notification test");
  }
}

static int buttonStatus = false;

BLYNK_WRITE(V3) {
  buttonStatus = param.asInt();
  digitalWrite(5, buttonStatus ? HIGH : LOW);
}

#include <SimpleTimer.h>
SimpleTimer timer;

void example_init() {
  pinMode(5, OUTPUT);

  timer.setInterval(1000L, []() {
    static int randomSensorValue = 50;
    randomSensorValue += random(-5, +5);
    randomSensorValue = constrain(randomSensorValue, 40, 90);
    Blynk.virtualWrite(V4, randomSensorValue);
  });

  timer.setInterval(50L, []() {
    static int prev_btn = 0;
    int btn = !digitalRead(0);
    if (prev_btn != btn) {
      prev_btn = btn;

      if (btn) {
        buttonStatus = !buttonStatus;

        digitalWrite(5, buttonStatus ? HIGH : LOW);
        Blynk.virtualWrite(V3, buttonStatus);
      }
    }
  });
}

void example_run() {
  timer.run();
}

