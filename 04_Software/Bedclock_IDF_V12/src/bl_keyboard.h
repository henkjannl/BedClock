#pragma once

#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <esp_system.h>
#include <esp_log.h>
#include "esp_timer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "driver/touch_pad.h"

#include "bl_common.h"

#define TOUCH_PAD_NO_CHANGE            -1
#define TOUCH_THRESH_NO_USE             0
#define TOUCHPAD_FILTER_TOUCH_PERIOD   10
#define TOUCHPAD_THRESHOLD            671

#define TOUCH_LEFT  0   //  Left button is GPIO  4 = Touch0
#define TOUCH_TOP   6   //   Top button is GPIO 14 = Touch6
#define TOUCH_RIGHT 5   // Right button is GPIO 12 = Touch5

#define PRESSED_TOGETHER_THRESHOLD 500*1000 // 500 ms between pressing left and right button
#define kb_tag "keyboard"

void task_keyboard(void *arg)
{
    uint16_t touch_value;
    uint16_t count_btn_left = 0;
    uint16_t count_btn_top = 0;
    uint16_t count_btn_right = 0;
    int64_t last_time_left_pressed = 0;
    int64_t last_time_right_pressed = 0;

    // Filter makes the FreeRTOS task crash
    // touch_pad_filter_start(TOUCHPAD_FILTER_TOUCH_PERIOD);

    ESP_LOGI(kb_tag, "Keyboard task started...");

    while(true) {
        // ESP_LOGI(kb_tag, "Keyboard Blip");

		// Read left button
		touch_pad_read(TOUCH_LEFT, &touch_value);
        if(touch_value<TOUCHPAD_THRESHOLD)
            count_btn_left++;
        else
            count_btn_left=0;

        if(count_btn_left==3) {
            ESP_LOGI(kb_tag, "Left button filtered value %d", touch_value);

            // Left and right buttons were pressed together
            last_time_left_pressed = esp_timer_get_time();
            if( (last_time_left_pressed-last_time_right_pressed) < PRESSED_TOGETHER_THRESHOLD)
                queue_send_message(display_queue, CMD_RESET_DISPLAY);
            else
                queue_send_message(display_queue, CMD_BTN_LEFT_PRESSED);
        }

		// Read top button
		touch_pad_read(TOUCH_TOP, &touch_value);
        if(touch_value<TOUCHPAD_THRESHOLD)
            count_btn_top++;
        else
            count_btn_top=0;

        if(count_btn_top==3) {
            ESP_LOGI(kb_tag, "Top button filtered value %d", touch_value);
            queue_send_message(light_queue, CMD_BTN_TOP_PRESSED);
        }

		// Read right button
		touch_pad_read(TOUCH_RIGHT, &touch_value);
        if(touch_value<TOUCHPAD_THRESHOLD)
            count_btn_right++;
        else
            count_btn_right=0;

        if(count_btn_right==3) {
            ESP_LOGI(kb_tag, "Right button filtered value %d", touch_value);

            // Left and right buttons were pressed together
            last_time_right_pressed = esp_timer_get_time();
            if( (last_time_right_pressed-last_time_left_pressed) < PRESSED_TOGETHER_THRESHOLD) {
                // queue_send_message(display_queue, CMD_RESET_DISPLAY);
                esp_restart();
            } else {
                queue_send_message(display_queue, CMD_BTN_RIGHT_PRESSED);
            }
        }

        vTaskDelay(pdMS_TO_TICKS(50));

    } // while 1
}

void keyboard_init() {
    ESP_ERROR_CHECK(touch_pad_init());

    // Set reference voltage for charging/discharging
    // In this case, the high reference valtage will be 2.7V - 1V = 1.7V
    // The low reference voltage will be 0.5
    // The larger the range, the larger the pulse count value.
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);

	touch_pad_config(TOUCH_LEFT,  TOUCH_THRESH_NO_USE);
	touch_pad_config(TOUCH_TOP,   TOUCH_THRESH_NO_USE);
	touch_pad_config(TOUCH_RIGHT, TOUCH_THRESH_NO_USE);

    // touch_pad_filter_start(TOUCHPAD_FILTER_TOUCH_PERIOD);

    ESP_LOGI(kb_tag, "Starting keyboard task...");
    xTaskCreatePinnedToCore(task_keyboard,    // TaskFunction_t pxTaskCode,
                            "task_keyboard",  // const char * const pcName,
                            4096,           // const uint32_t ulStackDepth,
                            // 16384,            // const uint32_t ulStackDepth,
                            NULL,             // void * const pvParameters,
                            2,                // UBaseType_t uxPriority,
                            NULL,             // TaskHandle_t * const pxCreatedTask,
                            0);               // const BaseType_t xCoreID );
}