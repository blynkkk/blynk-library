/*************************************************************
  Blynk is a platform with iOS and Android apps to control
  ESP32, Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build mobile and web interfaces for any
  projects by simply dragging and dropping widgets.

    Downloads, docs, tutorials: https://www.blynk.io
    Sketch generator:           https://examples.blynk.cc
    Blynk community:            https://community.blynk.cc
    Follow us:                  https://www.fb.com/blynkapp
                                https://twitter.com/blynk_app

  This example code is in public domain.

 *************************************************************
  App dashboard setup:
    Value Display widget on V2

  Attention!
    1. Using your phone:
        Disable PIN code on the SIM card
        Check your ballance
        Check that APN,User,Pass are correct and you have internet
    2. Ensure the sim card is correctly inserted into the module
    3. Provide a good, stable power supply (up to 2A)
       (4.0-4.2V or 5V according to your module documentation)
    4. Provide good serial connection
       (Hardware Serial is recommended)
    5. Check if GSM antenna is attached

 *************************************************************/

/* Fill in information from Blynk Device Info here */
//#define BLYNK_TEMPLATE_ID           "TMPxxxxxx"
//#define BLYNK_TEMPLATE_NAME         "Device"
//#define BLYNK_AUTH_TOKEN            "YourAuthToken"

// APN data
#define GPRS_APN       "YourAPN"    // Replace your GPRS APN
#define GPRS_USER      ""           // Replace with your GPRS user
#define GPRS_PASSWORD  ""           // Replace with your GPRS password

// Uncomment this to use HTTPS
//#define USE_HTTPS

// Set Serial to which GSM module is connected
Stream* stream = &Serial1;

// Blynk cloud server
const char* host = "blynk.cloud";

bool gprsInit();
bool gprsConnect();
bool gprsDisconnect();

void setup()
{
  Serial.begin(9600);
  delay(10);
  Serial.println();
  Serial.println();

  // Setup GPRS module baud rate
  Serial1.begin(115200);
  delay(3000);

  gprsInit();
}

void loop() {
  gprsConnect();

  String response;

  unsigned long value = millis();

  // Send value to the cloud
  // similar to Blynk.virtualWrite()

  Serial.print("Sending value: ");
  Serial.println(value);

  if (httpRequest("GET", String("/external/api/update?token=") + BLYNK_AUTH_TOKEN + "&pin=V2&value=" + value, "", response)) {
    if (response.length() != 0) {
      Serial.print("WARNING: ");
      Serial.println(response);
    }
  }

  // Read the value back
  // similar to Blynk.syncVirtual()

  Serial.println("Reading value");

  if (httpRequest("GET", String("/external/api/get?token=") + BLYNK_AUTH_TOKEN + "&pin=V2", "", response)) {
    Serial.print("Value from server: ");
    Serial.println(response);
  }

  // Set Property
  Serial.println("Setting property");

  if (httpRequest("GET", String("/external/api/update/property?token=") + BLYNK_AUTH_TOKEN + "&pin=V2&label=" + value, "", response)) {
    if (response.length() != 0) {
      Serial.print("WARNING: ");
      Serial.println(response);
    }
  }

  // For more HTTP API, see https://docs.blynk.io/en/blynk.cloud/https-api-overview

  // Disconnect and wait
  gprsDisconnect();

  Serial.println("Waiting 1 minute...");
  Serial.println();
  Serial.println();
  delay(60000L);
}

/**************************************************************
 * AT commands stuff
 **************************************************************/

typedef const __FlashStringHelper* GsmConstStr;

void sendAT(const String& cmd) {
  stream->print("AT");
  stream->println(cmd);
}

uint8_t waitResponse(uint32_t timeout, GsmConstStr r1,
                     GsmConstStr r2 = NULL, GsmConstStr r3 = NULL)
{
  String data;
  data.reserve(64);
  int index = 0;
  for (unsigned long start = millis(); millis() - start < timeout; ) {
    while (stream->available() > 0) {
      int c = stream->read();
      if (c < 0) continue;
      data += (char)c;
      if (data.indexOf(r1) >= 0) {
        index = 1;
        goto finish;
      } else if (r2 && data.indexOf(r2) >= 0) {
        index = 2;
        goto finish;
      } else if (r3 && data.indexOf(r3) >= 0) {
        index = 3;
        goto finish;
      }
    }
  }
finish:
  return index;
}


uint8_t waitResponse(GsmConstStr r1,
                     GsmConstStr r2 = NULL, GsmConstStr r3 = NULL)
{
  return waitResponse(1000, r1, r2, r3);
}

