
#include <WiFiClient.h>
#include <WebServer.h>
#include <DNSServer.h>

  const char* config_form = R"html(
<!DOCTYPE HTML>
<html>
<head>
  <title>WiFi setup</title>
  <style>
  body {
    background-color: #fcfcfc;
    box-sizing: border-box;
  }
  body, input {
    font-family: Roboto, sans-serif;
    font-weight: 400;
    font-size: 16px;
  }
  .centered {
    position: fixed;
    top: 50%;
    left: 50%;
    transform: translate(-50%, -50%);

    padding: 20px;
    background-color: #ccc;
    border-radius: 4px;
  }
  td { padding:0 0 0 5px; }
  label { white-space:nowrap; }
  input { width: 20em; }
  input[name="port"] { width: 5em; }
  input[type="submit"], img { margin: auto; display: block; width: 30%; }
  </style>
</head> 
<body>
<div class="centered">
  <form method="get" action="config">
    <table>
    <tr><td><label for="ssid">WiFi SSID:</label></td>  <td><input type="text" name="ssid" length=64 required="required"></td></tr>
    <tr><td><label for="pass">Password:</label></td>   <td><input type="text" name="pass" length=64></td></tr>
    <tr><td><label for="blynk">Auth token:</label></td><td><input type="text" name="blynk" placeholder="a0b1c2d..." pattern="[-_a-zA-Z0-9]{32}" maxlength="32" required="required"></td></tr>
    <tr><td><label for="host">Host:</label></td>       <td><input type="text" name="host" value="blynk.cloud" length=64></td></tr>
    <tr><td><label for="port_ssl">Port:</label></td>   <td><input type="number" name="port_ssl" value="443" min="1" max="65535"></td></tr>
    </table><br/>
    <input type="submit" value="Apply">
  </form>
</div>
</body>
</html>
)html";

WebServer server(80);
DNSServer dnsServer;
const byte DNS_PORT = 53;

static int connectNetRetries    = WIFI_CLOUD_MAX_RETRIES;
static int connectBlynkRetries  = WIFI_CLOUD_MAX_RETRIES;

void restartMCU() {
  NVIC_SystemReset();
}

static
String encodeUniquePart(uint32_t n, unsigned len)
{
  static constexpr char alphabet[] = { "0W8N4Y1HP5DF9K6JM3C2UA7R" };
  static constexpr int base = sizeof(alphabet)-1;

  char buf[16] = { 0, };
  char prev = 0;
  for (unsigned i = 0; i < len; n /= base) {
    char c = alphabet[n % base];
    if (c == prev) {
      c = alphabet[(n+1) % base];
    }
    prev = buf[i++] = c;
  }
  return String(buf);
}

static
String getWiFiName(bool withPrefix = true)
{
  static byte mac[6] = { 0, };
  static bool needMac = true;
  if (needMac) {
    WiFi.macAddress(mac);
    needMac = false;
  }

  uint32_t unique = 0;
  for (int i=0; i<4; i++) {
    unique = BlynkCRC32(&mac, sizeof(mac), unique);
  }
  String devUnique = encodeUniquePart(unique, 4);

  String devPrefix = CONFIG_DEVICE_PREFIX;
  String devName = String(BLYNK_DEVICE_NAME).substring(0, 31-6-devPrefix.length());

  if (withPrefix) {
    return devPrefix + " " + devName + "-" + devUnique;
  } else {
    return devName + "-" + devUnique;
  }
}

static
String getWiFiMacAddress() {
  return WiFi.macAddress();
}

static
String getWiFiApBSSID() {
  return WiFi.softAPmacAddress();
}

static
String getWiFiNetworkSSID() {
  return WiFi.SSID();
}

static
String getWiFiNetworkBSSID() {
  return WiFi.BSSIDstr();
}

