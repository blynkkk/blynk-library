/**
 * @file       BlynkApiNCP.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2023 Volodymyr Shymanskyy
 * @date       Jun 2023
 * @brief      High-level functions
 *
 */

#ifndef BlynkApi_h
#define BlynkApi_h

#if !defined(BLYNK_FIRMWARE_TYPE) && defined(BLYNK_TEMPLATE_ID)
    #define BLYNK_FIRMWARE_TYPE         BLYNK_TEMPLATE_ID
#endif

#if !defined(BLYNK_FIRMWARE_VERSION)
    #define BLYNK_FIRMWARE_VERSION      "0.0.0"
#endif

#if !defined(BLYNK_TEMPLATE_ID) || !defined(BLYNK_TEMPLATE_NAME)
    #error "Please specify your BLYNK_TEMPLATE_ID and BLYNK_TEMPLATE_NAME"
#endif

#if defined(BLYNK_AUTH_TOKEN)
    #error "BLYNK_AUTH_TOKEN is assigned automatically when using Blynk.Edgent, please remove it from the configuration"
#endif

#include <Blynk/BlynkConfig.h>
#include <Blynk/BlynkParam.h>
#include <Blynk/BlynkHandlers.h>

/**
 * Represents high-level functions of Blynk
 */
template <class Proto>
class BlynkApi
{
public:
    BlynkApi() {
    }

    /**
     * Sends value to a Virtual Pin
     *
     * @param pin  Virtual Pin number
     * @param data Value to be sent
     */
    template <typename... Args>
    void virtualWrite(int pin, Args... values) {
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add_multi(values...);

        rpc_buffer_t val = { (uint8_t*)cmd.getBuffer(), cmd.getLength()-1 };
        rpc_blynk_virtualWrite(pin, val);
    }

    /**
     * Sends buffer to a Virtual Pin
     *
     * @param pin  Virtual Pin number
     * @param buff Data buffer
     * @param len  Length of data
     */
    void virtualWriteBinary(int pin, const void* buff, size_t len) {
        rpc_buffer_t val = { (uint8_t*)buff, len };
        rpc_blynk_virtualWrite(pin, val);
    }

    /**
     * Sends BlynkParam to a Virtual Pin
     *
     * @param pin  Virtual Pin number
     * @param param
     */
    void virtualWrite(int pin, const BlynkParam& param) {
        virtualWriteBinary(pin, param.getBuffer(), param.getLength()-1);
    }

    void virtualWrite(int pin, const BlynkParamAllocated& param) {
        virtualWriteBinary(pin, param.getBuffer(), param.getLength()-1);
    }

    /**
     * Command grouping
     */
    void beginGroup() {
        rpc_blynk_beginGroup(0);
    }

    void beginGroup(uint64_t timestamp) {
        rpc_blynk_beginGroup(timestamp);
    }

    void endGroup() {
        rpc_blynk_endGroup();
    }

    /**
     * Handler helpers
     */
    void callWriteHandler(BlynkReq& req, const BlynkParam& param) {
        WidgetWriteHandler handler = GetWriteHandler(req.pin);
        if (handler && (handler != BlynkWidgetWrite)) {
            handler(req, param);
        } else {
            BlynkWidgetWriteDefault(req, param);
        }
    }

    /**
     * Requests Server to re-send current values for all widgets.
     */
    void syncAll() {
        rpc_blynk_syncAll();
    }

    /**
     * Requests App or Server to re-send current value of a Virtual Pin.
     * This will probably cause user-defined BLYNK_WRITE handler to be called.
     *
     * @param pin Virtual Pin number
     */
    template <typename... Args>
    void syncVirtual(Args... pins) {
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add_multi(pins...);
        rpc_buffer_t val = { (uint8_t*)cmd.getBuffer(), cmd.getLength()-1 };
        rpc_blynk_syncVirtual(val);
    }

    /**
     * Sets property of a Widget
     *
     * @experimental
     *
     * @param pin      Virtual Pin number
     * @param property Property name ("label", "labels", "color", ...)
     * @param value    Property value
     */
    template <typename... Args>
    void setProperty(int pin, const char* property, Args... values) {
        BlynkParam cmd(mem, 0, sizeof(mem));
        cmd.add_multi(values...);

        rpc_buffer_t val = { (uint8_t*)cmd.getBuffer(), cmd.getLength()-1 };
        rpc_blynk_setProperty(pin, property, val);
    }

    void setProperty(int pin, const char* property, const BlynkParam& param) {
        rpc_buffer_t val = { (uint8_t*)param.getBuffer(), param.getLength()-1 };
        rpc_blynk_setProperty(pin, property, val);
    }

    void setProperty(int pin, const char* property, const BlynkParamAllocated& param) {
        rpc_buffer_t val = { (uint8_t*)param.getBuffer(), param.getLength()-1 };
        rpc_blynk_setProperty(pin, property, val);
    }

    void logEvent(const char* event_name) {
        rpc_blynk_logEvent(event_name, "");
    }

    void logEvent(const char* event_name, const char* description) {
        rpc_blynk_logEvent(event_name, description);
    }

    void resolveEvent(const char* event_name) {
        rpc_blynk_resolveEvent(event_name);
    }

    void resolveAllEvents(const char* event_name) {
        rpc_blynk_resolveAllEvents(event_name);
    }

    void setMetadata(const char* field_name, const char* value) {
        rpc_blynk_setMetadata(field_name, value);
    }

protected:

    char mem[BLYNK_MAX_SENDBYTES];
};


#endif
