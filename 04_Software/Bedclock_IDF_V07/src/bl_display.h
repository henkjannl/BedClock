#pragma once

#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <esp_system.h>
#include <esp_log.h>

#include <stdio.h>

#include "bl_queues.h"

void task_display(void*z)
{
    uint32_t rcv = 0;
    while(1)
    {
        if (xQueueReceive(keyboard_to_display_queue, &rcv, pdMS_TO_TICKS(1000)) == true) 
        {
            printf("Display: Item received: %d\n", (int) rcv);
        }
        else
        {
            printf("Display: No item received. Timeout.\n");
        }
    }
}

void display_init() {
    xTaskCreatePinnedToCore(task_display, "task_display", 4096, NULL, 1, NULL, 0);
}