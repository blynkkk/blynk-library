/*
 * SimpleTimer.cpp
 *
 * SimpleTimer - A timer library for Arduino.
 * Author: mromani@ottotecnica.com
 * Copyright (c) 2010 OTTOTECNICA Italy
 *
 * Callback function parameters added & compiler warnings
 * removed by Bill Knight <billk@rosw.com> 20March2017
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
 */


#include "Blynk/BlynkTimer.h"
#include <string.h>

// Select time function:
//static inline unsigned long elapsed() { return micros(); }
static inline unsigned long elapsed() { return BlynkMillis(); }


SimpleTimer::SimpleTimer()
    : numTimers (-1)
{
}

void SimpleTimer::init() {
    unsigned long current_millis = elapsed();

    for (int i = 0; i < MAX_TIMERS; i++) {
        timer[i] = timer_t();
        timer[i].prev_millis = current_millis;
    }

    numTimers = 0;
}


void SimpleTimer::run() {
    int i;
    unsigned long current_millis;

    // get current time
    current_millis = elapsed();

    for (i = 0; i < MAX_TIMERS; i++) {

        timer[i].toBeCalled = DEFCALL_DONTRUN;

        // jump over empty slots
        if (isValidTimer(i)) {

            // is it time to process this timer ?
            // see http://arduino.cc/forum/index.php/topic,124048.msg932592.html#msg932592

            if ((current_millis - timer[i].prev_millis) >= timer[i].delay) {

                if (timer[i].delay) {
                    unsigned long skipTimes = (current_millis - timer[i].prev_millis) / timer[i].delay;
                    // update time
                    timer[i].prev_millis += timer[i].delay * skipTimes;
                } else {
                    timer[i].prev_millis = current_millis;
                }

                // check if the timer callback has to be executed
                if (timer[i].enabled) {

                    // "run forever" timers must always be executed
                    if (timer[i].maxNumRuns == RUN_FOREVER) {
                        timer[i].toBeCalled = DEFCALL_RUNONLY;
                    }
                    // other timers get executed the specified number of times
                    else if (timer[i].numRuns < timer[i].maxNumRuns) {
                        timer[i].toBeCalled = DEFCALL_RUNONLY;
                        timer[i].numRuns++;

                        // after the last run, delete the timer
                        if (timer[i].numRuns >= timer[i].maxNumRuns) {
                            timer[i].toBeCalled = DEFCALL_RUNANDDEL;
                        }
                    }
                }
            }
        }
    }

    for (i = 0; i < MAX_TIMERS; i++) {
        if (timer[i].toBeCalled == DEFCALL_DONTRUN) {
            continue;
        }

        if (timer[i].callback_p) {
            timer[i].callback_p(timer[i].param);
        } else {
            timer[i].callback();
        }

        if (timer[i].toBeCalled == DEFCALL_RUNANDDEL) {
            deleteTimer(i);
        }
    }
}


// find the first available slot
// return -1 if none found
int SimpleTimer::findFirstFreeSlot() {
    // all slots are used
    if (numTimers >= MAX_TIMERS) {
        return -1;
    }

    // return the first slot with no callback (i.e. free)
    for (int i = 0; i < MAX_TIMERS; i++) {
        if (!isValidTimer(i)) {
            return i;
        }
    }

    // no free slots found
    return -1;
}


int SimpleTimer::setupTimer(unsigned long d, const timer_callback& f, uint16_t n) {
    int freeTimer;

    if (numTimers < 0) {
        init();
    }

    freeTimer = findFirstFreeSlot();
    if (freeTimer < 0) {
        return -1;
    }

    if (f == NULL) {
        return -1;
    }

    timer[freeTimer].delay = d;
    timer[freeTimer].callback = f;
    timer[freeTimer].callback_p = NULL;
    timer[freeTimer].maxNumRuns = n;
    timer[freeTimer].enabled = true;
    timer[freeTimer].prev_millis = elapsed();

    numTimers++;

    return freeTimer;
}

