/*************************************************************
  Blynk is a platform with iOS and Android apps to control
  ESP32, Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build mobile and web interfaces for any
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: https://www.blynk.io
    Sketch generator:           https://examples.blynk.cc
    Blynk community:            https://community.blynk.cc
    Follow us:                  https://www.fb.com/blynkapp
                                https://twitter.com/blynk_app

  Blynk library is licensed under MIT license
  This example code is in public domain.

 *************************************************************
  This example shows how to use RedBear Duo
  to connect your project to Blynk.

  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
//#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
//#define BLYNK_TEMPLATE_NAME         "Device"
//#define BLYNK_AUTH_TOKEN            "YourAuthToken"


#include <BlynkSimpleRedBear_Duo.h>

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

  Blynk.begin(BLYNK_AUTH_TOKEN);
  // Or specify server using one of those commands:
  //Blynk.begin(BLYNK_AUTH_TOKEN, "blynk.cloud", 80);
  //Blynk.begin(BLYNK_AUTH_TOKEN, server_ip, port);
}

void loop()
{
  Blynk.run();
}

