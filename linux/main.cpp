/**
 * @file       main.cpp
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Mar 2015
 * @brief
 */

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