int SimpleTimer::setupTimer(unsigned long d, timer_callback_p f, void* p, uint16_t n) {
    int freeTimer;

    if (numTimers < 0) {
        init();
    }

    freeTimer = findFirstFreeSlot();
    if (freeTimer < 0) {
        return -1;
    }

    if (f == NULL) {
        return -1;
    }

    timer[freeTimer].delay = d;
    timer[freeTimer].callback = NULL;
    timer[freeTimer].callback_p = f;
    timer[freeTimer].param = p;
    timer[freeTimer].maxNumRuns = n;
    timer[freeTimer].enabled = true;
    timer[freeTimer].prev_millis = elapsed();

    numTimers++;

    return freeTimer;
}

bool SimpleTimer::changeInterval(unsigned numTimer, unsigned long d) {
    if (numTimer >= MAX_TIMERS) {
        return false;
    }

    // Updates interval of existing specified timer
    if (isValidTimer(numTimer)) {
        timer[numTimer].delay = d;
        timer[numTimer].prev_millis = elapsed();
        return true;
    }
    // false return for non-used numTimer, no callback
    return false;
}

bool SimpleTimer::changeFunction(unsigned numTimer, const timer_callback& f) {
    if (numTimer >= MAX_TIMERS) {
        return false;
    }

    if (isValidTimer(numTimer)) {
        timer[numTimer].callback = f;
        timer[numTimer].callback_p = NULL;
        timer[numTimer].param = NULL;
        return true;
    }
    return false;
}

bool SimpleTimer::changeFunction(unsigned numTimer, timer_callback_p f, void* p) {
    if (numTimer >= MAX_TIMERS) {
        return false;
    }

    if (isValidTimer(numTimer)) {
        timer[numTimer].callback = NULL;
        timer[numTimer].callback_p = f;
        timer[numTimer].param = p;
        return true;
    }
    return false;
}

void SimpleTimer::deleteTimer(unsigned timerId) {
    if (timerId >= MAX_TIMERS) {
        return;
    }

    // nothing to delete if no timers are in use
    if (numTimers == 0) {
        return;
    }

    // don't decrease the number of timers if the
    // specified slot is already empty
    if (isValidTimer(timerId)) {
        timer[timerId] = timer_t();
        timer[timerId].prev_millis = elapsed();

        // update number of timers
        numTimers--;
    }
}


// function contributed by code@rowansimms.com
void SimpleTimer::restartTimer(unsigned numTimer) {
    if (numTimer >= MAX_TIMERS) {
        return;
    }

    timer[numTimer].prev_millis = elapsed();
}

void SimpleTimer::executeNow(unsigned numTimer) {
    if (numTimer >= MAX_TIMERS) {
        return;
    }

    timer[numTimer].prev_millis = elapsed() - timer[numTimer].delay;
}

bool SimpleTimer::isEnabled(unsigned numTimer) {
    if (numTimer >= MAX_TIMERS) {
        return false;
    }

    return isValidTimer(numTimer) && timer[numTimer].enabled;
}

unsigned long SimpleTimer::remainingTime(unsigned numTimer) {
    if (!isEnabled(numTimer)) {
        return -1;
    }
    return (timer[numTimer].prev_millis + timer[numTimer].delay) - elapsed();
}

void SimpleTimer::enable(unsigned numTimer) {
    if (numTimer >= MAX_TIMERS) {
        return;
    }

    timer[numTimer].enabled = true;
}


void SimpleTimer::disable(unsigned numTimer) {
    if (numTimer >= MAX_TIMERS) {
        return;
    }

    timer[numTimer].enabled = false;
}

void SimpleTimer::enableAll() {
    // Enable all timers with a callback assigned (used)
    for (int i = 0; i < MAX_TIMERS; i++) {
        if (isValidTimer(i) && timer[i].maxNumRuns == RUN_FOREVER) {
            timer[i].enabled = true;
        }
    }
}

void SimpleTimer::disableAll() {
    // Disable all timers with a callback assigned (used)
    for (int i = 0; i < MAX_TIMERS; i++) {
        if (isValidTimer(i) && timer[i].maxNumRuns == RUN_FOREVER) {
            timer[i].enabled = false;
        }
    }
}

void SimpleTimer::toggle(unsigned numTimer) {
    if (numTimer >= MAX_TIMERS) {
        return;
    }

    timer[numTimer].enabled = !timer[numTimer].enabled;
}


unsigned SimpleTimer::getNumTimers() {
    return numTimers;
}
