/**
 * @file       BlynkSimpleEnergiaEthernet.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Oct 2016
 * @brief
 *
 */

#ifndef BlynkSimpleEnergiaEthernet_h
#define BlynkSimpleEnergiaEthernet_h

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
