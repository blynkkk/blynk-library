/**
 * @file       BlynkSimpleStream.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Mar 2016
 * @brief
 *
 */

#ifndef BlynkSimpleStream_h
#define BlynkSimpleStream_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION "Stream"
#endif

#include <Adapters/BlynkSerial.h>

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_BLYNK)
  BlynkTransportStream _blynkTransport;
  BlynkStream Blynk(_blynkTransport);
#else
  extern BlynkStream Blynk;
#endif

#include <BlynkWidgets.h>

#endif
