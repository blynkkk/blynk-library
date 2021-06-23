
#define OTA_FATAL(...) { BLYNK_LOG1(__VA_ARGS__); delay(1000); restartMCU(); }

#define USE_SSL

String overTheAirURL;

extern BlynkTimer edgentTimer;

BLYNK_WRITE(InternalPinOTA) {
  overTheAirURL = param.asString();

  overTheAirURL.replace("http://", "https://");

  edgentTimer.setTimeout(2000L, [](){
    // Start OTA
    Blynk.logEvent("sys_ota", "OTA started");

    // Disconnect, not to interfere with OTA process
    Blynk.disconnect();

    BlynkState::set(MODE_OTA_UPGRADE);
  });
}

#include <ArduinoOTA.h> // only for InternalStorage

#if defined(USE_SSL)

WiFiClient* connectSSL(const String& host, const int port)
{
  // Reuse Client
  WiFiSSLClient* clientSSL = &_blynkWifiClient;
  //WiFiSSLClient* clientSSL = new WiFiSSLClient();

  if (!clientSSL->connect(host.c_str(), port)) {
    OTA_FATAL(F("Connection failed"));
  }
  return clientSSL;
}

#endif

WiFiClient* connectTCP(const String& host, const int port)
{
  WiFiClient* clientTCP = new WiFiClient();
  if (!clientTCP->connect(host.c_str(), port)) {
    OTA_FATAL(F("Client not connected"));
  }
  return clientTCP;
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
    client = connectTCP(host, port);
#ifdef USE_SSL
  } else if (protocol == "https") {
    client = connectSSL(host, port);
#endif
  } else {
    OTA_FATAL(String("Unsupported protocol: ") + protocol);
  }

  client->print(String("GET ") + url + " HTTP/1.0\r\n"
               + "Host: " + host + "\r\n"
               + "Connection: keep-alive\r\n"
               + "\r\n");

  uint32_t timeout = millis();
  while (client->connected() && !client->available()) {
    if (millis() - timeout > 10000L) {
      OTA_FATAL("Response timeout");
    }
    delay(10);
  }

  // Collect headers
  String md5;
  int contentLength = 0;

  while (client->available()) {
    String line = client->readStringUntil('\n');
    line.trim();
    //DEBUG_PRINT(line);    // Uncomment this to show response headers
    line.toLowerCase();
    if (line.startsWith("content-length:")) {
      contentLength = line.substring(line.lastIndexOf(':') + 1).toInt();
    } else if (line.startsWith("x-md5:")) {
      md5 = line.substring(line.lastIndexOf(':') + 1);
    } else if (line.length() == 0) {
      break;
    }
    delay(10);
  }

  if (contentLength <= 0) {
    OTA_FATAL("Content-Length not defined");
  }

  if (!InternalStorage.open(contentLength)) {
    client->stop();
    OTA_FATAL("OTA begin failed");
  }

  DEBUG_PRINT("Flashing...");

  int written = 0;
  int prevProgress = 0;
  uint8_t buff[256];
  while (client->connected() && written < contentLength) {
    delay(10);
    timeout = millis();
    while (client->connected() && !client->available()) {
      delay(1);
      if (millis() - timeout > 10000L) {
        OTA_FATAL("Timeout");
      }
    }

    int len = client->read(buff, sizeof(buff));
    if (len <= 0) continue;

    for (int i = 0; i<len; i++) {
      InternalStorage.write(buff[i]);
    }
    written += len;

    const int progress = (written*100)/contentLength;
    if (progress - prevProgress >= 10 || progress == 100) {
      BLYNK_PRINT.print(String("\r ") + progress + "%");
      prevProgress = progress;
    }
  }
  BLYNK_PRINT.println();
  client->stop();
  InternalStorage.close();

  if (written != contentLength) {
    OTA_FATAL(String("Write failed. Written ") + written + " / " + contentLength + " bytes");
  }

  DEBUG_PRINT("=== Update successfully completed. Rebooting.");
  InternalStorage.apply();
}

