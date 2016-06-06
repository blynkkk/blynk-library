/*
 * This example shows dynamic device credentials configuration.
 * WARNING: THIS IS NOT INTENDED TO BE USED IN PRODUCTION!
 */

#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include "BlynkProvisioning.h"

void setup() {
  // This setups WiFi configuration, and cloud connection
  BlynkProvisioning.begin();
}

void loop() {
  Blynk.run();
}

