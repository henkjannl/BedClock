#pragma once

#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <esp_system.h>
#include <esp_log.h>

#include <stdio.h>

#include "bl_queues.h"

#define tag "keyboard"

void task_keyboard(void*arg)
{
    uint32_t snd = 0;

    while(1)
    {
        queue_command_t btn=0;

        switch(snd++) {
            case 0:
                btn = BTN_LEFT; 
                ESP_LOGI(tag, "0: Left button, select item 1 to display time");
            break;

            case 1:
                btn = BTN_LEFT; 
                ESP_LOGI(tag, "1: Left button, select item 2 to display intensity");
            break;

            case 2:
                btn = BTN_RIGHT; 
                ESP_LOGI(tag, "2: Right button, increase intensity");
            break;

            case 3:
                btn = BTN_RIGHT; 
                ESP_LOGI(tag, "3: Right button, increase intensity");
            break;

            case 4:
                btn = BTN_RIGHT; 
                ESP_LOGI(tag, "4: Right button, increase intensity");
            break;

            case 5:
                btn = BTN_LEFT; 
                ESP_LOGI(tag, "5: Left button, select item color");
            break;

            case 6:
                btn = BTN_RIGHT; 
                ESP_LOGI(tag, "6: Right button, increase color");
            break;

            case 7:
                btn = BTN_RIGHT; 
                ESP_LOGI(tag, "7: Right button, increase color");
            break;

            case 8:
                btn = BTN_LEFT; 
                ESP_LOGI(tag, "8: Left button, select item timer");
            break;

            case 9:
                btn = BTN_RIGHT; 
                ESP_LOGI(tag, "9: Right button, increase timer");
            break;

            case 10:
                btn = BTN_RIGHT; 
                ESP_LOGI(tag, "10: Right button, increase timer");
            break;
        }

        if(btn>0) xQueueSend(keyboard_to_display_queue, &btn, pdMS_TO_TICKS(0));

        vTaskDelay(pdMS_TO_TICKS(5000));
    }
}

void keyboard_init() {
    xTaskCreatePinnedToCore(task_keyboard, "task_keyboard", 4096, NULL, 1, NULL, 0);
}