/*
 * Required libraries:
 *  - WiFi101
 *  - ArduinoOTA
 *  - ArduinoHttpClient
 *  - Timer5
 *  
 * Please also update the WiFi module firmware, if needed
 */

// Fill-in information from your Blynk Template here
//#define BLYNK_TEMPLATE_ID           "TMPLxxxxxx"
//#define BLYNK_DEVICE_NAME           "Device"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

#include "BlynkEdgent.h"

void setup()
{
  Serial.begin(115200);
  delay(2000);

  BlynkEdgent.begin();
}

void loop() {
  BlynkEdgent.run();
}
