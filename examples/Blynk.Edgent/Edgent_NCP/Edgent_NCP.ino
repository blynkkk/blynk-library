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
 *************************************************************

  NOTE: This example requires the connectivity module on your board to be
        flashed using Blynk.NCP firmware.

   The easiest way to install the NCP firmware is using this PlatformIO project:
   https://github.com/blynkkk/BlynkNcpExample

 *************************************************************/

/* Fill in information from your Blynk Template here */
/* Read more: https://bit.ly/BlynkInject */
//#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
//#define BLYNK_TEMPLATE_NAME         "Device"

/* The firmware version of the Primary MCU (used for OTA updates) */
#define BLYNK_FIRMWARE_VERSION        "0.1.0"

// Debug output
#define BLYNK_PRINT                   Serial

// Redefine NCP connection port settings, if needed
//#define BLYNK_NCP_SERIAL            Serial1
//#define BLYNK_NCP_BAUD              2000000

#include <BlynkEdgentNCP.h>

BlynkTimer timer;

BLYNK_CONNECTED() {
  BLYNK_LOG("Connected to Blynk 🙌");
}

BLYNK_DISCONNECTED() {
  BLYNK_LOG("Blynk disconnected");
}

void setup() {
  Serial.begin(115200);
  Serial.println();

  // Give Serial Monitor some time to connect
  delay(3000);

  BLYNK_LOG("Main firmware: %s", BLYNK_FIRMWARE_VERSION);
  BLYNK_LOG("Build: %s", __DATE__ " " __TIME__);

  // Initialize the Blynk.NCP hardware
  if (Blynk.initNCP()) {
    String ver = Blynk.getNcpVersion();
    BLYNK_LOG("Blynk.NCP firmware: %s", ver.c_str());
  } else {
    BLYNK_LOG("Cannot communicate to Blynk.NCP");
    BLYNK_LOG("  Please ensure you have flashed your board with the Blynk.NCP firmware, before running this example.");
    BLYNK_LOG("  See: https://github.com/blynkkk/BlynkNcpExample");
    return;
  }

  // Print state changes
  Blynk.onStateChange([]() {
    BLYNK_LOG("State: %s", Blynk.getStateString());
  });

  // Set config mode timeout to 30 minutes, for testing purposes
  Blynk.setConfigTimeout(30 * 60);

  // White labeling (use this ONLY if you have a branded Blynk App)
  //Blynk.setVendorPrefix("MyCompany");
  //Blynk.setVendorServer("dashboard.mycompany.com");

  // Product setup
  Blynk.begin(BLYNK_TEMPLATE_ID, BLYNK_TEMPLATE_NAME);

  // Publish some data periodically
  timer.setInterval(1000, []() {
    Blynk.virtualWrite(V0, millis() / 1000);
  });
}

void loop() {
  timer.run();
  Blynk.run();
  delay(1);
}
