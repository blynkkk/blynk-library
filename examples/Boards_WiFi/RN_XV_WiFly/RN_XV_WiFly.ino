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
  This example shows how to use WiFly RN-XV
  to connect your project to Blynk.

  For this example you need WiFlyHQ library:
    https://github.com/harlequin-tech/WiFlyHQ

  NOTE: Ensure a stable serial connection!
        Hardware serial is preferred.
        Firmware version 4.41 or later is preferred.

  Change WiFi ssid, pass, and Blynk auth token to run :)
  Feel free to apply it to any other example. It's simple!
 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
//#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
//#define BLYNK_TEMPLATE_NAME         "Device"
//#define BLYNK_AUTH_TOKEN            "YourAuthToken"


#include <WiFlyHQ.h>
#include <BlynkSimpleWiFly.h>

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";

#define WiFlySerial Serial1
// This can be a SoftwareSerial object:
//#include <SoftwareSerial.h>
//SoftwareSerial WiFlySerial(2, 3); // RX, TX

WiFly wifly;

void setup()
{
  // Debug console
  Serial.begin(9600);

  delay(10);

  WiFlySerial.begin(9600);  // Set your RN-XV baud rate
  delay(10);

  // Bind WiFly driver to the serial
  if (!wifly.begin(&WiFlySerial)) {
    BLYNK_FATAL("Failed to start wifly");
  }

  // You can try increasing baud rate:
  //wifly.setBaud(115200);
  //WiFlySerial.begin(115200);

  Blynk.begin(BLYNK_AUTH_TOKEN, wifly, ssid, pass);
}

void loop()
{
  Blynk.run();
}

