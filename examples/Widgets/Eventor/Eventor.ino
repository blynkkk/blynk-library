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
 * You can use predefined rules on application side.
 *
 * Project setup in the Blynk app:
 *   Eventor widget with next rules :
 *     a) When V0 is equal to 1, set V1 to 255;
       b) When V0 is equal to 0, set V1 to 0;
 *   Led widget on V1 pin
 *
 **************************************************************/

#define BLYNK_PRINT Serial
#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>
#include <SimpleTimer.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

SimpleTimer timer;
boolean flag = true;

void sendFlagToServer() {
  if (flag) {
    Blynk.virtualWrite(V0, 1);
  } else {
    Blynk.virtualWrite(V0, 0);
  }
  flag = !flag;
}

void setup()
{
  Serial.begin(9600); // See the connection status in Serial Monitor
  Blynk.begin(auth);

  while (Blynk.connect() == false) {
    // Wait until connected
  }

  // Setup a function to be called every second
  timer.setInterval(1000L, sendFlagToServer);
}

BLYNK_WRITE(V1) {
  //here you'll get 0 or 255
  int ledValue = param.asInt();
}

void loop()
{
  Blynk.run();
  timer.run();
}
