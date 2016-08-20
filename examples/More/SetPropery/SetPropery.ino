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
 *   Gauge widget (0...100) on V0
 *   Slider widget (0...100) on V1
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

void loop()
{
  Blynk.run();
}

#define BLYNK_GREEN     "#23C48E"
#define BLYNK_BLUE      "#04C0F8"
#define BLYNK_YELLOW    "#ED9D00"
#define BLYNK_RED       "#D3435C"
#define BLYNK_DARK_BLUE "#5F7CD8"

int gaugeValue = 0;
String gaugeColor;

BLYNK_READ(V0) {
  Blynk.virtualWrite(V0, gaugeValue);

  String newColor;
  if (gaugeValue > 80) {
    newColor = BLYNK_RED;
  } else if (gaugeValue > 50) {
    newColor = BLYNK_YELLOW;
  } else {
    newColor = BLYNK_GREEN;
  }

  // Send only if changed
  if (newColor != gaugeColor) {
    gaugeColor = newColor;
    Blynk.setProperty(V0, "color", gaugeColor);
  }
}

BLYNK_WRITE(V1) {
  gaugeValue = param.asInt();
}

