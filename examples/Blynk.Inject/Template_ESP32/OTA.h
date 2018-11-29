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
 * How to trigger an OTA update?
 *   1. In Arduino IDE menu: Sketch -> Export compiled Binary
 *   2. Open console, navigate to the sketch directory
 *   3.a Trigger update using HTTPS API on local server for specific hardware:
 *       curl -v -F file=@Template_ESP8266.ino.nodemcu.bin --insecure -u admin@blynk.cc:admin https://localhost:9443/admin/ota/start?token=123
 *   3.b Trigger update using HTTPS API on local server for all hardware:
 *       curl -v -F file=@Template_ESP8266.ino.nodemcu.bin --insecure -u admin@blynk.cc:admin https://localhost:9443/admin/ota/start
 *   3.c Trigger update using HTTPS API on local server for single user:
 *       curl -v -F file=@Template_ESP8266.ino.nodemcu.bin --insecure -u admin@blynk.cc:admin https://localhost:9443/admin/ota/start?user=pupkin@gmail.com
 *   3.d Trigger update using HTTPS API on local server for single user and specific project:
 *       curl -v -F file=@Template_ESP8266.ino.nodemcu.bin --insecure -u admin@blynk.cc:admin https://localhost:9443/admin/ota/start?user=pupkin@gmail.com&project=123
 * More about ESP8266 OTA updates:
 *  https://github.com/esp8266/Arduino/blob/master/doc/ota_updates/readme.md
 */

#include <WiFi.h>
#include <Update.h>
#include <HTTPClient.h>

String overTheAirURL;

BLYNK_WRITE(InternalPinOTA) {
  overTheAirURL = param.asString();

  // Disconnect, not to interfere with OTA process
  Blynk.disconnect();

  // Start OTA
  BlynkState::set(MODE_OTA_UPGRADE);
  delay(500);
}

void enterOTA() {
  BlynkState::set(MODE_OTA_UPGRADE);

  DEBUG_PRINT(String("Firmware update URL: ") + overTheAirURL);

  HTTPClient http;
  http.begin(overTheAirURL);

  int httpCode = http.GET();
  if (httpCode != HTTP_CODE_OK) {
    DEBUG_PRINT("HTTP response should be 200");
    BlynkState::set(MODE_ERROR);
    return;
  }
  int contentLength = http.getSize();
  if (contentLength <= 0) {
    DEBUG_PRINT("Content-Length not defined");
    BlynkState::set(MODE_ERROR);
    return;
  }

  bool canBegin = Update.begin(contentLength);
  if (!canBegin) {
    DEBUG_PRINT("Not enough space to begin OTA");
    BlynkState::set(MODE_ERROR);
    return;
  }

  Client& client = http.getStream();
  int written = Update.writeStream(client);
  if (written != contentLength) {
    DEBUG_PRINT(String("OTA written ") + written + " / " + contentLength + " bytes");
    BlynkState::set(MODE_ERROR);
    return;
  }

  if (!Update.end()) {
    DEBUG_PRINT("Error #" + String(Update.getError()));
    BlynkState::set(MODE_ERROR);
    return;
  }

  if (!Update.isFinished()) {
    DEBUG_PRINT("Update failed.");
    BlynkState::set(MODE_ERROR);
    return;
  }

  DEBUG_PRINT("Update successfully completed. Rebooting.");
  ESP.restart();
}

