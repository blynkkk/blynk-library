/*
 * OTA is not ready for this board yet.
 * It could be derived from:
 * https://www.hackster.io/flower-platform/program-mkr-over-the-air-goodies-voice-control-etc-562e9a
 */

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
  BlynkState::set(MODE_ERROR);
}

