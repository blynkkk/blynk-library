/*
 * Required libraries:
 *  - Seeed Arduino rpcunified
 *  - Seeed Arduino rpcWiFi
 *  - Seeed Arduino SFUD
 *  - Seeed Arduino FS
 *  - Seeed Arduino mbedtls
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
  delay(100);
  
#ifdef APP_DEBUG 
  while (!Serial) {delay(10);}
#endif

  BlynkEdgent.begin();
}

void loop() {
  BlynkEdgent.run();
}
