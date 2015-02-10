#ifndef BlynkSimpleEthernet_h
#define BlynkSimpleEthernet_h

#include <BlynkEthernet.h>
#include <Ethernet.h>
#include <EthernetClient.h>

static EthernetClient _blynkEthernetClient;
static BlynkArduinoClient _blynkTransport(_blynkEthernetClient);
BlynkEthernet Blynk(_blynkTransport);

#endif
