/**
 * @file       BlynkApi.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2015 Volodymyr Shymanskyy
 * @date       Jan 2015
 * @brief      High-level functions
 *
 */

#ifndef BlynkApi_h
#define BlynkApi_h

#include <Blynk/BlynkConfig.h>
#include <Blynk/BlynkDebug.h>
#include <Blynk/BlynkParam.h>
#include <Blynk/BlynkHandlers.h>
#include <Blynk/BlynkProtocolDefs.h>

/**
* A test class. A more elaborate class description.
*/
template <class Proto>
class BlynkApi
{
public:
    BlynkApi() {
        Init();
    }

    template <typename T>
    void virtualWrite(int pin, const T& data) {
        char mem[64];
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add("vw");
        cmd.add(pin);
        cmd.add(data);
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_HARDWARE, 0, cmd.getBuffer(), cmd.getLength());
    }

    void virtualWrite(int pin, const void* buff, size_t len) {
        char mem[8];
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add("vw");
        cmd.add(pin);
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_HARDWARE, 0, cmd.getBuffer(), cmd.getLength(), buff, len);
    }

    void virtualWrite(int pin, const BlynkParam& param) {
        virtualWrite(pin, param.getBuffer(), param.getLength());
    }

    void tweet(const char* msg) {
        size_t len = strlen(msg);
        if (len < 140) {
            static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_TWEET, 0, msg, len);
        }
    }

#if defined(BLYNK_HAS_DELAY)

    void delay(unsigned long ms) {
        uint16_t start = (uint16_t)micros();
        while (ms > 0) {
            static_cast<Proto*>(this)->run();
            if (((uint16_t)micros() - start) >= 1000) {
                ms--;
                start += 1000;
            }
        }
    }

#endif

protected:
    void Init();
    void processCmd(const void* buff, size_t len);

};


#endif