uint8_t waitOK_ERROR(uint32_t timeout = 1000) {
  return waitResponse(timeout, F("OK\r\n"), F("ERROR\r\n"));
}

bool gprsInit()
{
  sendAT(F("E0"));
  waitOK_ERROR();

  sendAT(F("+SAPBR=3,1,\"Contype\",\"GPRS\""));
  waitOK_ERROR();

  sendAT(F("+SAPBR=3,1,\"APN\",\"" GPRS_APN "\""));
  waitOK_ERROR();

#ifdef GPRS_USER
  sendAT(F("+SAPBR=3,1,\"USER\",\"" GPRS_USER "\""));
  waitOK_ERROR();
#endif
#ifdef GPRS_PASSWORD
  sendAT(F("+SAPBR=3,1,\"PWD\",\"" GPRS_PASSWORD "\""));
  waitOK_ERROR();
#endif

  sendAT(F("+CGDCONT=1,\"IP\",\"" GPRS_APN "\""));
  waitOK_ERROR();
  return true;
}

// Start the GSM connection
bool gprsConnect()
{
  Serial.println("Connecting to GSM...");

  sendAT(F("+CGACT=1,1"));
  waitOK_ERROR(60000L);

  // Open a GPRS context
  sendAT(F("+SAPBR=1,1"));
  waitOK_ERROR(85000L);
  // Query the GPRS context
  sendAT(F("+SAPBR=2,1"));
  if (waitOK_ERROR(30000L) != 1)
    return false;

  Serial.println("GSM connected");
  return true;
}

bool gprsDisconnect() {
  sendAT(F("+SAPBR=0,1"));
  if (waitOK_ERROR(30000L) != 1)
    return;

  sendAT(F("+CGACT=0"));
  waitOK_ERROR(60000L);

  Serial.println("GSM disconnected");
  return true;
}

int httpRequest(const String& method,
                const String& url,
                const String& request,
                String&       response)
{
  Serial.print(F("  Request: "));
  Serial.print(host);
  Serial.println(url);

  sendAT(F("+HTTPTERM"));
  waitOK_ERROR();

  sendAT(F("+HTTPINIT"));
  waitOK_ERROR();

  sendAT(F("+HTTPPARA=\"CID\",1"));
  waitOK_ERROR();

#ifdef USE_HTTPS
  sendAT(F("+HTTPSSL=1"));
  waitOK_ERROR();
  sendAT(String(F("+HTTPPARA=\"URL\",\"https://")) + host + url + "\"");
  waitOK_ERROR();
#else
  sendAT(String(F("+HTTPPARA=\"URL\",\"")) + host + url + "\"");
  waitOK_ERROR();
#endif

  if (request.length()) {
    sendAT(F("+HTTPPARA=\"CONTENT\",\"application/json\""));
    waitOK_ERROR();
    sendAT(String(F("+HTTPDATA=")) + request.length() + "," + 10000);
    waitResponse(F("DOWNLOAD\r\n"));
    stream->print(request);
    waitOK_ERROR();
  }

  if (method == "GET") {
    sendAT(F("+HTTPACTION=0"));
  } else if (method == "POST") {
    sendAT(F("+HTTPACTION=1"));
  } else if (method == "HEAD") {
    sendAT(F("+HTTPACTION=2"));
  } else if (method == "DELETE") {
    sendAT(F("+HTTPACTION=3"));
  }
  waitOK_ERROR();

  if (waitResponse(30000L, F("+HTTPACTION:")) != 1) {
    Serial.println("HTTPACTION Timeout");
    return false;
  }
  stream->readStringUntil(',');
  int code = stream->readStringUntil(',').toInt();
  size_t len = stream->readStringUntil('\n').toInt();

  if (code != 200) {
    Serial.print("Error code:");
    Serial.println(code);
    sendAT(F("+HTTPTERM"));
    waitOK_ERROR();
    return false;
  }

  response = "";

  if (len > 0) {
    response.reserve(len);

    sendAT(F("+HTTPREAD"));
    if (waitResponse(10000L, F("+HTTPREAD: ")) != 1) {
      Serial.println("HTTPREAD Timeout");
      return false;
    }
    len = stream->readStringUntil('\n').toInt();

    while (len--) {
      while (!stream->available()) {
        delay(1);
      }
      response += (char)(stream->read());
    }
    waitOK_ERROR();
  }

  sendAT(F("+HTTPTERM"));
  waitOK_ERROR();

  return true;
}

