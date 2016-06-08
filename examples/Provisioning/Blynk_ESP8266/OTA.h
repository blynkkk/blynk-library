/* 
 * This is just an example how you can trigger OTA update using a virtual pin.
 *   1. In Arduino IDE menu: Sketch -> Export compiled Binary
 *   2. Navigate to the sketch directory
 *   3. Edit ota-firmwares.json
 *      Set your auth token, firmware version and .bin filename
 *   4. Run ota-server.js
 *   5. Trigger update by sending firmware url to V100.
 *      For example, using blynk-ctrl.js:
 *        blynk-ctrl.js --token your-token -vw 100 'http://your-server:3000/firmware.bin?auth=your-token'
 * 
 * More about ESP8266 OTA updates:
 *  https://github.com/esp8266/Arduino/blob/master/doc/ota_updates/readme.md
 */

#include <ESP8266HTTPClient.h>
#include <ESP8266httpUpdate.h>

BLYNK_WRITE(100) {
  // url should be something like "http://server:port/firmware.bin?auth=12345"
  String url = param.asString();
  Serial.print("Firmware update URL: ");
  Serial.println(url);
  switch (ESPhttpUpdate.update(url, BOARD_FIRMWARE_VERSION)) {
  case HTTP_UPDATE_FAILED:
    Serial.print("Firmware update failed (error ");
    Serial.print(ESPhttpUpdate.getLastError());
    Serial.print("): ");
    Serial.println(ESPhttpUpdate.getLastErrorString());
    break;
  case HTTP_UPDATE_NO_UPDATES:
    Serial.println("No firmware updates available.");
    break;
  case HTTP_UPDATE_OK:
    Serial.println("Firmware updated.");
    break;
  }
}

