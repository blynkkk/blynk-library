/*************************************************************
  Download latest Blynk library here:
    https://github.com/blynkkk/blynk-library/releases/latest
  Blynk is a platform with iOS and Android apps to control
  Arduino, Raspberry Pi and the likes over the Internet.
  You can easily build graphic interfaces for all your
  projects by simply dragging and dropping widgets.
    Downloads, docs, tutorials: http://www.blynk.cc
    Sketch generator:           http://examples.blynk.cc
    Blynk community:            http://community.blynk.cc
    Follow us:                  http://www.fb.com/blynkapp
                                http://twitter.com/blynk_app
  Blynk library is licensed under MIT license
  This example code is in public domain.
 *************************************************************
  This example runs directly on ESP8266 chip.
  Note: This requires ESP8266 support package:
    https://github.com/esp8266/Arduino
  Please be sure to select the right ESP8266 module
  in the Tools -> Board menu!
 *************************************************************/

#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <BlynkSimpleEsp8266.h>
#include <TimeLib.h>
#include <WidgetRTC.h>

char auth[] = "YourAuthToken";
char ssid[] = "YourNetworkName";
char pass[] = "YourPassword";
char OTApass[] = "YourOTAPassword";  // any password for OTA security access

int RSSIvalue = 0;
float Voltage=0.00f;
unsigned long Timer_5000 = 0;
byte mac[6];


#define PIN_RSSI V2
#define PIN_UBAT V3
#define LED      2

ADC_MODE (ADC_VCC);
WidgetTerminal terminal(V1);
WidgetRTC rtc;

void myTimerEvent()
{
  Blynk.virtualWrite(PIN_RSSI, RSSIvalue); 
  Blynk.virtualWrite(PIN_UBAT, Voltage);  
}

BLYNK_CONNECTED() {
  // Synchronize time on connection
  rtc.begin();
}


void setup()
{
  
  Serial.begin(9600);    // can be change for faster, ex. 115200, or 921600 
  Serial.println("Blynk.begin");  
  terminal.println("   -----> Blynk.begin");

  Blynk_init();
  if (WiFi.status() != WL_CONNECTED) ESP.restart();
  
  rtc.begin();
  terminal.print(currentTime());
  terminal.println(" Connected to server");
  terminal.println(WiFi.localIP());
  WiFi.macAddress(mac);  
  terminal.print("MAC: ");
  terminal.print(mac[5],HEX);
  terminal.print(":");
  terminal.print(mac[4],HEX);
  terminal.print(":");
  terminal.print(mac[3],HEX);
  terminal.print(":");
  terminal.print(mac[2],HEX);
  terminal.print(":");
  terminal.print(mac[1],HEX);
  terminal.print(":");
  terminal.println(mac[0],HEX);

  pinMode(LED,  OUTPUT);  
  digitalWrite(LED, HIGH);
  
  Serial.println("Blynk Started OK...");

  OTA_init();    

  Voltage = (ESP.getVcc()-210.0f)/1000.0f;           //  [-210.0f] - correction for real measurement with voltmeter

  myTimerEvent();                                    // send data to server
  terminal.flush();

  Timer_5000 = millis();

}

String currentTime() {
  char str[8];
  sprintf( str, "%02d:%02d:%02d", hour(), minute(), second() );
  return str;
}

void Blynk_init() {
  int conB_count = 5;
  while ((!Blynk.begin_t(auth, ssid, pass, IPAddress(172,18,1,111), 8080, 30000)) && (conB_count>0)) --conB_count;  // Should change IPAddress(172,18,1,111) to address of your server
}

void OTA_init() {
  ArduinoOTA.setHostname("MyESP");         //Set OTA host name (optional)
  ArduinoOTA.setPassword(OTApass);         //Set OTA host password (optional)
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }
    Serial.println("Start updating " + type);
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  //  terminal.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) {
      Serial.println("Auth Failed");
    } else if (error == OTA_BEGIN_ERROR) {
      Serial.println("Begin Failed");
    } else if (error == OTA_CONNECT_ERROR) {
      Serial.println("Connect Failed");
    } else if (error == OTA_RECEIVE_ERROR) {
      Serial.println("Receive Failed");
    } else if (error == OTA_END_ERROR) {
      Serial.println("End Failed");
    }
  });
  ArduinoOTA.begin();

  terminal.print(currentTime());
  Serial.println("OTA Started OK...");
  terminal.println(" OTA Started OK...");
}

void connectBlynk() {
  Serial.print(currentTime());
  Serial.println(F("  Woke up!"));    
  Serial.flush();
}

void loop()
{
  
  ArduinoOTA.handle();
  Blynk.run();
  if ((Timer_5000+5000) < millis()) {
    ESPsleep();                                   // ESP will run 5 sec for detect OTA request 
    Timer_5000 = millis();    
  }
  
}

void ESPsleep() {

  terminal.print(currentTime());
  terminal.println("  Sleeping 90 sec...");
  Serial.println("Sleeping 90 sec...");
  terminal.flush();
  delay(100);
  Blynk.disconnect();
  WiFi.mode(WIFI_OFF);                  // you must turn the modem off; using disconnect won't work
  digitalWrite(LED, HIGH);              // turn the LED off so they know the CPU isn't running
  extern os_timer_t *timer_list;
  timer_list = nullptr;                 // stop (but don't disable) the 4 OS timers
  wifi_fpm_set_sleep_type(LIGHT_SLEEP_T);
  gpio_pin_wakeup_enable(GPIO_ID_PIN(0), GPIO_PIN_INTR_LOLEVEL);  // GPIO wakeup (optional)
  wifi_fpm_set_wakeup_cb(connectBlynk); // set wakeup callback
  wifi_fpm_open();
  wifi_fpm_do_sleep(90E6);              // Sleep range = 10000 ~ 268,435,454 uS (0xFFFFFFE, 2^28-1)
  delay(90e3 + 1);                      // delay needs to be 1 mS longer than sleep or it only goes into Modem Sleep

  digitalWrite(LED, LOW);      // LED twice flash for sleep off
  delay(50);
  digitalWrite(LED, HIGH);
  delay(50);
  digitalWrite(LED, LOW);
  delay(50);
  digitalWrite(LED, HIGH);

  Blynk_init();
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    digitalWrite(LED, LOW);
    delay(250);
    digitalWrite(LED, HIGH);
    delay(100);
    ESP.restart();
  } else {
    rtc.begin();    
    terminal.print(currentTime());
    terminal.print(F("  Wokeup with "));
    terminal.print("WiFi ");
    terminal.print(6-conB_count);
    terminal.println(" Blynk atmpts");
    terminal.flush();
    ArduinoOTA.begin();
    Serial.println("Connection restored OK...");
    terminal.print(currentTime());
    terminal.println("  Connection restored. OTA available.");
    terminal.print(currentTime());
    terminal.print("  IP: ");
    terminal.println(WiFi.localIP());
    terminal.flush();
  }  
  Serial.println("Data sending started...");
  terminal.print(currentTime());
  terminal.println("  Data sending started...");
  terminal.flush();
  digitalWrite(LED, LOW);
  delay(25);
  digitalWrite(LED, HIGH);
  Voltage = (ESP.getVcc()-210.0f)/1000.0f;
  RSSIvalue = WiFi.RSSI();
  // And other your measurements
  myTimerEvent();                                 // Send datas
  terminal.print(currentTime());
  terminal.println("  Data Sended OK...");
  terminal.flush();
  Serial.println("Data Sended OK...");
}
