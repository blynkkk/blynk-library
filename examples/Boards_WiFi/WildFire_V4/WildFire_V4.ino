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
  This example shows how to use WildFire V4
  to connect your project to Blynk.

  Note: You need to install Arduino IDE WildFire support:
    http://shop.wickeddevice.com/resources/wildfire/

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill-in your Template ID (only if using Blynk.Cloud) */
//#define BLYNK_TEMPLATE_ID   "YourTemplateID"


#include <ESP8266_Lib.h>
#include <BlynkSimpleShieldEsp8266.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";

// Digital the pin that is used to reset/enable the ESP8266 module
const int EspSwitch =  23;

ESP8266 wifi(&Serial1);

void setup()
{
  // Debug console
  Serial.begin(9600);
  delay(10);

  // Set ESP8266 baud rate
  Serial1.begin(115200);
  delay(10);

  // Reset ESP
  pinMode(EspSwitch, OUTPUT);
  digitalWrite(EspSwitch, LOW);
  delay(50);
  digitalWrite(EspSwitch, HIGH);

  // Configure Blynk connection
  Blynk.begin(auth, wifi, ssid, pass);
}

void loop()
{
  Blynk.run();
}

