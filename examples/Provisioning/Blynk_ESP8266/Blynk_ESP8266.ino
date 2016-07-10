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
}

void loop() {
  // This handles the network and cloud connection
  BlynkProvisioning.run();
}

