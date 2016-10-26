/**************************************************************
 * This is a DEMO sketch which works with Blynk myPlant app and
 * showcases how your app made with Blynk can work
 *
 * You can download free app here:
 *
 * iOS:     TODO
 * Android: http://play.google.com/store/apps/details?id=cc.blynk.appexport.demo
 *
 * If you would like to add these features to your product,
 * please contact Blynk for Businesses:
 *
 *                  http://www.blynk.io/
 *
 **************************************************************/

#define USE_SPARKFUN_BLYNK_BOARD  // Uncomment the board you are using
//#define USE_NODE_MCU_BOARD      // Comment out the boards you are not using
//#define USE_WITTY_CLOUD_BOARD

#define DEBUG        // Comment this out to disable debug prints

#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include "BlynkProvisioning.h"

void setup() {
  delay(500);
  Serial.begin(115200);


  /**************************************************************
   *
   * Workflow to connect the device to WiFi network.
   * Here is how it works:
   * 1. At the first start hardware acts as an Access Point and
   *    broadcasts it's own WiFi.
   * 2. myPlant smartphone app connects to this Access Point
   * 3. myPlant smartphone app request new Auth Token and passes
   *    it together with user's WiFi SSID and password
   * 4. Hardware saves this information to EEPROM
   * 5. Hardware reboots and now connects to user's WiFi Network
   * 6. Hardware connects to Blynk Cloud and is ready to work with app
   *
   * Next time the hardware reboots, it will use the same configuration
   * to connect. User can RESET the board and re-initiate provisioning
   *
   * Explore the Settings.h for parameters
   * Read the documentation for more info: http://
   *
   **************************************************************/

  BlynkProvisioning.begin();

  example_init(); // Initialize this example
}

void loop() {
  // This handles the network and cloud connection
  BlynkProvisioning.run();

  // Run this example periodic actions
  example_run();
}


/**************************************************************
 *
 *              myPlant example App code
 *
 * The following code simulates plant watering system
 *
 **************************************************************/

#include <SimpleTimer.h>
SimpleTimer timer; // Initiating timer to perform repeating event

static int sensorSoilMoisture = 60;
static int sensorAirHumidity = 50;
static int wateringAmount = 5;
static int wateringTimer = -1;
static bool isNotificationSent = false;

// Getting data from "Set watering amount" slider
BLYNK_WRITE(V5) {
  wateringAmount = param.asInt();
  DEBUG_PRINT(String("Watering amount: ") + wateringAmount);
}

// Getting data from "Start Watering" button
BLYNK_WRITE(V6) {
  if (param.asInt() == 1) {
    // If watering started -> start simulating watering
    timer.enable(wateringTimer);
    DEBUG_PRINT("Watering started by user");
  } else {
    // If watering stopped -> stop simulating watering
    timer.disable(wateringTimer);
    DEBUG_PRINT("Watering stopped by user");
  }
}

// When device starts ->
//   sync watering switch button status
//   and watering amount level from the cloud (last App value)
BLYNK_CONNECTED() {
  Blynk.syncVirtual(V5);
  Blynk.syncVirtual(V6);
}

// This is a sinusoidal function used for simulations
float sinusoidal(float minv, float maxv, float period) {
  float amp = (maxv - minv) / 2.0;
  float med = minv + amp;
  return med + amp * sin((M_PI * 2 * millis()) / period);
}

// Simulating values jittering
float randomize(float minv, float maxv) {
  return float(random(minv * 1000, maxv * 1000)) / 1000;
}


void example_init() {

  // Update sensors each 3 seconds
  timer.setInterval(3000L, []() {
    // Soil moisture
    if (sensorSoilMoisture < 33) {
      Blynk.virtualWrite(V1, "DRY");
    } else if (sensorSoilMoisture > 33) {
      Blynk.virtualWrite(V1, "MOIST");
    } else {
      Blynk.virtualWrite(V1, "WET");
    }

    float dayPeriod = 3.0 * 60 * 1000;

    // Light level
    int light = sinusoidal(5, 95, dayPeriod);
    if (light < 33) {
      Blynk.virtualWrite(V2, "LOW");
    } else if (light > 33) {
      Blynk.virtualWrite(V2, "GOOD");
    } else {
      Blynk.virtualWrite(V2, "MED");
    }

    // Temperature
    Blynk.virtualWrite(V3, sinusoidal(18, 23, dayPeriod) + randomize(-1.0, 1.0));
  });

  // Humidity updates at a different rate (5s)
  timer.setInterval(5000L, []() {
    sensorAirHumidity += random (-5, +5);
    sensorAirHumidity = constrain(sensorAirHumidity, 30, 90);
    Blynk.virtualWrite(V4, sensorAirHumidity);
  });

  // Soil Moisture decreases 3% every second
  timer.setInterval(1000L, []() {
    sensorSoilMoisture -= 3;
    sensorSoilMoisture = constrain(sensorSoilMoisture, 7, 85);

    if (sensorSoilMoisture < 20) {
      if (isNotificationSent == false) {
        Blynk.email("myPlant notification", "Your plant is thirsty!");
        isNotificationSent = true;
        DEBUG_PRINT("Email notification sent");
      }
    }
  });

  // Simulate watering process
  wateringTimer = timer.setInterval(1000L, []() {
    sensorSoilMoisture += wateringAmount;
    sensorSoilMoisture = constrain(sensorSoilMoisture, 7, 85);

    if (sensorSoilMoisture > 30) {
      isNotificationSent = false;
    }
    if (sensorSoilMoisture >= 85) {
      // Stop watering
      timer.disable(wateringTimer);
      // Update "Start Watering" button widget state
      Blynk.virtualWrite(V6, 0);

      DEBUG_PRINT("Watering stopped automatically");
    }
  });
  timer.disable(wateringTimer);

}

void example_run() {
  timer.run();
}

