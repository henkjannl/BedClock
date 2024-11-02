#pragma once

#include <stdio.h>

/* Selected item:
  0: Sleep mode
  1: Display time
  2: Modify LED intensity
  3: Modify LED color 
  4: Modify LED timer
  5: Modify display intensity
*/

#define COMMON_MAX_LED_INTENSITY 4
#define COMMON_MAX_LED_COLOR 4
#define COMMON_MAX_LED_TIMER 4
#define COMMON_MAX_DISPLAY_INTENSITY 4
#define COMMON_MAX_SELECTED_ITEM 5

// ### TYPE DEFINITIONS ###

typedef struct {
    bool led_on;
    uint8_t led_intensity;
    uint8_t led_color;
    uint8_t led_timer;
    uint8_t display_intensity;
    uint8_t selected_item;
} common_settings_t;

// ### GLOBAL TYPES ##

// To be protected by a semaphore

common_settings_t common_settings = {
    .led_on = true,
    .led_intensity=COMMON_MAX_LED_INTENSITY,
    .led_color = 1,
    .led_timer = COMMON_MAX_LED_TIMER,
    .display_intensity = COMMON_MAX_DISPLAY_INTENSITY,
    .selected_item = 1
};

