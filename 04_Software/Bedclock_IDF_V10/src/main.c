#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <esp_system.h>
#include <esp_log.h>

#include "bl_common.h"
#include "bl_queues.h"
#include "sync_clock.h"
#include "bl_keyboard.h"
#include "bl_display.h"
#include "bl_light.h"
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
Bedclock_IDF_V10 : First working version

To do:
Implement LED color
Implement gradual LED change
Implement display intensity
Init display first, with splash screen
Display with graphics instead of text
Animate graphics on display
Store data in NVS
Make capacitive touch interrupt controlled
*/

#define mn_tag "main"

void app_main()
{
    time_init();
    queues_init();
    display_init();
    keyboard_init();
    light_init();

    while(true) {
        // ESP_LOGI(mn_tag, "Keyboard %d Display%d", keyboard_counter, display_counter);
        // keyboard_counter=0;
        // display_counter=0;
        ESP_LOGI(mn_tag, "Main blip");
		vTaskDelay(pdMS_TO_TICKS(1000));
    }
}