/**
 * @file       BlynkParam.h
 * @author     Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief      Container for handler parameters
 *
 */

#ifndef BlynkParam_h
#define BlynkParam_h

#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include <Blynk/BlynkConfig.h>
#include <Blynk/BlynkDebug.h>

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
        bool isValid() const            { return ptr != NULL; }

        bool operator < (const iterator& it) const { return ptr < it.ptr; }
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
    BlynkParam(void* addr, size_t length, size_t buff_size)
        : buff((char*)addr), len(length), buff_size(buff_size)
    {
    }

    iterator begin() const { return iterator(buff); }
    iterator end() const   { return iterator(buff+len); }

    iterator operator[](int index) const {
        const iterator e = end();
        for (iterator it = begin(); it < e; ++it) {
            if (!index--) {
                return it;
            }
        }
        return iterator::invalid();
    }

    iterator operator[](const char* key) const {
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

    uint8_t* getBuffer() const { return (uint8_t*)buff; }
    size_t getLength() const { return len; }

    // Modification

    void add(int value) {
        char str[2 + 8 * sizeof(value)];
        itoa(value, str, 10);
        add(str);
    }

    void add(unsigned int value) {
        char str[1 + 8 * sizeof(value)];
        utoa(value, str, 10);
        add(str);
    }

    void add(long value) {
        char str[2 + 8 * sizeof(value)];
        ltoa(value, str, 10);
        add(str);
    }

    void add(unsigned long value) {
        char str[1 + 8 * sizeof(value)];
        ultoa(value, str, 10);
        add(str);
    }

    void add(float value) {
        char str[33];
        dtostrf(value, 5, 3, str);
        add(str);
    }

    void add(double value) {
        char str[33];
        dtostrf(value, 5, 3, str);
        add(str);
    }

    void add(const char* str) {
        add(str, strlen(str)+1);
    }

    template <typename TK, typename TV>
    void add_key(const TK& key, const TV& val) {
        add(key);
        add(val);
    }

    void add(const void* b, size_t l) {
        if (len + l > buff_size)
            return;
        memcpy(buff+len, b, l);
        len += l;
    }

private:
    char*	buff;
    size_t	len;
    size_t	buff_size;
};

#endif
