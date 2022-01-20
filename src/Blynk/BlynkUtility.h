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

template <class T, class T2>
T BlynkMathClampMap(T x, T2 in_min, T2 in_max, T2 out_min, T2 out_max)
{
  x = BlynkMathClamp(x, in_min, in_max);
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
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

static inline
uint32_t BlynkCRC32(const void* data, size_t length, uint32_t previousCrc32 = 0)
{
  const uint32_t Polynomial = 0xEDB88320;
  uint32_t crc = ~previousCrc32;
  unsigned char* current = (unsigned char*)data;
  while (length--) {
    crc ^= *current++;
    for (unsigned int j = 0; j < 8; j++) {
      crc = (crc >> 1) ^ (-int(crc & 1) & Polynomial);
    }
  }
  return ~crc;
}

static inline
bool BlynkStrMatch(const char* pat, const char* txt) {
    const long n = strlen(txt);
    const long m = strlen(pat);

    if (m == 0)
        return (n == 0);

    long i = 0, j = 0, index_txt = -1,
        index_pat = -1;

    while (i < n) {
        if (j < m && txt[i] == pat[j]) {
            i++;
            j++;
        } else if (j < m && pat[j] == '?') {
            i++;
            j++;
        } else if (j < m && pat[j] == '*') {
            index_txt = i;
            index_pat = j;
            j++;
        } else if (index_pat != -1) {
            j = index_pat + 1;
            i = index_txt + 1;
            index_txt++;
        } else {
            return false;
        }
    }

    while (j < m && pat[j] == '*') {
        j++;
    }

    if (j == m) {
        return true;
    }

    return false;
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
