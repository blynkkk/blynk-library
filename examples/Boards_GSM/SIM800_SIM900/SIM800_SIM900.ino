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
 * This example shows how to use SIM800 or SIM900
 * to connect your project to Blynk.
 *
 * NOTE: Ensure a stable serial connection to the SIM module!
 *       You can change the baud rate (SIM module has auto-baud by default).
 *       It is recommended to use Hard Serial.
 *       Soft Serial may be unstable.
 *
 * WARNING: SIM moudle support is for BETA testing.
 *
 * Change GPRS apm, user, pass, and Blynk auth token to run :)
 * Feel free to apply it to any other example. It's simple!
 *
 **************************************************************/

//#define BLYNK_DEBUG
#define BLYNK_PRINT Serial    // Comment this out to disable prints and save space

// Default heartbeat interval for GSM is 60
// If you want override this value, uncomment and set this option:
//#define BLYNK_HEARTBEAT 30

#include <GSM_Lib.h>
#include <BlynkSimpleSIM800.h>

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "YourAuthToken";

// Your Network credentials
// Leave empty, if missing user or pass
char apn[]  = "YourAPN";
char user[] = "";
char pass[] = "";

// Hardware Serial on Mega, Leonardo, Micro...
#define GsmSerial Serial1

// or Software Serial on Uno, Nano...
//#include <SoftwareSerial.h>
//SoftwareSerial GsmSerial(2, 3); // RX, TX

GSM gsm(&GsmSerial);

void setup()
{
  // Set console baud rate
  Serial.begin(115200);
  delay(1000);

  // Set GSM module baud rate
  GsmSerial.begin(115200);
  delay(10);

  Blynk.begin(auth, gsm, apn, user, pass);
}

void loop()
{
  Blynk.run();
}

