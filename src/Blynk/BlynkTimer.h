/*
 * SimpleTimer.h
 *
 * SimpleTimer - A timer library for Arduino.
 * Author: mromani@ottotecnica.com
 * Copyright (c) 2010 OTTOTECNICA Italy
 *
 * Modifications by Bill Knight <billk@rosw.com> 18March2017
 *
 * This library is free software; you can redistribute it
 * and/or modify it under the terms of the GNU Lesser
 * General Public License as published by the Free Software
 * Foundation; either version 2.1 of the License, or (at
 * your option) any later version.
 *
 * This library is distributed in the hope that it will
 * be useful, but WITHOUT ANY WARRANTY; without even the
 * implied warranty of MERCHANTABILITY or FITNESS FOR A
 * PARTICULAR PURPOSE.  See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser
 * General Public License along with this library; if not,
 * write to the Free Software Foundation, Inc.,
 * 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 *
 */

#ifndef BLYNKTIMER_H
#define BLYNKTIMER_H

#include <Blynk/BlynkDebug.h>

// Replace SimpleTimer
#define SIMPLETIMER_H
#define SimpleTimer BlynkTimer

#ifndef BLYNK_MAX_TIMERS
  #define BLYNK_MAX_TIMERS 16
#endif

class SimpleTimer {
#ifdef BLYNK_HAS_FUNCTIONAL_H
    typedef std::function<void(void)> timer_callback;
#else
    typedef void (*timer_callback)(void);
#endif
    typedef void (*timer_callback_p)(void *);

public:
    // maximum number of timers
    const static int MAX_TIMERS = BLYNK_MAX_TIMERS;

    // setTimer() constants
    const static int RUN_FOREVER = 0;
    const static int RUN_ONCE = 1;

    class Handle {
    public:
        Handle()
            : st(NULL), id(-1)
        {}

        Handle(SimpleTimer* t, int i)
            : st(t),    id(i)
        {}

        ~Handle() {
        }

        operator int() const {
            return id;
        }

        operator bool() const {
            return isValid();
        }

        void operator() (void) const {
            if (isValid()) st->executeNow(id);
        }

        bool isValid() const { return st && id >= 0; }
        void changeInterval(unsigned long d) {
            if (isValid()) st->changeInterval(id, d);
        }
        void deleteTimer()  { if (isValid()) st->deleteTimer(id); invalidate(); }
        void restartTimer() { if (isValid()) st->restartTimer(id);        }
        bool isEnabled()    { return isValid() && st->isEnabled(id);      }
        void enable()       { if (isValid()) st->enable(id);              }
        void disable()      { if (isValid()) st->disable(id);             }
        void toggle()       { if (isValid()) st->toggle(id);              }

    private:
        void invalidate()   { st = NULL; id = -1; }

        SimpleTimer*    st;
        int             id;
    };

    // constructor
    SimpleTimer();

    void init();

    // this function must be called inside loop()
    void run();

    // Timer will call function 'f' every 'd' milliseconds forever
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    Handle setInterval(unsigned long d, const timer_callback& f) {
        return Handle(this, setupTimer(d, f, RUN_FOREVER));
    }

    // Timer will call function 'f' with parameter 'p' every 'd' milliseconds forever
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    Handle setInterval(unsigned long d, timer_callback_p f, void* p) {
        return Handle(this, setupTimer(d, f, p, RUN_FOREVER));
    }

    // Timer will call function 'f' after 'd' milliseconds one time
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    Handle setTimeout(unsigned long d, const timer_callback& f) {
        return Handle(this, setupTimer(d, f, RUN_ONCE));
    }

    // Timer will call function 'f' with parameter 'p' after 'd' milliseconds one time
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    Handle setTimeout(unsigned long d, timer_callback_p f, void* p) {
        return Handle(this, setupTimer(d, f, p, RUN_ONCE));
    }

    // Timer will call function 'f' every 'd' milliseconds 'n' times
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    Handle setTimer(unsigned long d, const timer_callback& f, unsigned n) {
        return Handle(this, setupTimer(d, f, n));
    }

    // Timer will call function 'f' with parameter 'p' every 'd' milliseconds 'n' times
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    Handle setTimer(unsigned long d, timer_callback_p f, void* p, unsigned n) {
        return Handle(this, setupTimer(d, f, p, n));
    }

    // updates interval of the specified timer
    bool changeInterval(unsigned numTimer, unsigned long d);

    // destroy the specified timer
    void deleteTimer(unsigned numTimer);

    // restart the specified timer
    void restartTimer(unsigned numTimer);

    // remove delay to next execution
    void executeNow(unsigned numTimer);

    // returns true if the specified timer is enabled
    bool isEnabled(unsigned numTimer);

    // enables the specified timer
    void enable(unsigned numTimer);

    // disables the specified timer
    void disable(unsigned numTimer);

    // enables all timers
    void enableAll();

    // disables all timers
    void disableAll();

    // enables the specified timer if it's currently disabled,
    // and vice-versa
    void toggle(unsigned numTimer);

    // returns the number of used timers
    unsigned getNumTimers();

    // returns the number of available timers
    unsigned getNumAvailableTimers() { return MAX_TIMERS - numTimers; };

private:
    // deferred call constants
    const static int DEFCALL_DONTRUN = 0;       // don't call the callback function
    const static int DEFCALL_RUNONLY = 1;       // call the callback function but don't delete the timer
    const static int DEFCALL_RUNANDDEL = 2;     // call the callback function and delete the timer

    // low level function to initialize and enable a new timer
    // returns the timer number (numTimer) on success or
    // -1 on failure (f == NULL) or no free timers
    int setupTimer(unsigned long d, const timer_callback& f, unsigned n);
    int setupTimer(unsigned long d,      timer_callback_p f, void* p, unsigned n);

    bool isValidTimer(unsigned id) {
        return timer[id].callback || timer[id].callback_p;
    }

    // find the first available slot
    int findFirstFreeSlot();

    typedef struct {
      unsigned long prev_millis;        // value returned by the millis() function in the previous run() call

      timer_callback    callback;
      timer_callback_p  callback_p;

      void* param;                      // function parameter
      bool hasParam;                 // true if callback takes a parameter
      unsigned long delay;              // delay value
      unsigned maxNumRuns;              // number of runs to be executed
      unsigned numRuns;                 // number of executed runs
      bool enabled;                  // true if enabled
      unsigned toBeCalled;              // deferred function call (sort of) - N.B.: only used in run()
    } timer_t;

    timer_t timer[MAX_TIMERS];

    // actual number of timers in use (-1 means uninitialized)
    int numTimers;
};

#endif
