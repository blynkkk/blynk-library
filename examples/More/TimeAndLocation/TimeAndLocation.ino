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

  Blynk can provide your device with time and location data.
  NOTE: the accuracy of this method is up to several seconds.

 *************************************************************/

/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

/* Fill in information from Blynk Device Info here */
//#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
//#define BLYNK_TEMPLATE_NAME         "Device"
//#define BLYNK_AUTH_TOKEN            "YourAuthToken"

#include <SPI.h>
#include <Ethernet.h>
#include <BlynkSimpleEthernet.h>

BLYNK_CONNECTED() {
  // Send requests for different internal data
  // Request what is actually needed for your use-case
  Blynk.sendInternal("utc", "tz_name");   // Name of timezone
  Blynk.sendInternal("utc", "iso");       // ISO-8601 formatted time
  Blynk.sendInternal("utc", "time");      // Unix timestamp (with msecs)
  Blynk.sendInternal("utc", "tz");        // Timezone and DST offsets
  Blynk.sendInternal("utc", "tz_rule");   // POSIX TZ rule
  Blynk.sendInternal("utc", "dst_next");  // Up to 2 next time offset changes (due to DST)
}

// Receive UTC data
BLYNK_WRITE(InternalPinUTC) {
  String cmd = param[0].asStr();
  if (cmd == "time") {
    uint64_t utc_time = param[1].asLongLong();
    Serial.print("Unix time (UTC): "); Serial.println((uint32_t)(utc_time / 1000));

  } else if (cmd == "iso") {
    String iso_time = param[1].asStr();
    Serial.print("ISO-8601 time:   "); Serial.println(iso_time);

  } else if (cmd == "tz") {
    long tz_offset = param[1].asInt();
    long dst_offset = param[2].asInt();
    Serial.print("TZ offset:       "); Serial.print(tz_offset);  Serial.println(" minutes");
    Serial.print("DST offset:      "); Serial.print(dst_offset); Serial.println(" minutes");

  } else if (cmd == "tz_name") {
    String tz_name = param[1].asStr();
    Serial.print("Timezone:        "); Serial.println(tz_name);

  } else if (cmd == "tz_rule") {
    String tz_rule = param[1].asStr();
    Serial.print("POSIX TZ rule:   "); Serial.println(tz_rule);

  } else if (cmd == "dst_next") {
    uint32_t next1_ts  = param[1].asLong();
    int next1_off      = param[2].asInt();
    uint32_t next2_ts  = param[3].asLong();
    int next2_off      = param[4].asInt();

    Serial.print("Next offset changes: ");
    Serial.print(next1_off); Serial.print("min. on "); Serial.print(next1_ts);
    Serial.print(", ");
    Serial.print(next2_off); Serial.print("min. on "); Serial.print(next2_ts);
    Serial.println();
  }
}

void setup()
{
  // Debug console
  Serial.begin(115200);

  Blynk.begin(BLYNK_AUTH_TOKEN);
}

void loop()
{
  Blynk.run();
}

