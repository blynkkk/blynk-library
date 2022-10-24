#ifndef BlynkStreamMulti_h
#define BlynkStreamMulti_h

#ifndef BLYNK_MULTI_STREAM_MAX_QTY
#define BLYNK_MULTI_STREAM_MAX_QTY 6
#endif

class MultiStream
    : public Stream
{
public:
    MultiStream() {}
    virtual ~MultiStream() {}

    bool addStream(Stream* stream) {
      if (stream && _streamsQty < BLYNK_MULTI_STREAM_MAX_QTY) {
        _streams[_streamsQty++] = stream;
        return true;
      }
      return false;
    }

    bool addStream(Stream& stream) {
      return addStream(&stream);
    }

    /*
     * Writing
     */

    virtual size_t write(uint8_t ch) {
      for (int i=0; i<_streamsQty; i++) {
        _streams[i]->write(ch);
      }
      return 1;
    }

    virtual void flush() {
      for (int i=0; i<_streamsQty; i++) {
        return _streams[i]->flush();
      }
    }

    /*
     * Reading
     */

    virtual int available() {
      for (int i=0; i<_streamsQty; i++) {
        int avail = _streams[i]->available();
        if (avail > 0) { return avail; }
      }
      return 0;
    }

    virtual int read() {
      for (int i=0; i<_streamsQty; i++) {
        if (_streams[i]->available()) {
          return _streams[i]->read();
        }
      }
      return -1;
    }

    virtual int peek() {
      for (int i=0; i<_streamsQty; i++) {
        if (_streams[i]->available()) {
          return _streams[i]->peek();
        }
      }
      return -1;
    }

private:
    Stream* _streams[BLYNK_MULTI_STREAM_MAX_QTY];
    int     _streamsQty = 0;
};

#endif
