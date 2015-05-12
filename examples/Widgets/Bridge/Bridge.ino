/**************************************************************
 * Blynk is a platform with iOS and Android apps to control
 * Arduino, Raspberry Pi and the likes over the Internet.
 * You can easily build graphic interfaces for all your
 * projects by simply dragging and dropping widgets.
 *
 *   Downloads, docs, tutorials: http://www.blynk.cc
 *   Blynk community:            http://community.blynk.cc
 *   Social groups:              http://www.fb.com/blynkapp
 *                               http://twitter.com/blynk_app
 *
 * Blynk library is licensed under MIT license
 * This example code is in public domain.
 *
 **************************************************************
 * Control another device using Bridge widget!
 *
 * App dashboard setup:
 *   Bridge widget on V1
 *
 **************************************************************/

#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <SimpleTimer.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// Bridge widget on virtual pin 1
WidgetBridge bridge1(1);

// Timer for blynking
SimpleTimer timer;

void setup()
{
  Blynk.begin(auth);

  if (Blynk.connect()) {
    bridge1.setAuthToken("OtherAuthToken");
  }

  // Call blynkAnotherDevice periodically
  timer.setInterval(1000, blynkAnotherDevice);
}

static bool value = true;
void blynkAnotherDevice()
{
  // Send value to another device
  if (value) {
    bridge1.digitalWrite(9, HIGH);
  } else {
    bridge1.digitalWrite(9, LOW);
  }
  // Toggle value
  value = !value;
}

void loop()
{
  Blynk.run();
  timer.run();
}

