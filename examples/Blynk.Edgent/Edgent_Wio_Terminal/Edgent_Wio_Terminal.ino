/*
 * Required libraries:
 *  - Seeed Arduino rpcUnified
 *  - Seeed Arduino rpcWiFi
 *  - Seeed Arduino SFUD
 *  - Seeed Arduino FS
 *  - Seeed Arduino mbedtls
 *  - Seeed Arduino FreeRTOS
 *  - ArduinoOTA
 *  - ArduinoHttpClient
 *
 * Please also update the WiFi module firmware:
 *   https://wiki.seeedstudio.com/Wio-Terminal-Network-Overview
 */

// Fill-in information from your Blynk Template here
//#define BLYNK_TEMPLATE_ID           "TMPxxxxxxx"
//#define BLYNK_DEVICE_NAME           "Device"

#define BLYNK_FIRMWARE_VERSION        "0.1.0"

#define BLYNK_PRINT Serial
//#define BLYNK_DEBUG

#define APP_DEBUG

#include "BlynkEdgent.h"

void setup()
{
  Serial.begin(115200);
  delay(100);

  BlynkEdgent.begin();
}

void loop() {
  BlynkEdgent.run();
}
