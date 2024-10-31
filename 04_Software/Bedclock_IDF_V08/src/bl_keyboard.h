#pragma once

#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <esp_system.h>
#include <esp_log.h>

#include <stdio.h>

#include "bl_queues.h"

void task_keyboard(void*arg)
{
    uint32_t snd = 0;
    while(1)
    {
        if (snd < 15)
        {
            xQueueSend(keyboard_to_display_queue, &snd, pdMS_TO_TICKS(0));
            snd++;
        }
        else
        {
            vTaskDelay(pdMS_TO_TICKS(5000));
            snd = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void keyboard_init() {
    xTaskCreatePinnedToCore(task_keyboard, "task_keyboard", 4096, NULL, 1, NULL, 0);
}