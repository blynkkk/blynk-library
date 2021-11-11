
#include <WiFiClient.h>
#include <utility>

WiFiServer server(80);

String urlDecode(const String& text);
String urlFindArg(const String& url, const String& arg);

enum Request {
  REQ_BOARD_INFO,
  REQ_ROOT,
  REQ_SCAN_WIFI,
  REQ_CONFIG,
  REQ_RESET,
  REQ_REBOOT
};

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

void restartMCU() {
  NVIC_SystemReset();
}


void getWiFiName(char* buff, size_t len, bool withPrefix = true) {
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
  unique &= 0xFFFFF;

  String devName = String(BLYNK_DEVICE_NAME).substring(0, 31-6-6);

  if (withPrefix) {
    snprintf(buff, len, "Blynk %s-%05X", devName.c_str(), unique);
  } else {
    snprintf(buff, len, "%s-%05X", devName.c_str(), unique);
  }
}

String urlDecode(const String& text)
{
  String decoded = "";
  char temp[] = "0x00";
  unsigned int len = text.length();
  unsigned int i = 0;
  while (i < len) {
    char decodedChar;
    char encodedChar = text.charAt(i++);
    if ((encodedChar == '%') && (i + 1 < len)) {
      temp[2] = text.charAt(i++);
      temp[3] = text.charAt(i++);

      decodedChar = strtol(temp, NULL, 16);
    } else {
      if (encodedChar == '+') {
        decodedChar = ' ';
      } else {
        decodedChar = encodedChar;
      }
    }
    decoded += decodedChar;
  }
  return decoded;
}

String urlFindArg(const String& url, const String& arg)
{
  int s = url.indexOf("&" + arg + "=");
  if (s < 0)
    return "";
  int s_len = arg.length() + 2;
  int e = url.indexOf('&', s + s_len);
  return urlDecode(url.substring(s + s_len, e));
}

