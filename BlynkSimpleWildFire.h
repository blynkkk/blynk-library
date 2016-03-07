/**
 * @file       BlynkSimpleWildFire.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Mar 2015
 * @brief
 *
 */

#ifndef BlynkSimpleWildFire_h
#define BlynkSimpleWildFire_h

#include <Adapters/BlynkWildFire.h>

WildFire_CC3000 cc3000;
static BlynkTransportWildFire _blynkTransport(cc3000);
BlynkWildFire Blynk(cc3000, _blynkTransport);

#include <BlynkWidgets.h>

#endif
