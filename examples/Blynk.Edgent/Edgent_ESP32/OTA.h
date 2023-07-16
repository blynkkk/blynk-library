
#include <WiFi.h>
#include <Update.h>
#include <HTTPClient.h>

String overTheAirURL;

extern BlynkTimer edgentTimer;

BLYNK_WRITE(InternalPinOTA) {
  overTheAirURL = param.asString();

  edgentTimer.setTimeout(2000L, [](){
    // Start OTA
    Blynk.logEvent("sys_ota", "OTA started");

    // Disconnect, not to interfere with OTA process
    Blynk.disconnect();

    BlynkState::set(MODE_OTA_UPGRADE);
  });
}

void enterOTA() {
  BlynkState::set(MODE_OTA_UPGRADE);

  DEBUG_PRINT(String("Firmware update URL: ") + overTheAirURL);

  HTTPClient http;
  http.begin(overTheAirURL);

  const char* headerkeys[] = { "x-MD5" };
  http.collectHeaders(headerkeys, sizeof(headerkeys)/sizeof(char*));

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

  if (http.hasHeader("x-MD5")) {
    String md5 = http.header("x-MD5");
    if (md5.length() == 32) {
      md5.toLowerCase();
      DEBUG_PRINT("Expected MD5: " + md5);
      Update.setMD5(md5.c_str());
    }
  }

#ifdef BLYNK_FS
  BLYNK_FS.end();
#endif

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

  DEBUG_PRINT("=== Update successfully completed. Rebooting.");
  restartMCU();
}

