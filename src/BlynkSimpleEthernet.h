/**
 * @file       BlynkSimpleEthernet.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief
 *
 */

#ifndef BlynkSimpleEthernet_h
#define BlynkSimpleEthernet_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION "W5100"
#endif

#include <Ethernet.h>
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
