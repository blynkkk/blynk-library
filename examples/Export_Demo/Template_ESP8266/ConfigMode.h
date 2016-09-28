/**************************************************************
 * This is a DEMO. You can use it only for development and testing.
 *
 * If you would like to add these features to your product,
 * please contact Blynk for Business:
 *
 *                  http://www.blynk.io/
 *
 **************************************************************/

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPUpdateServer.h>
#include <DNSServer.h>

ESP8266WebServer server(WIFI_AP_CONFIG_PORT);
ESP8266HTTPUpdateServer httpUpdater;
DNSServer dnsServer;
const byte DNS_PORT = 53;

const char* config_form = R"html(
<!DOCTYPE HTML><html>
<form method='get' action='config'>
  <label>WiFi SSID: </label><input type="text" name="ssid" length=32 required="required"><br/>
  <label>Password:  </label><input type="text" name="pass" length=32><br/>
  <label>Auth token:</label><input type="text" name="blynk" placeholder="a0b1c2d..." pattern="[a-zA-Z0-9]{32}" maxlength="32" required="required"><br/>
  <label>Host: </label><input type="text" name="host" length=32><br/>
  <label>Port: </label><input type="number" name="port" value="8442" min="1" max="65535"><br/>
  <input type='submit' value="Apply">
</form>
</html>
)html";

void restartMCU() {
  ESP.restart();
}

void enterConfigMode()
{
  WiFi.disconnect();
  WiFi.enableSTA(false);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(WIFI_AP_IP, WIFI_AP_IP, WIFI_AP_Subnet);
  WiFi.softAP(PRODUCT_WIFI_SSID);
  delay(500);
  IPAddress myIP = WiFi.softAPIP();
  DEBUG_PRINT(String("AP SSID: ") + PRODUCT_WIFI_SSID);
  DEBUG_PRINT(String("AP IP:   ") + myIP[0] + "." + myIP[1] + "." + myIP[2] + "." + myIP[3]);

  if (myIP == (uint32_t)0)
  {
    BlynkState::set(MODE_ERROR);
    return;
  }

  // Set up DNS Server
  dnsServer.setTTL(300); // Time-to-live 300s
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure); // Return code for non-accessible domains
#ifdef WIFI_CAPTIVE_PORTAL_ENABLE
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP()); // Point all to our IP
  server.onNotFound(handleRoot);
#else
  dnsServer.start(DNS_PORT, BOARD_CONFIG_AP_URL, WiFi.softAPIP());
  DEBUG_PRINT(String("AP URL:  ") + BOARD_CONFIG_AP_URL);
#endif

  httpUpdater.setup(&server);

  server.on("/", []() {
    server.send(200, "text/html", config_form);
  });
  server.on("/config", []() {
    String ssid = server.arg("ssid");
    String ssidManual = server.arg("ssidManual");
    String pass = server.arg("pass");
    if (ssidManual != "") {
      ssid = ssidManual;
    }
    String token = server.arg("blynk");
    String host  = server.arg("host");
    String port  = server.arg("port");

    String content;
    unsigned statusCode;
    if (token.length() == 32 && ssid.length() > 0) {
      CopyString(ssid, configStore.wifiSSID);
      CopyString(pass, configStore.wifiPass);
      CopyString(token, configStore.cloudToken);
      if (host.length()) {
        CopyString(host,  configStore.cloudHost);
      }
      if (port.length()) {
        configStore.cloudPort = port.toInt();
      }

      DEBUG_PRINT(String("WiFi SSID: ") + configStore.wifiSSID + " Pass: " + configStore.wifiPass);
      DEBUG_PRINT(String("Blynk cloud: ") + configStore.cloudToken + " @ " + configStore.cloudHost + ":" + configStore.cloudPort);

      content = R"json({"status":"ok","msg":"Configuration saved"})json";
      statusCode = 200;
      server.send(statusCode, "application/json", content);

      BlynkState::set(MODE_CONNECTING_NET);
    } else {
      DEBUG_PRINT("Configuration invalid");
      content = R"json({"status":"error","msg":"Configuration invalid"})json";
      statusCode = 404;
      server.send(statusCode, "application/json", content);
    }
  });
  server.on("/board_info.json", []() {
    char buff[256];
    snprintf(buff, sizeof(buff),
      R"json({"board":"%s","vendor":"%s","fw_ver":"%s","hw_ver":"%s"})json",
      BOARD_NAME,
      BOARD_VENDOR,
      BOARD_FIRMWARE_VERSION,
      BOARD_HARDWARE_VERSION
    );
    server.send(200, "application/json", buff);
  });
  server.on("/reset", []() {
    config_reset();
    server.send(200, "application/json", R"json({"status":"ok","msg":"Configuration reset"})json");
  });
  server.on("/reboot", []() {
    restartMCU();
  });

  server.begin();

  while (BlynkState::is(MODE_WAIT_CONFIG) || BlynkState::is(MODE_CONFIGURING)) {
    dnsServer.processNextRequest();
    server.handleClient();
    if (BlynkState::is(MODE_WAIT_CONFIG) && WiFi.softAPgetStationNum() > 0) {
      BlynkState::set(MODE_CONFIGURING);
    } else if (BlynkState::is(MODE_CONFIGURING) && WiFi.softAPgetStationNum() == 0) {
      BlynkState::set(MODE_WAIT_CONFIG);
    }
  }

  server.stop();
}

void enterConnectNet() {
  BlynkState::set(MODE_CONNECTING_NET);
  DEBUG_PRINT(String("Connecting to WiFi: ") + configStore.wifiSSID);
  
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  if (!WiFi.begin(configStore.wifiSSID, configStore.wifiPass))
    return;
    
  unsigned long timeoutMs = millis() + WIFI_NET_CONNECT_TIMEOUT;
  while ((timeoutMs > millis()) && (WiFi.status() != WL_CONNECTED))
  {
    delay(100);
    if (!BlynkState::is(MODE_CONNECTING_NET)) {
      WiFi.disconnect();
      return;
    }
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    BlynkState::set(MODE_CONNECTING_CLOUD);
  } else {
    BlynkState::set(MODE_ERROR);
  }
}

void enterConnectCloud() {
  BlynkState::set(MODE_CONNECTING_CLOUD);

  Blynk.disconnect();
  Blynk.config(configStore.cloudToken, configStore.cloudHost, configStore.cloudPort);
  Blynk.connect(0);

  unsigned long timeoutMs = millis() + WIFI_CLOUD_CONNECT_TIMEOUT;
  while ((timeoutMs > millis()) &&
        (Blynk.connected() == false))
  {
    Blynk.run();
    if (!BlynkState::is(MODE_CONNECTING_CLOUD)) {
      Blynk.disconnect();
      return;
    }
  }
  
  if (Blynk.connected()) {
    BlynkState::set(MODE_RUNNING);

    if (!configStore.flagConfig) {
      configStore.flagConfig = true;
      config_save();
      DEBUG_PRINT("Configuration stored to flash");
    }
  } else {
    BlynkState::set(MODE_ERROR);
  }
}

void enterError() {
  BlynkState::set(MODE_ERROR);
  
  unsigned long timeoutMs = millis() + 10000;
  while (timeoutMs > millis() || g_buttonPressed)
  {
    delay(10);
    if (!BlynkState::is(MODE_ERROR)) {
      return;
    }
  }
  DEBUG_PRINT("Restarting after error.");
  delay(10);

  restartMCU();
}

