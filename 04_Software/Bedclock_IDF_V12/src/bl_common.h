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

#define nvs_namespace "bl_backlight" // for non volatile storage

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
  CMD_TIMER_CHANGED,
  CMD_LIGHT_SWITCHES_ON,
  CMD_LIGHT_SWITCHES_OFF,
  CMD_ADJUST_LIGHT,
} common_command_t;

typedef struct {
    uint8_t led_intensity;
    uint8_t led_color;
    uint8_t led_timer;
    uint8_t display_intensity;

    // not stored in non volatile storage
    bool led_on;
    uint8_t selected_item;
    bool settings_changed;  // determines if we need to save data to non volatile storage
} common_settings_t;

// Global variables
char debug[128] = "";
SemaphoreHandle_t mutex_change_settings;

common_settings_t common_settings = {
    .led_intensity=0,
    .led_color = 0,
    .led_timer = 3,
    .display_intensity = COMMON_MAX_DISPLAY_INTENSITY,

    // Not stored in non volatile storage
    .led_on = false,
    .selected_item = 1,
    .settings_changed = false
};

QueueHandle_t display_queue; // Commands for the display
QueueHandle_t light_queue;   // Commands for the light

// Helper function to send a command from one process to another
void queue_send_message(QueueHandle_t queue, common_command_t cmd) {
    xQueueSend(queue, &cmd, pdMS_TO_TICKS(0));
}

// Change individual settings
bool common_increment_led_intensity() {
    if (xSemaphoreTake(mutex_change_settings, portMAX_DELAY) == pdTRUE) {
        common_settings.led_intensity++;
        if(common_settings.led_intensity>COMMON_MAX_LED_INTENSITY)
            common_settings.led_intensity=0;
        common_settings.settings_changed = true;
        xSemaphoreGive(mutex_change_settings);
        return true;
    }
    else return false; // No success in changing value
}

bool common_increment_led_color() {
    if (xSemaphoreTake(mutex_change_settings, portMAX_DELAY) == pdTRUE) {
        common_settings.led_color++;
        if(common_settings.led_color>COMMON_MAX_LED_COLOR)
            common_settings.led_color=0;
        common_settings.settings_changed = true;
        xSemaphoreGive(mutex_change_settings);
        return true;
    }
    else return false; // No success in changing value
}

bool common_increment_led_timer() {
    if (xSemaphoreTake(mutex_change_settings, portMAX_DELAY) == pdTRUE) {
        common_settings.led_timer++;
        if(common_settings.led_timer>COMMON_MAX_LED_TIMER)
            common_settings.led_timer=0;
        common_settings.settings_changed = true;
        xSemaphoreGive(mutex_change_settings);
        return true;
    }
    else return false; // No success in changing value
}

bool common_increment_display_intensity() {
    if (xSemaphoreTake(mutex_change_settings, portMAX_DELAY) == pdTRUE) {
        common_settings.display_intensity++;
        if(common_settings.display_intensity>COMMON_MAX_DISPLAY_INTENSITY)
            common_settings.display_intensity=0;
        common_settings.settings_changed = true;
        xSemaphoreGive(mutex_change_settings);
        return true;
    }
    else return false; // No success in changing value
}

bool common_change_led_on(bool led_on) {
    if (xSemaphoreTake(mutex_change_settings, portMAX_DELAY) == pdTRUE) {
        common_settings.led_on=led_on;
        xSemaphoreGive(mutex_change_settings);
        return true;
    }
    else return false;
}

bool common_change_selected_item(uint8_t selected_item) {
    if (xSemaphoreTake(mutex_change_settings, portMAX_DELAY) == pdTRUE) {
        common_settings.selected_item=selected_item;
        xSemaphoreGive(mutex_change_settings);
        return true;
    }
    else return false;
}

bool common_increment_selected_item() {
    if (xSemaphoreTake(mutex_change_settings, portMAX_DELAY) == pdTRUE) {
        common_settings.selected_item++;
        // Do not switch back to 0, but show clock
        if(common_settings.selected_item>COMMON_MAX_SELECTED_ITEM)
            common_settings.selected_item=1;
        xSemaphoreGive(mutex_change_settings);
        return true;
    }
    else return false;
}

uint8_t common_get_selected_item() {
    uint8_t result = 0;
    if (xSemaphoreTake(mutex_change_settings, portMAX_DELAY) == pdTRUE) {
        result = common_settings.selected_item;
        xSemaphoreGive(mutex_change_settings);
    }
    return result;
}

uint8_t common_get_display_intensity() {
    uint8_t result = 0;
    if (xSemaphoreTake(mutex_change_settings, portMAX_DELAY) == pdTRUE) {
        result = common_settings.display_intensity;
        xSemaphoreGive(mutex_change_settings);
    }
    return result;
}

// Loading data from non volatile storage
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

    err = nvs_get_u8(nvs_handle, "disp_int", &common_settings.display_intensity);

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

// Saving data to non volatile storage
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
    nvs_set_u8(nvs_handle, "led_intensity",     common_settings.led_intensity    );
    nvs_set_u8(nvs_handle, "led_color",         common_settings.led_color        );
    nvs_set_u8(nvs_handle, "led_timer",         common_settings.led_timer        );
    err = nvs_set_u8(nvs_handle, "disp_int", common_settings.display_intensity);

    switch (err) {
        case ESP_OK:
            ESP_LOGI(cm_tag, "Display intensity written to NVS");
            break;
        case ESP_FAIL:
            ESP_LOGI(cm_tag, "Display intensity error ESP_FAIL");
            break;
        case ESP_ERR_NVS_INVALID_HANDLE:
            ESP_LOGI(cm_tag, "Display intensity error ESP_ERR_NVS_INVALID_HANDLE");
            break;
        case ESP_ERR_NVS_READ_ONLY:
            ESP_LOGI(cm_tag, "Display intensity error ESP_ERR_NVS_READ_ONLY");
            break;
        case ESP_ERR_NVS_INVALID_NAME:
            ESP_LOGI(cm_tag, "Display intensity error ESP_ERR_NVS_INVALID_NAME");
            break;
        case ESP_ERR_NVS_NOT_ENOUGH_SPACE:
            ESP_LOGI(cm_tag, "Display intensity error ESP_ERR_NVS_NOT_ENOUGH_SPACE");
            break;
        case ESP_ERR_NVS_REMOVE_FAILED:
            ESP_LOGI(cm_tag, "Display intensity error ESP_ERR_NVS_REMOVE_FAILED");
            break;
        default :
            ESP_LOGI(cm_tag, "Error (%s) reading!\n", esp_err_to_name(err));
    }

    nvs_commit(nvs_handle);
    common_settings.settings_changed = false;

    xSemaphoreGive(mutex_change_settings);
  }

  nvs_close(nvs_handle);
}
