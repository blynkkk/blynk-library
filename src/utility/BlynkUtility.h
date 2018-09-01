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


template <class T, class T2>
T BlynkMathMap(T x, T2 in_min, T2 in_max, T2 out_min, T2 out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

template <class T, class T2>
T BlynkMathClamp(T val, T2 low, T2 high)
{
  return (val < low) ? low : ((val > high) ? high : val);
}


template <unsigned WSIZE, typename T>
void BlynkAverageSample (T& avg, const T& input) {
    avg -= avg/WSIZE;
    const T add = input/WSIZE;
    // Fix for shorter delays
    if (add > 0)
      avg += add;
    else
      avg -= 1;
}

class BlynkHelperAutoInc {
public:
    BlynkHelperAutoInc(uint8_t& counter) : c(counter) { ++c; }
    ~BlynkHelperAutoInc() { --c; }
private:
    uint8_t& c;
};

#define BlynkBitSet(value, bit)   ((value) |= (1UL << (bit)))
#define BlynkBitClear(value, bit) ((value) &= ~(1UL << (bit)))
#define BlynkBitRead(value, bit)  (((value) >> (bit)) & 0x01)
#define BlynkBitWrite(value, bit, bitvalue) (bitvalue ? bitSet(value, bit) : bitClear(value, bit))

#endif
