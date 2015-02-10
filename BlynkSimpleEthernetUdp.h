#ifndef BlynkSimpleEthernet_h
#define BlynkSimpleEthernet_h

#include <Blynk/BlynkProtocol.h>
#include <BlynkArduinoStream.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

class BlynkEthernetUdp
    : public BlynkProtocol< BlynkArduinoStream<EthernetUDP> >
{
public:
    BlynkEthernetUdp(BlynkArduinoStream<EthernetUDP>& conn)
        : BlynkProtocol<BlynkArduinoStream<EthernetUDP> >(conn)
    {}

    void begin(const char* auth, uint16_t port = 8181) {
        this->authkey = auth;
        this->conn.begin(port);
    }

    void run(void)
    {
        if(!this->conn.connected()) {
            BLYNK_LOG("Reconnecting...");
            this->connect();
        }
        if (this->conn.available()) {
            this->processInput();
        }
    }
};

static EthernetUDP _blynkEthernetUdp;
static BlynkArduinoStream<EthernetUDP> _blynkTransport(_blynkEthernetUdp);
BlynkEthernetUdp Blynk(_blynkTransport);

#endif
