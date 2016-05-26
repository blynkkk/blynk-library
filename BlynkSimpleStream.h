/**
 * @file       BlynkStream.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2016 Volodymyr Shymanskyy
 * @date       Mar 2016
 * @brief
 *
 */

#ifndef BlynkStream_h
#define BlynkStream_h

#ifndef BLYNK_INFO_CONNECTION
#define BLYNK_INFO_CONNECTION "Stream"
#endif

#include <Adapters/BlynkSerial.h>

BlynkTransportStream _blynkTransport;
BlynkStream Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#endif
