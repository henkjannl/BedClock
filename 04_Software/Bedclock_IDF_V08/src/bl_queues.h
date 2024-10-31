#pragma once

#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <esp_system.h>
#include <esp_log.h>

#include <stdio.h>

QueueHandle_t keyboard_to_display_queue; // From keyboard to display

void queues_init() {
    keyboard_to_display_queue = xQueueCreate(10, sizeof(uint32_t));
}