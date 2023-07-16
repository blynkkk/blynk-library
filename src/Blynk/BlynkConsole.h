/**
 * @file       BlynkConsole.h
 * @author     Volodymyr Shymanskyy
 * @license    This project is released under the MIT License (MIT)
 * @copyright  Copyright (c) 2020 Volodymyr Shymanskyy
 * @date       Oct 2020
 * @brief      Console Utility
 *
 */

#ifndef BlynkConsole_h
#define BlynkConsole_h

#include <Blynk/BlynkDebug.h>
#include <Blynk/BlynkParam.h>

#define BLYNK_CONSOLE_MAX_COMMANDS 64
#define BLYNK_CONSOLE_INPUT_BUFFER 256
#define BLYNK_CONSOLE_USE_STREAM

#ifdef BLYNK_CONSOLE_USE_STREAM
  #include <stdarg.h>
#endif

class BlynkConsole
{
private:

#ifdef BLYNK_HAS_FUNCTIONAL_H
    typedef std::function<void(void)> HandlerSimp;
    typedef std::function<void(int argc, const char** argv)> HandlerArgs;
    typedef std::function<void(const BlynkParam &param)> HandlerParams;
#else
    typedef void (*HandlerSimp)();
    typedef void (*HandlerArgs)(int argc, const char** argv);
    typedef void (*HandlerParams)(const BlynkParam &param);
#endif
    enum HandlerType {
        SIMPLE,
        WITH_ARGS,
        WITH_PARAMS,
        SUB_CONSOLE
    };

    class CmdHandler {
    public:
        const char* cmd;
        HandlerType type;
        union {
            HandlerSimp*    f_simp;
            HandlerArgs*    f_args;
            HandlerParams*  f_params;
            BlynkConsole*   f_cons;
        };
        CmdHandler() = default;
        CmdHandler(const char* s, HandlerSimp* f)
            : cmd(s), type(SIMPLE), f_simp(f)
        {}
        CmdHandler(const char* s, HandlerArgs* f)
            : cmd(s), type(WITH_ARGS), f_args(f)
        {}
        CmdHandler(const char* s, HandlerParams* f)
            : cmd(s), type(WITH_PARAMS), f_params(f)
        {}
        CmdHandler(const char* s, BlynkConsole* f)
            : cmd(s), type(SUB_CONSOLE), f_cons(f)
        {}
    };

public:
    
    enum ProcessResult {
        PROCESSED,
        SKIPPED,
        EXECUTED,
        NOT_FOUND,
    };

    BlynkConsole() {
        reset_buff();

#if defined(BLYNK_CONSOLE_USE_STREAM) && defined(BLYNK_HAS_FUNCTIONAL_H)
        help = [=]() {
            if (!stream) return;
            stream->print("Available commands: ");
            for (size_t i=0; i<commandsQty; i++) {
                CmdHandler& handler = commands[i];
                stream->print(handler.cmd);
                if (i < commandsQty-1) { stream->print(", "); }
            }
            stream->println();
        };
        
        addCommand("help", help);
        addCommand("?", help);
#endif

    }

#ifdef BLYNK_CONSOLE_USE_STREAM
    void print() {}

    template <typename T>
    void print(T val) {
        if (stream) stream->print(val);
    }

    template <typename T1, typename T2>
    void print(T1 val1, T2 val2) {
        if (stream) stream->print(val1, val2);
    }

    void printf(const char *fmt, ... ) {
        if (stream) {
            char buf[256];
            va_list args;
            va_start (args, fmt);
            vsnprintf(buf, sizeof(buf), (char*)fmt, args);
            va_end (args);
            stream->print(buf);
        }
    }
#endif

    void addCommand(const char* cmd, HandlerSimp h) {
        if (commandsQty >= BLYNK_CONSOLE_MAX_COMMANDS) return;
        commands[commandsQty++] = CmdHandler(cmd, new HandlerSimp(h));
    }

    void addCommand(const char* cmd, HandlerArgs h) {
        if (commandsQty >= BLYNK_CONSOLE_MAX_COMMANDS) return;
        commands[commandsQty++] = CmdHandler(cmd, new HandlerArgs(h));
    }

    void addCommand(const char* cmd, HandlerParams h) {
        if (commandsQty >= BLYNK_CONSOLE_MAX_COMMANDS) return;
        commands[commandsQty++] = CmdHandler(cmd, new HandlerParams(h));
    }

    void addCommand(const char* cmd, BlynkConsole* h) {
        if (commandsQty >= BLYNK_CONSOLE_MAX_COMMANDS || !h) return;
#ifdef BLYNK_CONSOLE_USE_STREAM
        h->begin(stream);
#endif
        commands[commandsQty++] = CmdHandler(cmd, h);
    }

    void addCommand(const char* cmd, BlynkConsole& h) {
        addCommand(cmd, &h);
    }