String scanNetworks()
{
    DEBUG_PRINT("Scanning networks...");
    int wifi_nets = WiFi.scanNetworks(true, true);
    const uint32_t t = millis();
    while (wifi_nets < 0 &&
           millis() - t < 20000)
    {
      delay(20);
      wifi_nets = WiFi.scanComplete();
    }
    DEBUG_PRINT(String("Found networks: ") + wifi_nets);

    if (wifi_nets > 0) {
      // Sort networks
      int indices[wifi_nets];
      for (int i = 0; i < wifi_nets; i++) {
        indices[i] = i;
      }
      for (int i = 0; i < wifi_nets; i++) {
        for (int j = i + 1; j < wifi_nets; j++) {
          if (WiFi.RSSI(indices[j]) > WiFi.RSSI(indices[i])) {
            std::swap(indices[i], indices[j]);
          }
        }
      }

      wifi_nets = BlynkMin(15, wifi_nets); // Show top 15 networks

      // TODO: skip empty names
      String result = "[\n";

      char buff[256];
      for (int i = 0; i < wifi_nets; i++){
        int id = indices[i];

        const char* sec;
        switch (WiFi.encryptionType(id)) {
        case WIFI_AUTH_WEP:          sec = "WEP"; break;
        case WIFI_AUTH_WPA_PSK:      sec = "WPA/PSK"; break;
        case WIFI_AUTH_WPA2_PSK:     sec = "WPA2/PSK"; break;
        case WIFI_AUTH_WPA_WPA2_PSK: sec = "WPA/WPA2/PSK"; break;
        case WIFI_AUTH_OPEN:         sec = "OPEN"; break;
        default:                     sec = "unknown"; break;
        }

        snprintf(buff, sizeof(buff),
          R"json(  {"ssid":"%s","bssid":"%s","rssi":%i,"sec":"%s","ch":%i})json",
          WiFi.SSID(id).c_str(),
          WiFi.BSSIDstr(id).c_str(),
          WiFi.RSSI(id),
          sec,
          WiFi.channel(id)
        );

        result += buff;
        if (i != wifi_nets-1) result += ",\n";
      }
      return result + "\n]";
    } else {
      return "[]";
    }
}

void handleRoot() {
  server.send(200, "text/html", config_form);
}

String networks = "[]";

