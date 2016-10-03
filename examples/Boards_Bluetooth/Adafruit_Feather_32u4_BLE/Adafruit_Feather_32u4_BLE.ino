/**************************************************************
 * Blynk is a platform with iOS and Android apps to control
 * Arduino, Raspberry Pi and the likes over the Internet.
 * You can easily build graphic interfaces for all your
 * projects by simply dragging and dropping widgets.
 *
 *   Downloads, docs, tutorials: http://www.blynk.cc
 *   Blynk community:            http://community.blynk.cc
 *   Social networks:            http://www.fb.com/blynkapp
 *                               http://twitter.com/blynk_app
 *
 * Blynk library is licensed under MIT license
 * This example code is in public domain.
 *
 **************************************************************
 *
 * This example shows how to use Adafruit Feather 32u4 BLE
 * to connect your project to Blynk.
 *
 * Note: This requires Adafruit BluefruitLE nRF51 library
 *   from http://librarymanager
 *   or https://github.com/adafruit/Adafruit_BluefruitLE_nRF51
 *
 * NOTE: BLE support is in beta!
 *
 **************************************************************/

//#define BLYNK_DEBUG
#define BLYNK_PRINT Serial

#define BLYNK_USE_DIRECT_CONNECT

#include <BlynkSimpleSerialBLE.h>
#include <SPI.h>
#include <Adafruit_BLE.h>
#include <Adafruit_BluefruitLE_SPI.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// SHARED SPI SETTINGS (see adafruit webpages for details)
#define BLUEFRUIT_SPI_CS               8
#define BLUEFRUIT_SPI_IRQ              7
#define BLUEFRUIT_SPI_RST              4    // Optional but recommended, set to -1 if unused
#define BLUEFRUIT_VERBOSE_MODE         true

// Create ble instance, see pinouts above
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

void setup() {
  Serial.begin(9600);

  Blynk.begin(auth, ble);
  ble.begin(BLUEFRUIT_VERBOSE_MODE);
  ble.factoryReset(); // Optional
  ble.setMode(BLUEFRUIT_MODE_DATA);
}

void loop() {
  Blynk.run();
}

