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
 * App project setup:
 *   Time Input widget on V1.
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
  Serial.begin(9600);

  Blynk.begin(auth);
}

BLYNK_WRITE(V1) {
  TimeInputParam time_input(param);

  // Process start time
  if (BlynkTime& t = time_input.getStart())
  {
    Serial.println(String("Start: ") + t.hour() + ":" + t.minute() + ":" + t.second());
  }

  // Process stop time, if present
  if (BlynkTime& t = time_input.getStop())
  {
    Serial.println(String("Stop: ")  + t.hour() + ":" + t.minute() + ":" + t.second());
  }

  // Process timezone
  Serial.println(String("Time zone: ") + time_input.getTZ());

  // Process weekdays
  for (int i=1; i<=7; i++) {
    if (time_input.isWeekdaySelected(i)) {
      Serial.println(String("Day ") + i + " is selected");
    }
  }

  Serial.println();
}

void loop()
{
  Blynk.run();
}
