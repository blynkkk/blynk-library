/**
 * @file       BlynkParam.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief      Container for handler parameters
 *
 */

#ifndef BlynkParam_h
#define BlynkParam_h

#include <string.h>
#include <stdlib.h>
#include <Blynk/BlynkConfig.h>
#include <Blynk/BlynkDebug.h>

#define BLYNK_PARAM_KV(k, v) k "\0" v "\0"

class BlynkParam
{
public:
    class iterator
    {
    public:
        iterator(char* c) : ptr(c) {}
        static iterator invalid() { return iterator(NULL); }

        operator const char* () const   { return asStr(); }
        operator int () const           { return asInt(); }
        const char* asStr() const       { return ptr; }
        int         asInt() const       { return atoi(ptr); }
        long        asLong() const      { return atol(ptr); }
        double      asDouble() const    { return atof(ptr); }
        bool isValid() const            { return ptr != NULL; }

        bool operator <  (const iterator& it) const { return ptr < it.ptr; }
        bool operator >= (const iterator& it) const { return ptr >= it.ptr; }

        iterator& operator ++() {
            ptr += strlen(ptr)+1;
            return *this;
        }
    private:
        const char* ptr;
    };

public:
    explicit
    BlynkParam(void* addr, size_t length)
        : buff((char*)addr), len(length), buff_size(length)
    {}

    explicit
    BlynkParam(void* addr, size_t length, size_t buffsize)
        : buff((char*)addr), len(length), buff_size(buffsize)
    {}

    const char* asStr() const       { return buff; }
    int         asInt() const       { return atoi(buff); }
    long        asLong() const      { return atol(buff); }
    double      asDouble() const    { return atof(buff); }

    iterator begin() const { return iterator(buff); }
    iterator end() const   { return iterator(buff+len); }

    iterator operator[](int index) const;
    iterator operator[](const char* key) const;

    uint8_t* getBuffer() const { return (uint8_t*)buff; }
    size_t getLength() const { return len; }

    // Modification
    void add(int value);
    void add(unsigned int value);
    void add(long value);
    void add(unsigned long value);
    void add(float value);
    void add(double value);
    void add(const char* str);
    void add(const void* b, size_t l);
#if defined(ARDUINO) || defined(SPARK)
    void add(const String& str);
#endif

    template <typename TV>
    void add_key(const char* key, const TV& val) {
        add(key);
        add(val);
    }

private:
    char*	buff;
    size_t	len;
    size_t	buff_size;
};

inline
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

inline
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

inline
void BlynkParam::add(const void* b, size_t l)
{
    if (len + l > buff_size)
        return;
    memcpy(buff+len, b, l);
    len += l;
}

inline
void BlynkParam::add(const char* str)
{
    add(str, strlen(str)+1);
}

#if defined(ARDUINO) || defined(SPARK)
inline
void BlynkParam::add(const String& str)
{
    size_t len = str.length()+1;
    char buff[len];
    str.toCharArray(buff, len);
    BlynkParam::add(buff, len);
}
#endif

#if defined(__AVR__)

    #include <stdlib.h>

	inline
    void BlynkParam::add(int value)
    {
        char str[2 + 8 * sizeof(value)];
        itoa(value, str, 10);
        add(str);
    }

	inline
    void BlynkParam::add(unsigned int value)
    {
        char str[1 + 8 * sizeof(value)];
        utoa(value, str, 10);
        add(str);
    }

	inline
    void BlynkParam::add(long value)
    {
        char str[2 + 8 * sizeof(value)];
        ltoa(value, str, 10);
        add(str);
    }

	inline
    void BlynkParam::add(unsigned long value)
    {
        char str[1 + 8 * sizeof(value)];
        ultoa(value, str, 10);
        add(str);
    }

	inline
    void BlynkParam::add(float value)
    {
        char str[33];
        dtostrf(value, 5, 3, str);
        add(str);
    }

	inline
    void BlynkParam::add(double value)
    {
        char str[33];
        dtostrf(value, 5, 3, str);
        add(str);
    }

#else

    #include <stdio.h>

	inline
    void BlynkParam::add(int value)
    {
        len += snprintf(buff+len, buff_size-len, "%i", value)+1;
    }

	inline
    void BlynkParam::add(unsigned int value)
    {
        len += snprintf(buff+len, buff_size-len, "%u", value)+1;
    }

	inline
    void BlynkParam::add(long value)
    {
        len += snprintf(buff+len, buff_size-len, "%li", value)+1;
    }

	inline
    void BlynkParam::add(unsigned long value)
    {
        len += snprintf(buff+len, buff_size-len, "%lu", value)+1;
    }

	inline
    void BlynkParam::add(float value)
    {
        len += snprintf(buff+len, buff_size-len, "%2.3f", value)+1;
    }

	inline
    void BlynkParam::add(double value)
    {
        len += snprintf(buff+len, buff_size-len, "%2.3f", value)+1;
    }

#endif


#endif
