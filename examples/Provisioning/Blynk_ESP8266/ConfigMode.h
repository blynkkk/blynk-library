#include <ESP8266WiFi.h>
#include <WiFiClient.h>
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

void enterConfigMode()
{
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(WIFI_AP_IP, WIFI_AP_IP, WIFI_AP_Subnet);
  WiFi.softAP(PRODUCT_WIFI_SSID);
  IPAddress myIP = WiFi.softAPIP();
  DEBUG_PRINT(String("AP IP address: ") + myIP[0] + "." + myIP[1] + "." + myIP[2] + "." + myIP[3]);
       
  // BUG: ESP8266 IP may be 0 -- makes AP un-connectable
  if (myIP == (uint32_t)0)
  {
    DEBUG_PRINT("IP is zero ;(");
  }

  // Set up DNS Server
  dnsServer.setTTL(300); // Time-to-live 300s
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure); // Return code for non-accessible domains
#ifdef WIFI_CAPTIVE_PORTAL_ENABLE
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP()); // Point all to our IP
  server.onNotFound(handleRoot);
#else
  dnsServer.start(DNS_PORT, BOARD_CONFIG_AP_URL, WiFi.softAPIP());
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

    CopyString(ssid, configStore.wifiSSID);
    CopyString(pass, configStore.wifiPass);
    CopyString(token, configStore.cloudToken);
    if (host != "") {
      CopyString(host,  configStore.cloudHost);
    }
    if (port != "") {
      configStore.cloudPort = port.toInt();
    }
    configStore.flagConfig = true;

    DEBUG_PRINT(String("WiFi SSID: ") + ssid + " Pass: " + pass);
    DEBUG_PRINT(String("Cloud Auth: ") + token + " Host: " + host + " Port: " + port);

    String content;
    unsigned statusCode;
    if (ssid.length() > 0 && pass.length() > 0) {

      content = "{\"Success\":\"saved to eeprom... reset to boot into new wifi\"}";
      statusCode = 200;
      server.send(statusCode, "application/json", content);
      config_save();
      delay(100);
      ESP.restart();
    } else {
      content = "{\"Error\":\"404 not found\"}";
      statusCode = 404;
      DEBUG_PRINT("Sending 404");
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
    server.send(200, "application/json", "");
  });
  server.begin();

  while(1) {
    dnsServer.processNextRequest();
    server.handleClient();
    delay(1);
  }
}


bool wifiCheckForStations(void)
{
  return (WiFi.softAPgetStationNum() > 0);
}