String scanNetworks()
{
    DEBUG_PRINT("Scanning networks...");
    int wifi_nets = WiFi.scanNetworks();
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
        case AUTH_MODE_SHARED_KEY:   sec = "WEP"; break;
        case AUTH_MODE_WPA:          sec = "WPA"; break;
        case AUTH_MODE_WPA2:         sec = "WPA2"; break;
        case AUTH_MODE_WPA_PSK:      sec = "WPA/PSK"; break;
        case AUTH_MODE_WPA2_PSK:     sec = "WPA2/PSK"; break;
        case AUTH_MODE_OPEN_SYSTEM:  sec = "OPEN"; break;
        default:                     sec = "unknown"; break;
        }

        byte mac[6] = { 0, };
        WiFi.BSSID(id, mac);

        snprintf(buff, sizeof(buff),
          R"json(  {"ssid":"%s","bssid":"%02x:%02x:%02x:%02x:%02x:%02x","rssi":%i,"sec":"%s","ch":%i})json",
          WiFi.SSID(id),
          mac[5], mac[4], mac[3], mac[2], mac[1], mac[0],
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

void enterConfigMode()
{
  char ssidBuff[64];
  getWiFiName(ssidBuff, sizeof(ssidBuff));

  String networks = scanNetworks();

  WiFi.beginAP(ssidBuff);

  delay(500);
  IPAddress myIP = WiFi.localIP();
  DEBUG_PRINT(String("AP SSID: ") + ssidBuff);
  DEBUG_PRINT(String("AP IP:   ") + myIP[0] + "." + myIP[1] + "." + myIP[2] + "." + myIP[3]);

  server.begin();

  while(BlynkState::is(MODE_WAIT_CONFIG)) {
    app_loop();

    WiFiClient client = server.available();   // listen for incoming clients

    if (client) {                             // if you get a client,
      String currentLine = "";                // make a String to hold incoming data from the client
      String config_line = "";
      Request req = REQ_ROOT;
      while (client.connected()) {            // loop while the client's connected
        if (client.available()) {             // if there's bytes to read from the client,
          app_loop();
          char c = client.read();             // read a byte, then
          //Serial.write(c);                    // print it out the serial monitor
          if (c == '\n') {                    // if the byte is a newline character

            // if the current line is blank, you got two newline characters in a row.
            // that's the end of the client HTTP request, so send a response:
            if (currentLine.length() == 0) {
              String responce = "200 OK";
              String content = "";
              String content_type = "text/html";

  switch(req) {
  case REQ_ROOT: {
    content = config_form;
  } break;
  case REQ_CONFIG: {
    DEBUG_PRINT("Applying configuration...");
    String ssid = urlFindArg(config_line, "ssid");
    String ssidManual = urlFindArg(config_line, "ssidManual");
    String pass = urlFindArg(config_line, "pass");
    if (ssidManual != "") {
      ssid = ssidManual;
    }
    String token = urlFindArg(config_line, "blynk");
    String host  = urlFindArg(config_line, "host");
    String port  = urlFindArg(config_line, "port_ssl");

    String ip   = urlFindArg(config_line, "ip");
    String mask = urlFindArg(config_line, "mask");
    String gw   = urlFindArg(config_line, "gw");
    String dns  = urlFindArg(config_line, "dns");
    String dns2 = urlFindArg(config_line, "dns2");

    bool save  = urlFindArg(config_line, "save").toInt();

    DEBUG_PRINT(String("WiFi SSID: ") + ssid + " Pass: " + pass);
    DEBUG_PRINT(String("Blynk cloud: ") + token + " @ " + host + ":" + port);

    if (token.length() == 32 && ssid.length() > 0) {
      configStore.setFlag(CONFIG_FLAG_VALID, false);
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

      if (save) {
        configStore.setFlag(CONFIG_FLAG_VALID, true);
        config_save();

        content = R"json({"status":"ok","msg":"Configuration saved"})json";
      } else {
        content = R"json({"status":"ok","msg":"Trying to connect..."})json";
      }

      BlynkState::set(MODE_SWITCH_TO_STA);
    } else {
      DEBUG_PRINT("Configuration invalid");
      content = R"json({"status":"error","msg":"Configuration invalid"})json";
      responce = "500 Internal Error";
    }
    content_type = "application/json";
  } break;
  case REQ_BOARD_INFO: {
    DEBUG_PRINT("Sending board info...");
    const char* tmpl = BLYNK_TEMPLATE_ID;
    char ssidBuff[64];
    getWiFiName(ssidBuff, sizeof(ssidBuff));
    char buff[512];

    byte mac[6] = { 0, };
    WiFi.macAddress(mac);

    snprintf(buff, sizeof(buff),
      R"json({"board":"%s","tmpl_id":"%s","fw_type":"%s","fw_ver":"%s","ssid":"%s","bssid":"%02x:%02x:%02x:%02x:%02x:%02x","last_error":%d,"wifi_scan":true,"static_ip":true})json",
      BLYNK_DEVICE_NAME,
      tmpl ? tmpl : "Unknown",
      BLYNK_FIRMWARE_TYPE,
      BLYNK_FIRMWARE_VERSION,
      ssidBuff,
      mac[5], mac[4], mac[3], mac[2], mac[1], mac[0],
      configStore.last_error
    );
    content = buff;
    content_type = "application/json";
  } break;
  case REQ_SCAN_WIFI: {
    DEBUG_PRINT("Sending networks...");
    content = networks;
    content_type = "application/json";
  } break;
  case REQ_RESET: {
    BlynkState::set(MODE_RESET_CONFIG);
    content = R"json({"status":"ok","msg":"Configuration reset"})json";
    content_type = "application/json";
  } break;
  case REQ_REBOOT: {
    restartMCU();
  } break;
  }

              client.println("HTTP/1.1 " + responce);
              client.println("Content-type:" + content_type);
              client.println();
              client.println(content);
              // break out of the while loop:
              break;
            } else {      // if you got a newline, then clear currentLine:
              currentLine = "";
            }
          } else if (c != '\r') {  // if you got anything else but a carriage return character,
            currentLine += c;      // add it to the end of the currentLine
          }
  
          if (currentLine.indexOf("GET /board_info.json") >= 0) {
            req = REQ_BOARD_INFO;
          } else if (currentLine.indexOf("GET /wifi_scan.json") >= 0) {
            req = REQ_SCAN_WIFI;
          } else if (currentLine.indexOf(" /config") >= 0) {
            req = REQ_CONFIG;
            int idx = currentLine.indexOf("?");
            config_line = "&" + currentLine.substring(idx+1, currentLine.lastIndexOf(' ')) + "&";
          } else if (currentLine.indexOf(" /reset") >= 0) {
            req = REQ_RESET;
          } else if (currentLine.indexOf(" /reboot") >= 0) {
            req = REQ_REBOOT;
          }
        }
      }
      client.stop();
    }
  }
}

void enterConnectNet() {
  BlynkState::set(MODE_CONNECTING_NET);
  DEBUG_PRINT(String("Connecting to WiFi: ") + configStore.wifiSSID);

  WiFi.end();

  char ssidBuff[64];
  getWiFiName(ssidBuff, sizeof(ssidBuff));
  String hostname(ssidBuff);
  hostname.replace(" ", "-");
  WiFi.setHostname(hostname.c_str());

  if (configStore.getFlag(CONFIG_FLAG_STATIC_IP)) {
        WiFi.config(configStore.staticIP,
                    configStore.staticDNS,
                    configStore.staticGW,
                    configStore.staticMask);
  }

  WiFi.begin(configStore.wifiSSID, configStore.wifiPass);

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

    BlynkState::set(MODE_CONNECTING_CLOUD);
  } else {
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
    BlynkState::set(MODE_WAIT_CONFIG);
  } else if (Blynk.connected()) {
    BlynkState::set(MODE_RUNNING);

    if (!configStore.getFlag(CONFIG_FLAG_VALID)) {
      configStore.last_error = BLYNK_PROV_ERR_NONE;
      configStore.setFlag(CONFIG_FLAG_VALID, true);
      config_save();
    }
  } else {
    config_set_last_error(BLYNK_PROV_ERR_CLOUD);
    BlynkState::set(MODE_ERROR);
  }
}

void enterSwitchToSTA() {
  BlynkState::set(MODE_SWITCH_TO_STA);

  DEBUG_PRINT("Switching to STA...");

  delay(1000);

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