void enterConfigMode()
{
  networks = scanNetworks();

  WiFi.mode(WIFI_OFF);
  delay(100);
  WiFi.mode(WIFI_AP);
  delay(2000);
  WiFi.softAPConfig(WIFI_AP_IP, WIFI_AP_IP, WIFI_AP_Subnet);
  WiFi.softAP(getWiFiName().c_str());
  delay(500);

  // Set up DNS Server
  dnsServer.setTTL(300); // Time-to-live 300s
  dnsServer.setErrorReplyCode(DNSReplyCode::ServerFailure); // Return code for non-accessible domains
#ifdef WIFI_CAPTIVE_PORTAL_ENABLE
  dnsServer.start(DNS_PORT, "*", WiFi.softAPIP()); // Point all to our IP
  server.onNotFound(handleRoot);
#else
  dnsServer.start(DNS_PORT, CONFIG_AP_URL, WiFi.softAPIP());
  DEBUG_PRINT(String("AP URL:  ") + CONFIG_AP_URL);
#endif

  server.on("/config", []() {
    DEBUG_PRINT("Applying configuration...");
    String ssid = server.arg("ssid");
    String ssidManual = server.arg("ssidManual");
    String pass = server.arg("pass");
    if (ssidManual != "") {
      ssid = ssidManual;
    }
    String token = server.arg("blynk");
    String host  = server.arg("host");
    String port  = server.arg("port_ssl");

    String ip   = server.arg("ip");
    String mask = server.arg("mask");
    String gw   = server.arg("gw");
    String dns  = server.arg("dns");
    String dns2 = server.arg("dns2");

    bool forceSave  = server.arg("save").toInt();

    String content;

    DEBUG_PRINT(String("WiFi SSID: ") + ssid + " Pass: " + pass);
    DEBUG_PRINT(String("Blynk cloud: ") + token + " @ " + host + ":" + port);

    if (token.length() == 32 && ssid.length() > 0) {
      configStore = configDefault;
      CopyString(ssid, configStore.wifiSSID);
      CopyString(pass, configStore.wifiPass);
      CopyString(token, configStore.cloudToken);
      if (host.length()) {
        CopyString(host,  configStore.cloudHost);
      }
      if (port.length()) {
        configStore.cloudPort = port.toInt();
      }

      IPAddress addr;

      if (ip.length() && addr.fromString(ip)) {
        configStore.staticIP = addr;
        configStore.setFlag(CONFIG_FLAG_STATIC_IP, true);
      } else {
        configStore.setFlag(CONFIG_FLAG_STATIC_IP, false);
      }
      if (mask.length() && addr.fromString(mask)) {
        configStore.staticMask = addr;
      }
      if (gw.length() && addr.fromString(gw)) {
        configStore.staticGW = addr;
      }
      if (dns.length() && addr.fromString(dns)) {
        configStore.staticDNS = addr;
      }
      if (dns2.length() && addr.fromString(dns2)) {
        configStore.staticDNS2 = addr;
      }

      if (forceSave) {
        configStore.setFlag(CONFIG_FLAG_VALID, true);
        config_save();

        content = R"json({"status":"ok","msg":"Configuration saved"})json";
      } else {
        content = R"json({"status":"ok","msg":"Trying to connect..."})json";
      }
      server.send(200, "application/json", content);

      connectNetRetries = connectBlynkRetries = 1;
      BlynkState::set(MODE_SWITCH_TO_STA);
    } else {
      DEBUG_PRINT("Configuration invalid");
      content = R"json({"status":"error","msg":"Configuration invalid"})json";
      server.send(500, "application/json", content);
    }
  });
  server.on("/board_info.json", []() {
    // Configuring starts with board info request (may impact indication)
    BlynkState::set(MODE_CONFIGURING);

    DEBUG_PRINT("Sending board info...");
    const char* tmpl = BLYNK_TEMPLATE_ID;

    char buff[512];
    snprintf(buff, sizeof(buff),
      R"json({"board":"%s","tmpl_id":"%s","fw_type":"%s","fw_ver":"%s","ssid":"%s","bssid":"%s","mac":"%s","last_error":%d,"wifi_scan":true,"static_ip":true,"5ghz":true})json",
      BLYNK_DEVICE_NAME,
      tmpl ? tmpl : "Unknown",
      BLYNK_FIRMWARE_TYPE,
      BLYNK_FIRMWARE_VERSION,
      getWiFiName().c_str(),
      getWiFiApBSSID().c_str(),
      getWiFiMacAddress().c_str(),
      configStore.last_error
    );
    server.send(200, "application/json", buff);
  });
  server.on("/wifi_scan.json", []() {
    server.send(200, "application/json", networks);
  });
  server.on("/reset", []() {
    BlynkState::set(MODE_RESET_CONFIG);
    server.send(200, "application/json", R"json({"status":"ok","msg":"Configuration reset"})json");
  });
  server.on("/reboot", []() {
    restartMCU();
  });

  server.begin();

  while (BlynkState::is(MODE_WAIT_CONFIG) || BlynkState::is(MODE_CONFIGURING)) {
    delay(10);
    dnsServer.processNextRequest();
    server.handleClient();
    app_loop();
    if (BlynkState::is(MODE_CONFIGURING) && WiFi.softAPgetStationNum() == 0) {
      BlynkState::set(MODE_WAIT_CONFIG);
    }
  }

  server.stop();
}

