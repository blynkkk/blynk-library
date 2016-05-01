/**
 * @file       BlynkDebug.cpp
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief      Debug utilities for Arduino
 */

#include <Blynk/BlynkDebug.h>
#include <Arduino.h>

size_t BlynkFreeRam()
{
#if defined(__AVR__)
    extern int __heap_start, *__brkval;
    int v;
    return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval);
#else
    return 0;
#endif
}

void BlynkReset()
{
    void(*resetFunc)(void) = 0;
    resetFunc();
    for(;;); // To make compiler happy
}

void BlynkFatal()
{
    delay(10000L);
    BlynkReset();
}
