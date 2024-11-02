#pragma once

#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <esp_system.h>
#include <esp_log.h>

#include <stdio.h>

QueueHandle_t keyboard_to_display_queue; // From keyboard to display

typedef int16_t queue_command_t;

const queue_command_t BTN_LEFT  = 1;
const queue_command_t BTN_TOP   = 2;
const queue_command_t BTN_RIGHT = 3;

SemaphoreHandle_t mutex_change_settings;

void queues_init() {
    mutex_change_settings = xSemaphoreCreateMutex();
    keyboard_to_display_queue = xQueueCreate(20, sizeof(queue_command_t));
}