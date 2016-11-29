/**
 * @file       BlynkStreamDebugger.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jun 2015
 * @brief      Utility functions
 *
 */

#ifndef BlynkStreamDebugger_h
#define BlynkStreamDebugger_h

class BlynkStreamDebugger
  : public Stream
{
  public:
    BlynkStreamDebugger(Stream& data, Stream& dump)
      : _data(data), _dump(dump)
    {
    }
    virtual size_t write(uint8_t ch) {
      _dump.write(ch);
      return _data.write(ch);
    }
    virtual int read() {
      int ch = _data.read();
      _dump.write(ch);
      return ch;
    }
    virtual int available() { return _data.available(); }
    virtual int peek()      { return _data.peek();      }
    virtual void flush()    { _data.flush();            }
  private:
    Stream& _data;
    Stream& _dump;
};

#endif
