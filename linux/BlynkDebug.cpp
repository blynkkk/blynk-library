/**
 * @file       BlynkDebug.cpp
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Mar 2015
 * @brief      Debug utilities for Linux
 */

#include <Blynk/BlynkDebug.h>
#include <stdlib.h>

size_t BlynkFreeRam()
{
    // TODO
    return 0;
}

void BlynkReset()
{
    exit(1);
    for(;;); // To make compiler happy
}

void BlynkFatal()
{
    BlynkReset();
}
