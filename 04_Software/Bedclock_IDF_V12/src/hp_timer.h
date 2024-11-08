#pragma once

#include "esp_timer.h"
#include "esp_err.h"
#include <stdio.h>

/**
 * @file hp_timer.h
 * @brief microsecond-precision 64-bit timer API
 *
 * Can be used for single shot timers or for looping (auto reset) timers
 * Beware that an auto reset timer will only be reset if and when
 * hp_timer_has_lapsed() or hp_timer_lapsed() are called.
 *
 * If auto_reset is enabled, the interval is added to the start time multiple
 * times until the next trigger moment is in the future.
 *
 * This means that if intervals have been missed because hp_timer_has_lapsed()
 * or hp_timer_lapsed() were not called frequently enough, trigger
 * signals will not accumulate and clog the system.
 *
 * Just for fun:
 *
 *      esp_timer_get_time() - timer->last_reset_time) < timer->interval
 *
 * is used to ensure that the timer also works if the clock passes the 64 bit mark,
 * (250.000 years after switching on the device).
*/

/**
 * @brief structure that holds the data for a single timer
 */
typedef struct {
    int64_t interval;         // timer interval in us
    bool auto_restart;        // indicates if reset() must be called after each lapse
    int64_t last_reset_time;  // the time at which the timer was reset
    bool response_given;      // ensures only one hp_timer_lapsed() event is returned for single shot timers
} hp_timer_t;

/**
 * @brief Reset the timer so it starts counting at zero
 *
 * @param timer timer handle
 */
void hp_timer_reset(hp_timer_t * timer) {
    timer->last_reset_time = esp_timer_get_time();
    timer->response_given = false;
}

/**
 * @brief Initializes the timer with an interval in microseconds
 *
 * @param timer timer handle
 * @param interval_us timer interval in microseconds
 * @param auto_restart true if the timer must restart after the interval has expired
 */
void hp_timer_init(hp_timer_t * timer, int64_t interval_us, bool auto_restart) {
    timer->interval = interval_us;
    timer->auto_restart = auto_restart;
    hp_timer_reset(timer);
}

/**
 * @brief Initializes the timer with an interval in milliseconds
 *
 * @param timer timer handle
 * @param interval_ms timer interval in milliseconds
 * @param auto_restart true if the timer must restart after the interval has expired
 */
void hp_timer_init_ms(hp_timer_t * timer, int64_t interval_ms, bool auto_restart) {
    timer->interval = 1000*interval_ms;
    timer->auto_restart = auto_restart;
    hp_timer_reset(timer);
}

/**
 * @brief Sets the interval in microseconds but leaves autorestart unchanged
 *
 * @param timer timer handle
 * @param interval_us timer interval in microseconds
 */
void hp_set_interval(hp_timer_t * timer, int64_t interval_us) {
    timer->interval = interval_us;
}

/**
 * @brief Sets the interval in milliseconds but leaves autorestart unchanged
 *
 * @param timer timer handle
 * @param interval_ms timer interval in milliseconds
 */
void hp_set_interval_ms(hp_timer_t * timer, int64_t interval_ms) {
    timer->interval = 1000*interval_ms;
}

/**
 * @brief Returns true if the interval has passed
 *        the same value will be returned if this is called multiple times.
 *        a call to this function will also reset the timer interval if
 *        auto_restart is enabled.
 *
 * @param timer timer handle
 * @return True if the interval has expired
 */
bool hp_timer_has_lapsed(hp_timer_t * timer) {
    // Return true if timer has passed the interval
    int64_t current_time = esp_timer_get_time();
    bool has_lapsed = (current_time - timer->last_reset_time) >= timer->interval;

    // The interval was reached
    if(has_lapsed && timer->auto_restart) {
        while((current_time - timer->last_reset_time) >= timer->interval) {
            timer->last_reset_time+=timer->interval;
        }
    }

    return has_lapsed;
}

/**
 * @brief Returns true if the interval was just passed
 *        for a single shot timer, a true value will only be returned
 *        once unless hp_timer_reset() is called
 *        a call to this function will also reset the timer interval if
 *        auto_restart is enabled.
 *
 * @param timer timer handle
 * @return True if the interval has just expired
 */
bool hp_timer_lapsed(hp_timer_t * timer) {
    // Return false if the timer has not reached the interval
    if( !hp_timer_has_lapsed(timer) ) return false;

    // Return true if the timer is an autoresponse timer
    if(timer->auto_restart) return true;

    // Return true only once if it is a single shot timer
    if(timer->response_given)
        return false;
    else {
        timer->response_given = true;
        return true;
    }
}
