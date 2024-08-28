//
// Copyright 2022 Blues Inc.  All rights reserved.
// Use of this source code is governed by licenses granted by the
// copyright holder including that found in the LICENSE file.
#include <stdint.h>
#include "Arduino.h"

/**
 * @brief Provides a duration timer that can measure durations up to 2^31-1 milliseconds safely with
 * wrap around when the ticks counter resets to 0 after 2^32 ticks.
 */
struct TicksTimer {

    typedef uint32_t ticks_t;

    ticks_t start_time = 0;
    ticks_t duration = 0;

    inline void set(ticks_t start_time, ticks_t duration) {
        this->start_time = start_time;
        this->duration = duration;
    }

    inline bool hasElapsed(ticks_t current_ticks, bool reset=false) {
        bool elapsed = (current_ticks-start_time)>duration;
        if (elapsed && reset) {
            this->reset(current_ticks);
        }
        return elapsed;
    }

    inline int32_t remaining(ticks_t current_ticks) {
        return int32_t(start_time+duration)-current_ticks;
    }

    inline void reset(ticks_t current_ticks) {
        set(current_ticks, duration);
    }
};

/**
 * @brief Convenience class that implements TicksTimer using arduino millis().
 */
struct ArduinoTicksTimer: public TicksTimer {
    inline ArduinoTicksTimer() {}
    inline ArduinoTicksTimer(ticks_t duration) {
        set(duration);
    }

    inline void set(ticks_t duration) {
        TicksTimer::set(millis(), duration);
    }

    inline bool hasElapsed(bool reset=false) {
        return TicksTimer::hasElapsed(millis(), reset);
    }

    inline bool hasElapsedAndReset() {
        return hasElapsed(true);
    }

    inline int32_t remaining() {
        return TicksTimer::remaining(millis());
    }
};
