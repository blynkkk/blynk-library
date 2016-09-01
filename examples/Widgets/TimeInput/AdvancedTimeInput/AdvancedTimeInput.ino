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
  TimeInputParam timeInput(param);

  // Process start time

  switch (timeInput.getStartMode())
  {
    case TimeInputParam::TIME_SPECIFIED:
      Serial.println(String("Start: ") + timeInput.getStartHour() + ":" + timeInput.getStartMinute() + ":" + timeInput.getStartSecond());
      break;
    case TimeInputParam::TIME_SUNRISE:
      Serial.println("Start at sunrize");
      break;
    case TimeInputParam::TIME_SUNSET:
      Serial.println("Start at sunset");
      break;
    case TimeInputParam::TIME_UNDEFINED:
      // Do nothing
      break;
  }


  // Process stop time

  switch (timeInput.getStopMode())
  {
    case TimeInputParam::TIME_SPECIFIED:
      Serial.println(String("Stop: ") + timeInput.getStopHour() + ":" + timeInput.getStopMinute() + ":" + timeInput.getStopSecond());
      break;
    case TimeInputParam::TIME_SUNRISE:
      Serial.println("Stop at sunrise");
      break;
    case TimeInputParam::TIME_SUNSET:
      Serial.println("Stop at sunset");
      break;
    case TimeInputParam::TIME_UNDEFINED:
      // Do nothing
      break;
  }

  // Process timezone (in seconds, it is already added to start/stop time)

  Serial.println(String("Time zone: ") + timeInput.getTZ());

  // Process weekdays

  for (int i = 1; i <= 7; i++) {
    if (timeInput.isWeekdaySelected(i)) {
      Serial.println(String("Day ") + i + " is selected");
    }
  }

  Serial.println();
}

void loop()
{
  Blynk.run();
}
