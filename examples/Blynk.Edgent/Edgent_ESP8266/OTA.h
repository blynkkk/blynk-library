
#define OTA_FATAL(...) { BLYNK_LOG1(__VA_ARGS__); delay(1000); restartMCU(); }

#define USE_SSL

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

#if defined(ESP32)
  #include <Update.h>
  #include <WiFiClientSecure.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <WiFiClientSecure.h>
  #include <time.h>
#endif

#if defined(USE_SSL) && defined(ESP8266)

WiFiClient* connectSSL(const String& host, const int port)
{
  WiFiUDP::stopAll();
  WiFiClient::stopAll();

  time_t now = time(nullptr);
  if (time(nullptr) < 100000) {
    // Synchronize time useing SNTP. This is necessary to verify that
    // the TLS certificates offered by the server are currently valid
    configTime(0, 0, "pool.ntp.org", "time.nist.gov");

    while (now < 100000) {
        delay(100);
        now = time(nullptr);
    }
  }

  // Reuse Secure WIFI Client on ESP8266
  //WiFiClientSecure* clientSSL = &_blynkWifiClient;
  WiFiClientSecure* clientSSL = new WiFiClientSecure();

  clientSSL->setTrustAnchors(&BlynkCert);
  if (!clientSSL->connect(host.c_str(), port)) {
    OTA_FATAL(F("Connection failed"));
  }
  return clientSSL;
}

#elif defined(USE_SSL) && defined(ESP32)

WiFiClient* connectSSL(const String& host, const int port)
{
  WiFiUDP::stopAll();
  WiFiClient::stopAll();

  WiFiClientSecure* clientSSL = new WiFiClientSecure();
  clientSSL->setCACert(BLYNK_DEFAULT_ROOT_CA);
  if (clientSSL->connect(host.c_str(), port)) {
    DEBUG_PRINT(F("Certificate OK"));
  } else {
    OTA_FATAL(F("Secure connection failed"));
  }
  return clientSSL;
}

#endif

WiFiClient* connectTCP(const String& host, const int port)
{
  WiFiUDP::stopAll();
  WiFiClient::stopAll();

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

  bool canBegin = Update.begin(contentLength);
  if (!canBegin) {
#ifdef BLYNK_PRINT
    Update.printError(BLYNK_PRINT);
#endif
    OTA_FATAL("OTA begin failed");
  }

  if (md5.length()) {
    md5.trim();
    md5.toLowerCase();
    DEBUG_PRINT(String("Expected MD5: ") + md5);
    if(!Update.setMD5(md5.c_str())) {
      OTA_FATAL("Cannot set MD5");
    }
  }

  DEBUG_PRINT("Flashing...");

  // The next loop does approx. the same thing as Update.writeStream(http) or Update.write(http)

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

    Update.write(buff, len);
    written += len;
    const int progress = (written*100)/contentLength;
    if (progress - prevProgress >= 10 || progress == 100) {
#ifdef BLYNK_PRINT
      BLYNK_PRINT.print(String("\r ") + progress + "%");
#endif
      prevProgress = progress;
    }
  }
#ifdef BLYNK_PRINT
  BLYNK_PRINT.println();
#endif
  client->stop();

  if (written != contentLength) {
#ifdef BLYNK_PRINT
    Update.printError(BLYNK_PRINT);
#endif
    OTA_FATAL(String("Write failed. Written ") + written + " / " + contentLength + " bytes");
  }

  if (!Update.end()) {
#ifdef BLYNK_PRINT
    Update.printError(BLYNK_PRINT);
#endif
    OTA_FATAL(F("Update not ended"));
  }

  if (!Update.isFinished()) {
    OTA_FATAL(F("Update not finished"));
  }

  DEBUG_PRINT("=== Update successfully completed. Rebooting.");
  restartMCU();
}

