/**
 * @file       BlynkSimpleEsp32_SSL.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Dec 2017
 * @brief
 *
 */

#ifndef BlynkSimpleEsp32_SSL_h
#define BlynkSimpleEsp32_SSL_h

#ifndef ESP32
#error This code is intended to run on the ESP32 platform! Please check your Tools->Board setting.
#endif

#define BLYNK_DEFAULT_ROOT_CA                                            \
    "-----BEGIN CERTIFICATE-----\n"                                      \
    "MIID5TCCAs2gAwIBAgIJAIHSnb+cv4ECMA0GCSqGSIb3DQEBCwUAMIGIMQswCQYD\n" \
    "VQQGEwJVQTENMAsGA1UECAwES3lpdjENMAsGA1UEBwwES3lpdjELMAkGA1UECgwC\n" \
    "SVQxEzARBgNVBAsMCkJseW5rIEluYy4xGDAWBgNVBAMMD2JseW5rLWNsb3VkLmNv\n" \
    "bTEfMB0GCSqGSIb3DQEJARYQZG1pdHJpeUBibHluay5jYzAeFw0xNjAzMTcxMTU4\n" \
    "MDdaFw0yMTAzMTYxMTU4MDdaMIGIMQswCQYDVQQGEwJVQTENMAsGA1UECAwES3lp\n" \
    "djENMAsGA1UEBwwES3lpdjELMAkGA1UECgwCSVQxEzARBgNVBAsMCkJseW5rIElu\n" \
    "Yy4xGDAWBgNVBAMMD2JseW5rLWNsb3VkLmNvbTEfMB0GCSqGSIb3DQEJARYQZG1p\n" \
    "dHJpeUBibHluay5jYzCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALso\n" \
    "bhbXQuNlzYBFa9h9pd69n43yrGTL4Ba6k5Q1zDwY9HQbMdfC5ZfnCkqT7Zf+R5MO\n" \
    "RW0Q9nLsFNLJkwKnluRCYGyUES8NAmDLQBbZoVc8mv9K3mIgAQvGyY2LmKak5GSI\n" \
    "V0PC3x+iN03xU2774+Zi7DaQd7vTl/9RGk8McyHe/s5Ikbe14bzWcY9ZV4PKgCck\n" \
    "p1chbmLhSfGbT3v64sL8ZbIppQk57/JgsZMrVpjExvxQPZuJfWbtoypPfpYO+O8l\n" \
    "1szaMlTEPIZVMoYi9uE+DnOlhzJFn6Ac4FMrDzJXzMmCweSX3IxguvXALeKhUHQJ\n" \
    "+VP3G6Q3pkZRVKz+5XsCAwEAAaNQME4wHQYDVR0OBBYEFJtqtI62Io66cZgiTR5L\n" \
    "A5Tl5m+xMB8GA1UdIwQYMBaAFJtqtI62Io66cZgiTR5LA5Tl5m+xMAwGA1UdEwQF\n" \
    "MAMBAf8wDQYJKoZIhvcNAQELBQADggEBAKphjtEOGs7oC3S87+AUgIw4gFNOuv+L\n" \
    "C98/l47OD6WtsqJKvCZ1lmKxY5aIro9FBPk8ktCOsbwEjE+nyr5wul+6CLFr+rnv\n" \
    "7OHYGwLpjoz+rZgYJiQ61E1m0AZ4y9Fyd+D90HW6247vrBXyEiUXOhN/oDDVfDQA\n" \
    "eqmNBx1OqWel81D3tA7zPMA7vUItyWcFIXNjOCP+POy7TMxZuhuPMh5bVu+/cthl\n" \
    "/Q9u/Z2lKl4CWV0Ivt2BtlN6iefva0e2AP/As+gfwjxrb0t11zSILLNJ+nxRIwg+\n" \
    "k4MGb1zihKbIXUzsjslONK4FY5rlQUSwKJgEAVF0ClxB4g6dECm0ckc=\n"         \
    "-----END CERTIFICATE-----\n"

#include <BlynkApiArduino.h>
#include <Blynk/BlynkProtocol.h>
#include <Adapters/BlynkArduinoClient.h>
#include <WiFiClientSecure.h>

template <typename Client>
class BlynkArduinoClientSecure
    : public BlynkArduinoClientGen<Client>
{
public:
    BlynkArduinoClientSecure(Client& client)
        : BlynkArduinoClientGen<Client>(client)
        , caCert(NULL)
    {}

    void setRootCA(const char* fp) { caCert = fp; }

    bool connect() {
        this->client->setCACert(caCert);
        if (BlynkArduinoClientGen<Client>::connect()) {
          BLYNK_LOG1(BLYNK_F("Certificate OK"));
          return true;
        }
        return false;
    }

private:
    const char* caCert;
};

template <typename Transport>
class BlynkWifi
    : public BlynkProtocol<Transport>
{
    typedef BlynkProtocol<Transport> Base;
public:
    BlynkWifi(Transport& transp)
        : Base(transp)
    {}

    void connectWiFi(const char* ssid, const char* pass)
    {
        BLYNK_LOG2(BLYNK_F("Connecting to "), ssid);
        WiFi.mode(WIFI_STA);
        if (pass && strlen(pass)) {
            WiFi.begin(ssid, pass);
        } else {
            WiFi.begin(ssid);
        }
        while (WiFi.status() != WL_CONNECTED) {
            BlynkDelay(500);
        }
        BLYNK_LOG1(BLYNK_F("Connected to WiFi"));

        IPAddress myip = WiFi.localIP();
        BLYNK_LOG_IP("IP: ", myip);
    }

    void config(const char* auth,
                const char* domain = BLYNK_DEFAULT_DOMAIN,
                uint16_t    port   = BLYNK_DEFAULT_PORT_SSL,
                const char* root_ca = BLYNK_DEFAULT_ROOT_CA)
    {
        Base::begin(auth);
        this->conn.begin(domain, port);
        this->conn.setRootCA(root_ca);
    }

    void config(const char* auth,
                IPAddress   ip,
                uint16_t    port = BLYNK_DEFAULT_PORT_SSL,
                const char* root_ca = BLYNK_DEFAULT_ROOT_CA)
    {
        Base::begin(auth);
        this->conn.begin(ip, port);
        this->conn.setRootCA(root_ca);
    }

    void begin(const char* auth,
               const char* ssid,
               const char* pass,
               const char* domain = BLYNK_DEFAULT_DOMAIN,
               uint16_t    port   = BLYNK_DEFAULT_PORT_SSL,
               const char* root_ca = BLYNK_DEFAULT_ROOT_CA)
    {
        connectWiFi(ssid, pass);
        config(auth, domain, port, root_ca);
        while(this->connect() != true) {}
    }

    void begin(const char* auth,
               const char* ssid,
               const char* pass,
               IPAddress   ip,
               uint16_t    port   = BLYNK_DEFAULT_PORT_SSL,
               const char* root_ca = BLYNK_DEFAULT_ROOT_CA)
    {
        connectWiFi(ssid, pass);
        config(auth, ip, port, root_ca);
        while(this->connect() != true) {}
    }

};

static WiFiClientSecure _blynkWifiClient;
static BlynkArduinoClientSecure<WiFiClientSecure> _blynkTransport(_blynkWifiClient);
BlynkWifi<BlynkArduinoClientSecure<WiFiClientSecure> > Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#endif
