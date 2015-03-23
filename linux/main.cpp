//#define BLYNK_DEBUG
#define BLYNK_PRINT stdout
#include <BlynkSocket.h>

static BlynkTransportSocket _blynkTransport;
BlynkSocket Blynk(_blynkTransport);

BLYNK_WRITE(1)
{
  BLYNK_LOG("Got a value: %s", param[0].asStr());
}

int main()
{
    Blynk.begin("YourAuthToken");
    while(true) {
        Blynk.run();
    }
    return 0;
}

