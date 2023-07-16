#ifndef BlynkStreamNull_h
#define BlynkStreamNull_h

class NullStream
    : public Stream
{
public:

    /*
     * Writing
     */

    virtual size_t write(uint8_t) override {
        return 1;
    }

    virtual size_t write(const uint8_t* buffer, size_t size) override {
        (void)buffer;
        (void)size;
        return size;
    }

    virtual int availableForWrite() {
        return 4096;
    }

    virtual void flush() {}

    /*
     * Reading
     */

    virtual int available() override {
        return 0;
    }

    virtual int read() override {
        return -1;
    }

    virtual int peek() override {
        return -1;
    }

    virtual size_t readBytes(char* buffer, size_t len) {
        (void)buffer;
        (void)len;
        return 0;
    }

    virtual int read(uint8_t* buffer, size_t len) {
        (void)buffer;
        (void)len;
        return 0;
    }
};

#if !defined(NO_GLOBAL_INSTANCES) && !defined(NO_GLOBAL_NULLSTREAM)
static NullStream NullStream;
#endif

#endif
