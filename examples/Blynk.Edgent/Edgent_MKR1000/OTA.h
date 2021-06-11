
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
  BlynkState::set(MODE_ERROR);
}

