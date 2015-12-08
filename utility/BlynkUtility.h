/**
 * @file       BlynkUtility.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jun 2015
 * @brief      Utility functions
 *
 */

#ifndef BlynkUtility_h
#define BlynkUtility_h

template<class T>
const T& BlynkMin(const T& a, const T& b)
{
    return (b < a) ? b : a;
}

template<class T>
const T& BlynkMax(const T& a, const T& b)
{
    return (b < a) ? a : b;
}

template <unsigned WSIZE, typename T>
void BlynkAverageSample (T& avg, const T& input) {
    avg -= avg/WSIZE;
    const T add = input/WSIZE;
    // Fix for shorter delays
    avg += (add > 0) ? add : -1;
}

#endif
