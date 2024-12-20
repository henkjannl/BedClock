#pragma once

#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <esp_system.h>
#include <esp_log.h>

#include <stdio.h>

#include "bl_common.h"

QueueHandle_t display_queue; // Commands for the display
QueueHandle_t light_queue;   // Commands for the light

SemaphoreHandle_t mutex_change_settings;

void queues_init() {
    mutex_change_settings = xSemaphoreCreateMutex();
    display_queue = xQueueCreate(20, sizeof(common_command_t));
    light_queue   = xQueueCreate(20, sizeof(common_command_t));
}

void queue_send_message(QueueHandle_t queue, common_command_t cmd) {
    xQueueSend(queue, &cmd, pdMS_TO_TICKS(0));
}

