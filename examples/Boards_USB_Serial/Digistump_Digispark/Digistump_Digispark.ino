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
 * There is an USB issue on Linux:
 *   https://github.com/digistump/DigistumpArduino/issues/16
 *
 **************************************************************
 *            USB HOWTO: http://tiny.cc/BlynkUSB
 **************************************************************/

// Minimize size for non-Pro version
#if !defined(ARDUINO_AVR_DIGISPARKPRO)
#define BLYNK_NO_INFO
#define BLYNK_NO_BUILTIN
#endif

#include <BlynkSimpleStream.h>
#include <DigiCDC.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

void setup()
{
  // Blynk will work through SerialUSB
  SerialUSB.begin();
  Blynk.begin(auth, SerialUSB);
}

void loop()
{
  Blynk.run();
}

