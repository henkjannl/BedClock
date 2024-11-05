#pragma once

#include <stdio.h>
#include <stdlib.h> 

/* Selected item:
  0: Sleep mode
  1: Display time
  2: Modify LED intensity
  3: Modify LED color 
  4: Modify LED timer
  5: Modify display intensity
*/

#define COMMON_MAX_LED_INTENSITY      4
#define COMMON_MAX_LED_COLOR          3
#define COMMON_MAX_LED_TIMER          4
#define COMMON_MAX_DISPLAY_INTENSITY  4
#define COMMON_MAX_SELECTED_ITEM      5

// ### TYPE DEFINITIONS ###
typedef enum {
  CMD_NONE,
  CMD_BTN_LEFT_PRESSED,
  CMD_BTN_TOP_PRESSED,
  CMD_BTN_RIGHT_PRESSED,
  CMD_SETTINGS_CHANGED,
  CMD_LIGHT_SWITCHED_ON
} common_command_t;

typedef struct {
    uint8_t led_intensity;
    uint8_t led_color;
    uint8_t led_timer;
    uint8_t display_intensity;
    uint8_t selected_item;
} common_settings_t;

// ### GLOBAL TYPES ##

// Must be protected by a semaphore
// Default values are defined here
common_settings_t common_settings = {
    .led_intensity=0,
    .led_color = 0,
    .led_timer = 3,
    .display_intensity = COMMON_MAX_DISPLAY_INTENSITY,
    .selected_item = 1
};


// Function to create a random number between 0 and 1
float random_float() {
   return (float)rand() / (float)RAND_MAX;
};
