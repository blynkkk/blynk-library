/**
 * @file       BlynkSimpleEthernet2.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jan 2016
 * @brief
 *
 */

#ifndef BlynkSimpleEthernet2_h
#define BlynkSimpleEthernet2_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION "W5500"
#endif

#include <Ethernet2.h>
#include <EthernetClient.h>
#include <Adapters/BlynkEthernet.h>

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_BLYNK)
  static EthernetClient _blynkEthernetClient;
  static BlynkArduinoClient _blynkTransport(_blynkEthernetClient);
  BlynkEthernet Blynk(_blynkTransport);
#else
  extern BlynkEthernet Blynk;
#endif

#include <BlynkWidgets.h>

#endif
