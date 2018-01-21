/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest

  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************

  You can use predefined rules on application side.

  Project setup in the Blynk app:
    Eventor widget with next rules :
      a) When V0 is equal to 1, set V1 to 255;
      b) When V0 is equal to 0, set V1 to 0;
    Led widget on V1 pin
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

BlynkTimer timer;
boolean flag = true;

void sendFlagToServer() {
  if (flag) {
    Blynk.virtualWrite(V0, 1);
  } else {
    Blynk.virtualWrite(V0, 0);
  }
  flag = !flag;
}

BLYNK_WRITE(V1) {
  //here you'll get 0 or 255
  int ledValue = param.asInt();
}

void setup()
{
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth);

  // Setup a function to be called every second
  timer.setInterval(1000L, sendFlagToServer);
}

void loop()
{
  Blynk.run();
  timer.run();
}

