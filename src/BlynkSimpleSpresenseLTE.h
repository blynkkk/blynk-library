/**
 * @file       BlynkSimpleSpresenseLTE.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       May 2020
 * @brief
 *
 */

#ifndef BlynkSimpleSpresenseLTE_h
#define BlynkSimpleSpresenseLTE_h

#include <Adapters/BlynkSpresenseLTE.h>

static BlynkArduinoClient _blynkTransport;
BlynkSIM Blynk(_blynkTransport);

#include <BlynkWidgets.h>

#endif
