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

  if (time_input.hasStartTime())
  {
    Serial.println(String("Start: ") +
                   time_input.getStartHour() + ":" +
                   time_input.getStartMinute() + ":" +
                   time_input.getStartSecond());
  }
  else if (time_input.isStartSunrise())
  {
    Serial.println("Start at sunrise");
  }
  else if (time_input.isStartSunset())
  {
    Serial.println("Start at sunset");
  }
  else
  {
    // Do nothing
  }

  // Process stop time

  if (time_input.hasStopTime())
  {
    Serial.println(String("Stop: ") +
                   time_input.getStopHour() + ":" +
                   time_input.getStopMinute() + ":" +
                   time_input.getStopSecond());
  }
  else if (time_input.isStopSunrise())
  {
    Serial.println("Stop at sunrise");
  }
  else if (time_input.isStopSunset())
  {
    Serial.println("Stop at sunset");
  }
  else
  {
    // Do nothing: no stop time was set
  }

  // Process timezone (in seconds)
  // Timezone is already added to start/stop time

  Serial.println(String("Time zone: ") + timeInput.getTZ());

  // Process weekdays (1. Mon, 2. Tue, 3. Wed, ...)

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
