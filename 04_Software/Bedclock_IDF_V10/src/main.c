#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <esp_system.h>
#include <esp_log.h>

#include "bl_common.h"
#include "bl_queues.h"
#include "bl_keyboard.h"
#include "bl_display.h"
#include "hp_timer.h"

/*
Bedclock_IDF_V01 : Test WS2812 led chain
Bedclock_IDF_V02 : Test SSD1306 OLED screen
Bedclock_IDF_V03 : Test capacitive touch sensors
Bedclock_IDF_V04 : Test WiFi
Bedclock_IDF_V05 : Test time sync with timeserver
Bedclock_IDF_V06 : Port timer object using esp_timer_get_time() / 1000;
Bedclock_IDF_V07 : Test FreeRTOS
Bedclock_IDF_V08 : Port current WS2812 functionality to ESP-IDF
Bedclock_IDF_V09 : Internal Espressif SSD1306 driver including LVGL
Bedclock_IDF_V10 : Create menu structure
*/

void app_main()
{
    queues_init();
    display_init();
    keyboard_init();
}