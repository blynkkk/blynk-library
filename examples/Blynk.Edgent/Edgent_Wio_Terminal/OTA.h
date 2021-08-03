
#include <ArduinoOTA.h> // only for InternalStorage
#include <ArduinoHttpClient.h>

#define OTA_FATAL(...) { BLYNK_LOG1(__VA_ARGS__); delay(1000); restartMCU(); }

#define USE_SSL

String overTheAirURL;

extern BlynkTimer edgentTimer;

BLYNK_WRITE(InternalPinOTA) {
  overTheAirURL = param.asString();

  overTheAirURL.replace("https://", "http://");

  edgentTimer.setTimeout(2000L, [](){
    // Start OTA
    Blynk.logEvent("sys_ota", "OTA started");

    // Disconnect, not to interfere with OTA process
    Blynk.disconnect();

    BlynkState::set(MODE_OTA_UPGRADE);
  });
}

bool parseURL(String url, String& protocol, String& host, int& port, String& uri)
{
  int index = url.indexOf(':');
  if(index < 0) {
    return false;
  }

  protocol = url.substring(0, index);
  url.remove(0, (index + 3)); // remove protocol part

  index = url.indexOf('/');
  String server = url.substring(0, index);
  url.remove(0, index);       // remove server part

  index = server.indexOf(':');
  if(index >= 0) {
    host = server.substring(0, index);          // hostname
    port = server.substring(index + 1).toInt(); // port
  } else {
    host = server;
    if (protocol == "http") {
      port = 80;
    } else if (protocol == "https") {
      port = 443;
    }
  }

  if (url.length()) {
    uri = url;
  } else {
    uri = "/";
  }
  return true;
}

void enterOTA() {
  BlynkState::set(MODE_OTA_UPGRADE);

  // Disconnect, not to interfere with OTA process
  Blynk.disconnect();

  String protocol, host, url;
  int port;
  
  DEBUG_PRINT(String("OTA: ") + overTheAirURL);

  if (!parseURL(overTheAirURL, protocol, host, port, url)) {
    OTA_FATAL(F("Cannot parse URL"));
  }

  DEBUG_PRINT(String("Connecting to ") + host + ":" + port);

  Client* client = NULL;
  if (protocol == "http") {
    client = new WiFiClient();
#ifdef USE_SSL
  } else if (protocol == "https") {
    client = &_blynkWifiClient;
    //client = new WiFiClientSecure();
#endif
  } else {
    OTA_FATAL(String("Unsupported protocol: ") + protocol);
  }
  HttpClient http(*client, host, port);
  http.get(url);

  int statusCode = http.responseStatusCode();
  Serial.print("Update status code: ");
  Serial.println(statusCode);
  if (statusCode != 200) {
    http.stop();
    return;
  }


  long length = http.contentLength();
  if (length == HttpClient::kNoContentLengthHeader) {
    http.stop();
    Serial.println("Server didn't provide Content-length header. Can't continue with update.");
    return;
  }
  Serial.print("Server returned update file of size ");
  Serial.print(length);
  Serial.println(" bytes");

  if (!InternalStorage.open(length)) {
    http.stop();
    Serial.println("There is not enough space to store the update. Can't continue with update.");
    return;
  }
  //InternalStorage.debugPrint();

  byte b;
  while (length > 0) {
    if (!http.readBytes(&b, 1)) // reading a byte with timeout
      break;
    InternalStorage.write(b);
    length--;
  }
  InternalStorage.close();
  http.stop();
  if (length > 0) {
    Serial.print("Timeout downloading update file at ");
    Serial.print(length);
    Serial.println(" bytes. Can't continue with update.");
    return;
  }

  DEBUG_PRINT("=== Update successfully completed. Rebooting.");
  InternalStorage.apply();
}

