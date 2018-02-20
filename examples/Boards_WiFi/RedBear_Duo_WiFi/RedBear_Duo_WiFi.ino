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
  This example shows how to use RedBear Duo
  to connect your project to Blynk.

  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <BlynkSimpleRedBear_Duo.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// Your WiFi credentials.
// Choose wifi_sec from WPA2, WPA, or WEP
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";
int wifi_sec = WPA2;

//SYSTEM_MODE(AUTOMATIC);
SYSTEM_MODE(MANUAL);

void setup()
{
  // Debug console
  Serial.begin(9600);

  delay(5000);

  WiFi.on();
  WiFi.setCredentials(ssid, pass, wifi_sec);
  WiFi.connect();

  Blynk.begin(auth);
  // Or specify server using one of those commands:
  //Blynk.begin(auth, "blynk-cloud.com", 80);
  //Blynk.begin(auth, server_ip, port);
}

void loop()
{
  Blynk.run();
}

