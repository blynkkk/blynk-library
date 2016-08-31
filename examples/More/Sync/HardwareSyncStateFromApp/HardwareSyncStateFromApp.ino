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
 * You can synchronize the state of widgets with hardware states,
 * even if hardware resets or looses connection temporarily
 *
 * Project setup in the Blynk app:
 *   Slider widget (0...100) on V0
 *   Slider widget (0...100) on V2
 *   Button widget on digital pin (connected to an LED)
 *
 **************************************************************/

#define BLYNK_PRINT Serial
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

void setup()
{
  Serial.begin(9600); // See the connection status in Serial Monitor
  Blynk.begin(auth);
}

// Keep this flag not to re-sync on every reconnection
bool isFirstConnect = true;

// This function will run every time Blynk connection is established
BLYNK_CONNECTED() {
  if (isFirstConnect) {
    // Request Blynk server to re-send latest values for all pins
    Blynk.syncAll();

    // You can also update an individual Virtual pin like this:
    //Blynk.syncVirtual(V0);

    isFirstConnect = false;
  }

  // Let's write your hardware uptime to Virtual Pin 2
  int value = millis() / 1000;
  Blynk.virtualWrite(V2, value);
}

BLYNK_WRITE(V0)
{
  int value = param.asInt();
  Blynk.virtualWrite(V2, value);
}

void loop()
{
  Blynk.run();
}