    ProcessResult process(char c) {
        if (cmdPtr >= cmdBuff+sizeof(cmdBuff)) {
            reset_buff();
        }

        *(cmdPtr++) = c;
        if (c == '\n' || c == '\r') {
            return runCommandInBuff();
        }
        return PROCESSED;
    }

    ProcessResult runCommand(const char* cmd) {
        strncpy(cmdBuff, cmd, sizeof(cmdBuff));
        cmdBuff[sizeof(cmdBuff)-1] = '\0';
        cmdPtr = cmdBuff + strlen(cmdBuff);
        return runCommandInBuff();
    }

private:

    ProcessResult runCommandInBuff() {
        char* argv[16];
        int argc = split_argv(cmdBuff, argv, 16);
        if (argc <= 0) {
            return SKIPPED;
        }
#ifdef BLYNK_CONSOLE_USE_STREAM
        if (stream) stream->println();
#endif
        ProcessResult ret = runCommand(argc, (const char**)argv);
        reset_buff();
        return ret;
    }

    ProcessResult runCommand(int argc, const char** argv) {
        for (size_t i=0; i<commandsQty; i++) {
            CmdHandler& handler = commands[i];
            if (!strncasecmp(argv[0], handler.cmd, strlen(handler.cmd)+1)) {
                switch (handler.type) {
                case SIMPLE:
                    (*(handler.f_simp))();
                    break;
                case WITH_ARGS:
                    (*(handler.f_args))(argc-1, (const char**)(argv+1));
                    break;
                case WITH_PARAMS: {
                    size_t len = (cmdPtr - cmdBuff);
                    if (len > 0 && len < BLYNK_CONSOLE_INPUT_BUFFER) {
                        char mem[len];
                        BlynkParam param(mem, 0, sizeof(mem));
                        for (int i = 1; i < argc; i++) {
                            param.add(argv[i]);
                        }
                        (*(handler.f_params))(param);
                    }
                    break;
                }
                case SUB_CONSOLE:
                    if (argc < 2) return NOT_FOUND;
                    return handler.f_cons->runCommand(argc-1, (const char**)(argv+1));
                }
                return EXECUTED;
            }
        }
        return NOT_FOUND;
    }

public:

#ifdef BLYNK_CONSOLE_USE_STREAM

    void begin(Stream& s) {
        stream = &s;
    }

    void begin(Stream* s) {
        stream = s;
    }

    Stream& getStream() {
        return *stream;
    }

    void run() {
        while (stream && stream->available()) {
            char c = stream->read();
            switch (process(c)) {
            case SKIPPED: break;
            case PROCESSED:
                stream->print(c);
                break;
            case NOT_FOUND:
                stream->println("Command not found.");
                // Fall-through
            case EXECUTED:
                stream->print(">");
                break;
            }
        }
    }
#endif

private:
    CmdHandler commands[BLYNK_CONSOLE_MAX_COMMANDS];
    unsigned   commandsQty = 0;

    char* cmdPtr;
    char  cmdBuff[BLYNK_CONSOLE_INPUT_BUFFER];

#ifdef BLYNK_CONSOLE_USE_STREAM
    Stream* stream = nullptr;
    HandlerSimp help;
#endif

    void reset_buff() {
        memset(cmdBuff, 0, sizeof(cmdBuff));
        cmdPtr = cmdBuff;
    }

    static
    void unescape(char* buff)
    {
        char* outp = buff;
        while (*buff) {
            if (*buff == '\\') {
                switch (*(buff+1)) {
                case '0':  *outp++ = '\0'; break;
                case 'b':  *outp++ = '\b'; break;
                case 'n':  *outp++ = '\n'; break;
                case 'r':  *outp++ = '\r'; break;
                case 't':  *outp++ = '\t'; break;
                case 'x': {
                    char hex[3] = { *(buff+2), *(buff+3), '\0' };
                    *outp = strtol(hex, NULL, 16);
                    buff += 2; outp += 1;
                    break;
                }
                // Otherwise just pass the letter
                // Also handles '\\'
                default: *outp++ = *(buff+1); break;
                }
                buff += 2;
            } else {
                *outp++ = *buff++;
            }
        }
        *outp = '\0';
    }
    
    static
    int split_argv(char *str, char** argv, int argv_capacity)
    {
        int result = 0;
        char* curr = str;
        int len = 0;
        memset(argv, 0, sizeof(char*)*argv_capacity);
        for (int i = 0; str[i] != '\0' && result < (argv_capacity-1); i++) {
            if (strchr(" \n\r\t", str[i])) {
                if (len) {  // Found space after non-space
                    str[i] = '\0';
                    unescape(curr);
                    argv[result++] = curr;
                    len = 0;
                }
            } else {
                if (!len) { // Found non-space after space
                    curr = &str[i];
                }
                len++;
            }
        }
        // Add final argument, if needed
        if (len && result < (argv_capacity-1)) {
            unescape(curr);
            argv[result++] = curr;
        }
        return result;
    }

};

#endif // BlynkConsole
