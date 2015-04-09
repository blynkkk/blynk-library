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
 * Represents high-level functions of Blynk
 */
template <class Proto>
class BlynkApi
{
public:
    BlynkApi() {
        Init();
    }

    /**
     * Sends value to a Virtual Pin
     *
     * @param pin  Virtual Pin number
     * @param data Value to be sent
     */
    template <typename T>
    void virtualWrite(int pin, const T& data) {
        char mem[64];
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add("vw");
        cmd.add(pin);
        cmd.add(data);
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_HARDWARE, 0, cmd.getBuffer(), cmd.getLength());
    }

    /**
     * Sends buffer to a Virtual Pin
     *
     * @param pin  Virtual Pin number
     * @param buff Data buffer
     * @param len  Length of data
     */
    void virtualWrite(int pin, const void* buff, size_t len) {
        char mem[8];
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add("vw");
        cmd.add(pin);
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_HARDWARE, 0, cmd.getBuffer(), cmd.getLength(), buff, len);
    }

    /**
     * Sends BlynkParam to a Virtual Pin
     *
     * @param pin  Virtual Pin number
     * @param param
     */
    void virtualWrite(int pin, const BlynkParam& param) {
        virtualWrite(pin, param.getBuffer(), param.getLength());
    }

    /**
     * Tweets a message
     *
     * @param msg Text of the message
     */
    void tweet(const char* msg) {
        size_t len = strlen(msg);
        if (len < 140) {
            static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_TWEET, 0, msg, len+1);
        }
    }

    /**
     * Sends a push notification to the App
     *
     * @param msg Text of the message
     */
    void push_notification(const char* msg) {
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_PUSH_NOTIFICATION, 0, msg, strlen(msg)+1);
    }

    /**
     * Sends an pre-defined email message
     * The message is defined in the App
     */
    void email() {
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_EMAIL, 0);
    }

    /**
     * Sends an email message
     *
     * @param msg Text of the message
     */
    void email(const char* msg) {
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_EMAIL, 0, msg, strlen(msg)+1);
    }

    /**
     * Sends an email message
     *
     * @param email   Email to send to
     * @param subject Subject of message
     * @param msg     Text of the message
     */
    void email(const char* email, const char* subject, const char* msg) {
        char mem[128];
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add(email);
        cmd.add(subject);
        cmd.add(msg);
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_EMAIL, 0, cmd.getBuffer(), cmd.getLength());
    }

#if defined(BLYNK_EXPERIMENTAL)
    // Attention!
    // Every function in this section may be changed, removed or renamed.

    /**
     * Refreshes value of a widget by running
     * user-defined BLYNK_READ handler of a pin.
     *
     * @experimental
     *
     * @param pin Virtual Pin number
     */
    void refresh(int pin) {
        if (WidgetReadHandler handler = GetReadHandler(pin)) {
            BlynkReq req = { 0, BLYNK_SUCCESS, (uint8_t)pin };
            handler(req);
        }
    }

    /**
     * Requests App or Server to re-send a value of a Virtual Pin.
     * This will probably cause user-define BLYNK_WRITE handler to be called.
     *
     * @experimental
     *
     * @param pin Virtual Pin number
     */
    void virtualRead(int pin) {
        char mem[8];
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add("vr");
        cmd.add(pin);
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_HARDWARE, 0, cmd.getBuffer(), cmd.getLength());
    }

    /**
     * digitalWrite + update widget state
     *
     * @experimental
     *
     * @param pin Digital pin number
     * @param val Value to set
     */
    void digitalWrite(uint8_t pin, uint8_t val) {
    	::digitalWrite(pin, val);
        char mem[8];
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add("dw");
        cmd.add(pin);
        cmd.add(val);
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_HARDWARE, 0, cmd.getBuffer(), cmd.getLength());
    }

    /**
     * analogWrite + update widget state
     *
     * @experimental
     *
     * @param pin PWM pin number
     * @param val Value to set
     */
    void analogWrite(uint8_t pin, int val) {
    	::analogWrite(pin, val);
        char mem[12];
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add("aw");
        cmd.add(pin);
        cmd.add(val);
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_HARDWARE, 0, cmd.getBuffer(), cmd.getLength());
    }

    /**
     * Delays for N milliseconds, handling server communication in background.
     *
     * @experimental
     * @warning Should be used very carefully, especially on platforms with small RAM.
     *
     * @param ms Milliseconds to wait
     */
    void delay(unsigned long ms) {
        uint16_t start = (uint16_t)micros();
        while (ms > 0) {
            static_cast<Proto*>(this)->run();
            yield();
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
