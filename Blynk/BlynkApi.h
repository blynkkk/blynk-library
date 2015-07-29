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

#ifdef DOXYGEN // These API here are only for the documentation

    /**
     * Connects to the server.
     * Blocks until connected or timeout happens.
     * May take less or more then timeout value.
     *
     * @param timeout    Connection timeout
     * @returns          True if connected to the server
     */
    bool connect(unsigned long timeout = BLYNK_TIMEOUT_MS*3);

    /**
     * Disconnects from the server.
     * It will not try to reconnect, until connect() is called
     */
    void disconnect();

    /**
     * @returns          True if connected to the server
     */
    bool connected();

    /**
     * Performs Blynk-related housekeeping
     * and processes incoming commands
     *
     * @param available  True if there is incoming data to process
     *                   Only used when user manages connection manually.
     */
    bool run(bool available = false);

#endif // DOXYGEN

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
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_HARDWARE, 0, cmd.getBuffer(), cmd.getLength()-1);
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
    template<typename T>
    void tweet(const T& msg) {
        char mem[128];
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add(msg);
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_TWEET, 0, cmd.getBuffer(), cmd.getLength()-1);
    }

    /**
     * Sends a push notification to the App
     *
     * @param msg Text of the message
     */
    template<typename T>
    void notify(const T& msg) {
        char mem[128];
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add(msg);
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_NOTIFY, 0, cmd.getBuffer(), cmd.getLength()-1);
    }

    /**
     * Sends an email message
     *
     * @param email   Email to send to
     * @param subject Subject of message
     * @param msg     Text of the message
     */
    template <typename T1, typename T2>
    void email(const char* email, const T1& subject, const T2& msg) {
        char mem[128];
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add(email);
        cmd.add(subject);
        cmd.add(msg);
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_EMAIL, 0, cmd.getBuffer(), cmd.getLength()-1);
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
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_HARDWARE, 0, cmd.getBuffer(), cmd.getLength()-1);
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
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_HARDWARE, 0, cmd.getBuffer(), cmd.getLength()-1);
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
        static_cast<Proto*>(this)->sendCmd(BLYNK_CMD_HARDWARE, 0, cmd.getBuffer(), cmd.getLength()-1);
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
#if defined(ARDUINO) && (ARDUINO >= 151)
            yield();
#endif
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
