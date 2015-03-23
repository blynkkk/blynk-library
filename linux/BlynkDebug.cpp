/**
 * @file       BlynkDebug.cpp
 * @author     Volodymyr Shymanskyy
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
}

void BlynkFatal()
{
	exit(1);
	for(;;); // To make compiler happy
}
