
String overTheAirURL;

BLYNK_WRITE(InternalPinOTA) {
  overTheAirURL = param.asString();

  delay(500);
  // Disconnect, not to interfere with OTA process
  Blynk.disconnect();

  // Start OTA
  BlynkState::set(MODE_OTA_UPGRADE);
}

void enterOTA() {
  BlynkState::set(MODE_ERROR);
}

