#pragma once

#include "esp_timer.h"
#include "esp_err.h"
#include <stdio.h>
#include <inttypes.h>

/* This module defines a timer that can be either be single shot or autorest */

/*
Bedclock_IDF_V01 : Test WS2812 led chain
Bedclock_IDF_V02 : Test SSD1306 OLED screen
Bedclock_IDF_V03 : Test capacitive touch sensors
Bedclock_IDF_V04 : Test WiFi
Bedclock_IDF_V05 : Test time sync with timeserver
Bedclock_IDF_V06 : Port timer object using esp_timer_get_time() / 1000;
Bedclock_IDF_V07 : Test FreeRTOS
*/

typedef struct {
    int64_t interval;       // timer interval in us
    bool auto_restart;      // indicates if reset() must be called after each lapse
    int64_t previous_call;  // the time at which the timer was reset
    bool response_given;    // ensures only one lapsed() event is returned if auto_restart=false
} hp_timer_t;

esp_err_t hp_timer_reset(hp_timer_t * timer) {
    timer->previous_call = esp_timer_get_time();
    timer->response_given = false;
    return ESP_OK;
}

esp_err_t hp_timer_init(hp_timer_t * timer, int64_t interval_us, bool auto_restart) {
    timer->interval = interval_us;
    timer->auto_restart = auto_restart;
    hp_timer_reset(timer);
    return ESP_OK;
}

esp_err_t hp_timer_init_ms(hp_timer_t * timer, int64_t interval_ms, bool auto_restart) {
    timer->interval = 1000*interval_ms;
    timer->auto_restart = auto_restart;
    hp_timer_reset(timer);
    return ESP_OK;
}

bool hp_timer_lapsed(hp_timer_t * timer) {
    // Return false if the timer has not reached the interval
    if(esp_timer_get_time() - timer->previous_call < timer->interval ) return false;

    // The interval was reached
    if(timer->auto_restart) {

        // This is an autorestart timer
        hp_timer_reset(timer);
        return true;

    } else {
        // This is a single shot timer
        if(timer->response_given)
            return false;
        else {
            timer->response_given = true;
            return true;
        }   
    }
}

