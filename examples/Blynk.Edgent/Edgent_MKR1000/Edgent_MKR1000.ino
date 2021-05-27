
// Fill-in information from your Blynk Template here
#define BLYNK_TEMPLATE_ID "TMPLTWeMejHX"
#define BLYNK_DEVICE_NAME "Test mob"

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
