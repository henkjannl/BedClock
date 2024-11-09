#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <esp_system.h>
#include "nvs_flash.h"
#include "nvs.h"
#include <esp_log.h>

#define cm_tag "common"
#define nvs_namespace "bl_backlight"

/* Selected item:
  0: Sleep mode
  1: Display time
  2: Modify LED intensity
  3: Modify LED color
  4: Modify LED timer
*/

#define COMMON_MAX_LED_INTENSITY      4
#define COMMON_MAX_LED_COLOR          3
#define COMMON_MAX_LED_TIMER          4
#define COMMON_MAX_DISPLAY_INTENSITY  2
#define COMMON_MAX_SELECTED_ITEM      5

// ### TYPE DEFINITIONS ###
typedef enum {
  CMD_NONE,
  CMD_BTN_LEFT_PRESSED,
  CMD_BTN_TOP_PRESSED,
  CMD_BTN_RIGHT_PRESSED,
  CMD_SETTINGS_CHANGED,
  CMD_LIGHT_SWITCHED_ON,
  CMD_ADJUST_LIGHT,
  CMD_RESET_DISPLAY
} common_command_t;

typedef struct {
    uint8_t led_intensity;
    uint8_t led_color;
    uint8_t led_timer;
    uint8_t display_intensity;
    uint8_t selected_item;  // not stored in non volatile storage
    bool settings_changed;  // determines if we need to save data to non volatile storage
} common_settings_t;

char debug[128] = "";

// ### GLOBAL TYPES ##

SemaphoreHandle_t mutex_change_settings;

// Must be protected by a semaphore
// Default values are defined here
common_settings_t common_settings = {
    .led_intensity=0,
    .led_color = 0,
    .led_timer = 3,
    .display_intensity = COMMON_MAX_DISPLAY_INTENSITY,
    .selected_item = 1,
    .settings_changed = false
};

QueueHandle_t display_queue; // Commands for the display
QueueHandle_t light_queue;   // Commands for the light

void queue_send_message(QueueHandle_t queue, common_command_t cmd) {
    xQueueSend(queue, &cmd, pdMS_TO_TICKS(0));
}

void common_init() {
    mutex_change_settings = xSemaphoreCreateMutex();
    display_queue = xQueueCreate(20, sizeof(common_command_t));
    light_queue   = xQueueCreate(20, sizeof(common_command_t));

    ESP_LOGI(cm_tag, "Reading variables from non volatile storage");
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK( err );

    nvs_handle_t nvs_handle;
    ESP_ERROR_CHECK(nvs_open(nvs_namespace, NVS_READWRITE, &nvs_handle));

    if (xSemaphoreTake(mutex_change_settings, portMAX_DELAY) == pdTRUE) {

    // Read
    err = nvs_get_u8(nvs_handle, "led_intensity", &common_settings.led_intensity);

    switch (err) {
        case ESP_OK:
            ESP_LOGI(cm_tag, "Read LED intensity");
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            ESP_LOGI(cm_tag, "LED intensity not initialized yet");
            break;
        default :
            ESP_LOGI(cm_tag, "Error (%s) reading!\n", esp_err_to_name(err));
    }

    err = nvs_get_u8(nvs_handle, "led_color", &common_settings.led_color );

    switch (err) {
        case ESP_OK:
            ESP_LOGI(cm_tag, "Read LED color");
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            ESP_LOGI(cm_tag, "LED color not initialized yet");
            break;
        default :
            ESP_LOGI(cm_tag, "Error (%s) reading!\n", esp_err_to_name(err));
    }
    err = nvs_get_u8(nvs_handle, "led_timer", &common_settings.led_timer);

    switch (err) {
        case ESP_OK:
            ESP_LOGI(cm_tag, "Read LED timer");
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            ESP_LOGI(cm_tag, "LED timer not initialized yet");
            break;
        default :
            ESP_LOGI(cm_tag, "Error (%s) reading!\n", esp_err_to_name(err));
    }

    err = nvs_get_u8(nvs_handle, "display_intensity", &common_settings.display_intensity);

    switch (err) {
        case ESP_OK:
            ESP_LOGI(cm_tag, "Read display intensity");
            break;
        case ESP_ERR_NVS_NOT_FOUND:
            ESP_LOGI(cm_tag, "Display intensity not initialized yet");
            break;
        default :
            ESP_LOGI(cm_tag, "Error (%s) reading!\n", esp_err_to_name(err));
    }

    common_settings.settings_changed = false;
    xSemaphoreGive(mutex_change_settings);
  }

  nvs_close(nvs_handle);
}

void common_save() {
  // Only save settings if settings were changed
  // single boolean can be read without using semaphore
  if(!common_settings.settings_changed) return;

  ESP_LOGI(cm_tag, "Saving variables to non volatile storage");

  esp_err_t err = nvs_flash_init();
  if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND) {
    // NVS partition was truncated and needs to be erased
    // Retry nvs_flash_init
    ESP_ERROR_CHECK(nvs_flash_erase());
    err = nvs_flash_init();
  }
  ESP_ERROR_CHECK( err );

  nvs_handle_t nvs_handle;
  nvs_open(nvs_namespace, NVS_READWRITE, &nvs_handle);

  if (xSemaphoreTake(mutex_change_settings, portMAX_DELAY) == pdTRUE) {
    // Write
    nvs_set_u8(nvs_handle, "led_intensity", common_settings.led_intensity);
    nvs_set_u8(nvs_handle, "led_color",     common_settings.led_color);
    nvs_set_u8(nvs_handle, "led_timer",     common_settings.led_timer);
    nvs_set_u8(nvs_handle, "display_intensity", common_settings.display_intensity);
    common_settings.settings_changed = false;

    xSemaphoreGive(mutex_change_settings);
  }

  nvs_close(nvs_handle);
}
