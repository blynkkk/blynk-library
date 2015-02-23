/**
 * @file       BlynkParam.cpp
 * @author     Volodymyr Shymanskyy
 * @date       Feb 2015
 * @brief      Container for handler parameters
 *
 */

#include <Blynk/BlynkParam.h>

BlynkParam::iterator BlynkParam::operator[](int index) const
{
    const iterator e = end();
    for (iterator it = begin(); it < e; ++it) {
        if (!index--) {
            return it;
        }
    }
    return iterator::invalid();
}

BlynkParam::iterator BlynkParam::operator[](const char* key) const
{
    const iterator e = end();
    for (iterator it = begin(); it < e; ++it) {
        if (!strcmp(it.asStr(), key)) {
            return ++it;
        }
        ++it;
        if (it >= e) break;
    }
    return iterator::invalid();
}


void BlynkParam::add(const void* b, size_t l)
{
    if (len + l > buff_size)
        return;
    memcpy(buff+len, b, l);
    len += l;
}

void BlynkParam::add(const char* str)
{
    add(str, strlen(str)+1);
}

void BlynkParam::add(int value)
{
    char str[2 + 8 * sizeof(value)];
    itoa(value, str, 10);
    add(str);
}

void BlynkParam::add(unsigned int value)
{
    char str[1 + 8 * sizeof(value)];
    utoa(value, str, 10);
    add(str);
}

void BlynkParam::add(long value)
{
    char str[2 + 8 * sizeof(value)];
    ltoa(value, str, 10);
    add(str);
}

void BlynkParam::add(unsigned long value)
{
    char str[1 + 8 * sizeof(value)];
    ultoa(value, str, 10);
    add(str);
}

void BlynkParam::add(float value)
{
    char str[33];
    dtostrf(value, 5, 3, str);
    add(str);
}

void BlynkParam::add(double value)
{
    char str[33];
    dtostrf(value, 5, 3, str);
    add(str);
}