void enterConnectNet() {
  BlynkState::set(MODE_CONNECTING_NET);
  DEBUG_PRINT(String("Connecting to WiFi: ") + configStore.wifiSSID);

  String hostname = getWiFiName();
  hostname.replace(" ", "-");
  WiFi.setHostname(hostname.c_str());

  if (configStore.getFlag(CONFIG_FLAG_STATIC_IP)) {
    if (!WiFi.config(configStore.staticIP,
                    configStore.staticGW,
                    configStore.staticMask,
                    configStore.staticDNS,
                    configStore.staticDNS2)
    ) {
      DEBUG_PRINT("Failed to configure Static IP");
      config_set_last_error(BLYNK_PROV_ERR_CONFIG);
      BlynkState::set(MODE_ERROR);
      return;
    }
  }

  if (strlen(configStore.wifiPass)) {
    WiFi.begin(configStore.wifiSSID, configStore.wifiPass);
  } else {
    WiFi.begin(configStore.wifiSSID);
  }

  unsigned long timeoutMs = millis() + WIFI_NET_CONNECT_TIMEOUT;
  while ((timeoutMs > millis()) && (WiFi.status() != WL_CONNECTED))
  {
    delay(10);
    app_loop();

    if (!BlynkState::is(MODE_CONNECTING_NET)) {
      WiFi.disconnect();
      return;
    }
  }

  if (WiFi.status() == WL_CONNECTED) {
    IPAddress localip = WiFi.localIP();
    if (configStore.getFlag(CONFIG_FLAG_STATIC_IP)) {
      BLYNK_LOG_IP("Using Static IP: ", localip);
    } else {
      BLYNK_LOG_IP("Using Dynamic IP: ", localip);
    }

    connectNetRetries = WIFI_CLOUD_MAX_RETRIES;
    BlynkState::set(MODE_CONNECTING_CLOUD);
  } else if (--connectNetRetries <= 0) {
    config_set_last_error(BLYNK_PROV_ERR_NETWORK);
    BlynkState::set(MODE_ERROR);
  }
}

void enterConnectCloud() {
  BlynkState::set(MODE_CONNECTING_CLOUD);

  Blynk.config(configStore.cloudToken, configStore.cloudHost, configStore.cloudPort);
  Blynk.connect(0);

  unsigned long timeoutMs = millis() + WIFI_CLOUD_CONNECT_TIMEOUT;
  while ((timeoutMs > millis()) &&
        (WiFi.status() == WL_CONNECTED) &&
        (!Blynk.isTokenInvalid()) &&
        (Blynk.connected() == false))
  {
    delay(10);
    Blynk.run();
    app_loop();
    if (!BlynkState::is(MODE_CONNECTING_CLOUD)) {
      Blynk.disconnect();
      return;
    }
  }

  if (millis() > timeoutMs) {
    DEBUG_PRINT("Timeout");
  }

  if (Blynk.isTokenInvalid()) {
    config_set_last_error(BLYNK_PROV_ERR_TOKEN);
    BlynkState::set(MODE_WAIT_CONFIG); // TODO: retry after timeout
  } else if (WiFi.status() != WL_CONNECTED) {
    BlynkState::set(MODE_CONNECTING_NET);
  } else if (Blynk.connected()) {
    BlynkState::set(MODE_RUNNING);
    connectBlynkRetries = WIFI_CLOUD_MAX_RETRIES;

    if (!configStore.getFlag(CONFIG_FLAG_VALID)) {
      configStore.last_error = BLYNK_PROV_ERR_NONE;
      configStore.setFlag(CONFIG_FLAG_VALID, true);
      config_save();
    }
  } else if (--connectBlynkRetries <= 0) {
    config_set_last_error(BLYNK_PROV_ERR_CLOUD);
    BlynkState::set(MODE_ERROR);
  }
}

void enterSwitchToSTA() {
  BlynkState::set(MODE_SWITCH_TO_STA);

  DEBUG_PRINT("Switching to STA...");

  delay(1000);
  WiFi.mode(WIFI_OFF);
  delay(100);
  WiFi.mode(WIFI_STA);

  BlynkState::set(MODE_CONNECTING_NET);
}

void enterError() {
  BlynkState::set(MODE_ERROR);

  unsigned long timeoutMs = millis() + 10000;
  while (timeoutMs > millis() || g_buttonPressed)
  {
    delay(10);
    app_loop();
    if (!BlynkState::is(MODE_ERROR)) {
      return;
    }
  }
  DEBUG_PRINT("Restarting after error.");
  delay(10);

  restartMCU();
}

