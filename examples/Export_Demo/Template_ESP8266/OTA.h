/**************************************************************
 * This is a DEMO. You can use it only for development and testing.
 *
 * If you would like to add these features to your product,
 * please contact Blynk for Business:
 *
 *                  http://www.blynk.io/
 *
 **************************************************************
 *
 * This is just an example how you can trigger OTA update using a virtual pin.
 *   1. In Arduino IDE menu: Sketch -> Export compiled Binary
 *   2. Navigate to the sketch directory
 *   3. Edit ota-firmwares.json:
 *        Set your auth token, firmware version and .bin filename
 *   4. Run OTA server:
          node ota-server.js
 *   5. Trigger update by sending the firmware url to V100.
 *      For example, using blynk-ctrl.js:
 *        blynk-ctrl.js --token your-token -vw 100 'http://your-server:3000/firmware.bin?auth=your-token'
 * 
 * More about ESP8266 OTA updates:
 *  https://github.com/esp8266/Arduino/blob/master/doc/ota_updates/readme.md
 */

#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

String overTheAirURL;

BLYNK_WRITE(100) {
  // url should be something like "http://server:port/firmware.bin?auth=12345"
  String overTheAirURL = param.asString();

  // Disconnect, not to interfere with OTA process
  Blynk.disconnect();

  // Start OTA
  BlynkState::set(MODE_OTA_UPGRADE);
  delay(500);
}

void enterOTA() {
  BlynkState::set(MODE_OTA_UPGRADE);

  DEBUG_PRINT(String("Firmware update URL: ") + overTheAirURL);
  switch (ESPhttpUpdate.update(overTheAirURL, BOARD_FIRMWARE_VERSION)) {
  case HTTP_UPDATE_FAILED:
    DEBUG_PRINT(String("Firmware update failed (error ") + ESPhttpUpdate.getLastError() + "): " + ESPhttpUpdate.getLastErrorString());
    BlynkState::set(MODE_ERROR);
    break;
  case HTTP_UPDATE_NO_UPDATES:
    DEBUG_PRINT("No firmware updates available.");
    BlynkState::set(MODE_CONNECTING_CLOUD);
    break;
  case HTTP_UPDATE_OK:
    DEBUG_PRINT("Firmware updated.");
    restartMCU();
    break;
  }
}

