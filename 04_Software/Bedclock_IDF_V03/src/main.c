/*
 * SPDX-FileCopyrightText: 2021-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

/*
This is a demo for the touch sensor functionality of ESP32
Practical hints:
Examples are at https://github.com/espressif/esp-idf/tree/v5.2.3/examples/peripherals/touch_sensor
Not every example is compatible with all ESP32 versions. See:
https://github.com/espressif/esp-idf/blob/v5.2.3/examples/peripherals/touch_sensor/touch_periph_version.txt

The watchdog timer can cause problems. Be sure to disable it in menuconfig
Component config --> ESP System Settings --> Interrupt watchdog
Component config --> ESP System Settings --> Enable Task Watchdog Timer

Probably due to the internal wiring, different sensors may have a different open capacitance, and may therefore need
a different threshold value
*/
#include <stdio.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/touch_pad.h"
#include "esp_log.h"
#include "esp_task_wdt.h"

#define TOUCH_PAD_NO_CHANGE   (-1)
#define TOUCH_THRESH_NO_USE   (0)
#define TOUCH_FILTER_MODE_EN  (1)
#define TOUCHPAD_FILTER_TOUCH_PERIOD (10)

#define MAX_TOUCH_BTN 3

typedef struct {
  uint8_t touch_id;
  uint16_t threshold;
  const char *id;
} touch_button_t;

touch_button_t touch_buttons[] = {
    { .touch_id = 0, .threshold = 1000, .id = "L"},
    { .touch_id = 6, .threshold = 1000, .id = "T"},
    { .touch_id = 5, .threshold =  800, .id = "R"}
};

/*
  Read values sensed at all available touch pads.
 Print out values in a loop on a serial monitor.
 */
static void tp_example_read_task(void *pvParameter)
{
    // uint16_t touch_value;
    uint16_t touch_filter_value;
    uint8_t btn_pressed;
#if TOUCH_FILTER_MODE_EN
    printf("Touch Sensor filter mode read, the output format is: \nTouchpad num:[raw data, filtered data]\n\n");
#else
    printf("Touch Sensor normal mode read, the output format is: \nTouchpad num:[raw data]\n\n");
#endif
    while (1) {

        for (uint8_t btn_id=0; btn_id<MAX_TOUCH_BTN; btn_id++) {
            touch_pad_read_filtered(touch_buttons[btn_id].touch_id, &touch_filter_value);
            btn_pressed = (touch_filter_value<touch_buttons[btn_id].threshold);
            printf("T%d:[%4"PRIu16"] %s ", touch_buttons[btn_id].touch_id, touch_filter_value, btn_pressed? touch_buttons[btn_id].id : " ");
}

//         btn_pressed = 0;
//         for (int i = 0; i < TOUCH_PAD_MAX; i++) {
// #if TOUCH_FILTER_MODE_EN
//             // If open the filter mode, please use this API to get the touch pad count.
//             // touch_pad_read_raw_data(i, &touch_value);
//             // touch_pad_read_filtered(i, &touch_filter_value);
//             // printf("T%d:[%4"PRIu16",%4"PRIu16"] ", i, touch_value, touch_filter_value);

//             // touch_pad_read_raw_data(i, &touch_value);
//             touch_pad_read_filtered(i, &touch_filter_value);
//             printf("T%d:[%4"PRIu16"] ", i, touch_filter_value);

//             if( (i==0) && (touch_filter_value<1000) ) { btn_pressed += 1; };
//             if( (i==6) && (touch_filter_value<1000) ) { btn_pressed += 2; };
//             if( (i==5) && (touch_filter_value< 800) ) { btn_pressed += 4; };

// #else
//             touch_pad_read(i, &touch_value);
//             printf("T%d:[%4"PRIu16"] ", i, touch_value);
// #endif
//         }
//         printf("{%1"PRIu16"} ", btn_pressed);

        printf("\n");
        vTaskDelay(200 / portTICK_PERIOD_MS);
    }
}

static void tp_example_touch_pad_init(void)
{
    for (int i = 0; i < TOUCH_PAD_MAX; i++) {
        touch_pad_config(i, TOUCH_THRESH_NO_USE);
    }
}

void app_main(void)
{
    // Initialize touch pad peripheral.
    // The default fsm mode is software trigger mode.
    ESP_ERROR_CHECK(touch_pad_init());
    // Set reference voltage for charging/discharging
    // In this case, the high reference valtage will be 2.7V - 1V = 1.7V
    // The low reference voltage will be 0.5
    // The larger the range, the larger the pulse count value.
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);
    tp_example_touch_pad_init();
#if TOUCH_FILTER_MODE_EN
    touch_pad_filter_start(TOUCHPAD_FILTER_TOUCH_PERIOD);
#endif
    // Start task to read values sensed by pads
    xTaskCreate(&tp_example_read_task, "touch_pad_read_task", 4096, NULL, 5, NULL);

    while(1) {

    }
}