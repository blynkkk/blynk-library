#ifndef BlynkParam_h
#define BlynkParam_h

#include <string.h>
#include <stdlib.h>

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

        bool operator != (const iterator& it) const { return ptr != it.ptr; }
        bool operator == (const iterator& it) const { return ptr == it.ptr; }
        bool operator <  (const iterator& it) const { return ptr <  it.ptr; }

        iterator& operator ++() {
            if (isValid()) {
                ptr += strlen(ptr)+1;
            }
            return *this;
        }
    private:
        const char* ptr;
    };

public:
    BlynkParam(void* addr, int length)
        : buff((char*)addr), len(length)
    {
        // 2 last bytes should be \0
        BLYNK_ASSERT(buff[len-1] == '\0');
        BLYNK_ASSERT(buff[len-2] == '\0');
    }

    iterator begin() const { return iterator(buff); }
    iterator end() const   { return iterator(buff+len-1); }

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
            if (!(it < e)) break;
        }
        return iterator::invalid();
    }

    void* getBuffer() const { return buff; }
    size_t getLength() const { return len; }

private:
    char*	buff;
    size_t	len;
};

#endif

/*
#include "stdio.h"

char msg[] = "abc\0" "123\0" "def\0" "456\0";

int main() {
    printf("size %lu\n", sizeof(msg));
    BlynkParam buff(msg, sizeof(msg));

    for (BlynkParam::Iterator it = buff.begin(); it != buff.end(); ++it) {
        printf("it: %s\n", it.asStr());
    }

    for (int i = 0; i < 6; i++) {
        BlynkParam::Iterator it = buff[i];
        printf("%d: %s\n", i, it.asStr());
    }

    printf("abc: %d\n", buff["abc"].asInt());
    printf("def: %d\n", buff["def"].asInt());
    printf("123: %s\n", buff["123"]);

    return 0;
}
*/
