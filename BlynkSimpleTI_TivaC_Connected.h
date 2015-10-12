/**
 * @file       BlynkSimpleEthernet.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief
 *
 */

#ifndef BlynkSimpleTivaC_Connected_h
#define BlynkSimpleTivaC_Connected_h

#ifndef BLYNK_INFO_DEVICE
#define BLYNK_INFO_DEVICE  "TI Tiva C Connected"
#endif

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION "TM4C12x"
#endif

#include <Ethernet.h>
#include <EthernetClient.h>
#include <Adapters/BlynkEthernet.h>

static EthernetClient _blynkEthernetClient;
static BlynkArduinoClient _blynkTransport(_blynkEthernetClient);
BlynkEthernet Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#endif
