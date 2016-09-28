/**************************************************************
 * This is a DEMO. You can use it only for development and testing.
 * You should open Setting.h and modify General options.
 *
 * If you would like to add these features to your product,
 * please contact Blynk for Business:
 *
 *                  http://www.blynk.io/
 *
 **************************************************************/

#define USE_SPARKFUN_BLYNK_BOARD
//#define USE_NODE_MCU_BOARD
//#define USE_WITTY_CLOUD_BOARD

#define DEBUG        // Comment this out to disable debug prints

#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp8266.h>
#include "BlynkProvisioning.h"

void setup() {
  delay(500);
  Serial.begin(115200);

  BlynkProvisioning.begin();

  // Initialize this example
  example_init();
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
SimpleTimer timer;

static int sensorSoilMoisture = 60;
static int sensorAirHumidity = 50;
static int wateringAmount = 5;
static int wateringTimer = -1;
static bool isNotificationSent = false;

// Set fake watering amount slider
BLYNK_WRITE(V5) {
  wateringAmount = param.asInt();
}

// Start Watering button
BLYNK_WRITE(V6) {
  if (param.asInt() == 1) {
    // If watering started -> start simulating watering
    timer.enable(wateringTimer);
  } else {
    // If watering stopped -> stop simulating watering
    timer.disable(wateringTimer);
  }
}

// When device starts ->
//   sync watering switch button status
//   and watering amount level from the cloud (last App value)
BLYNK_CONNECTED() {
  Blynk.syncVirtual(V5);
  Blynk.syncVirtual(V6);
}

// This simulates sinusoidal function
float sinusoidal(float minv, float maxv, float period) {
  float amp = (maxv - minv) / 2.0;
  float med = minv + amp;
  return med + amp * sin((M_PI*2*millis())/period);
}

// This simulates value jittering
float randomize(float minv, float maxv) {
  return float(random(minv*1000, maxv*1000))/1000;
}


void example_init() {

  // Update sensors each 3 seconds
  timer.setInterval(3000L, []() {
    // Soil moisture
    Blynk.virtualWrite(V1, sensorSoilMoisture);

    float dayPeriod = 3.0 * 60 * 1000;
    // Light level
    Blynk.virtualWrite(V2, (int)sinusoidal(5, 95, dayPeriod));
    // Temperature
    Blynk.virtualWrite(V3, sinusoidal(18, 23, dayPeriod) + randomize(-1.0, 1.0));

  });

  // Humidity updates at different rate (5s)
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
        Blynk.notify("Your plant is thirsty!");
        isNotificationSent = true;
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
  });
  timer.disable(wateringTimer);

}

void example_run() {
  timer.run();
}

