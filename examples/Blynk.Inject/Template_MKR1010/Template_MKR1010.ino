/*************************************************************
  This is a DEMO. You can use it only for development and testing.
  You should open Setting.h and modify General options.

  If you would like to add these features to your product,
  please contact Blynk for Businesses:

                   http://www.blynk.io/

  This example requires some additional libraries:
    https://github.com/cmaglie/FlashStorage
    https://github.com/michael71/Timer5

 *************************************************************/

#define USE_MKR1010_BOARD

#define APP_DEBUG        // Comment this out to disable debug prints

#define BLYNK_PRINT Serial

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

