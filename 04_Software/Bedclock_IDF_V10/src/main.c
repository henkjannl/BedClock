#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <esp_system.h>
#include <esp_log.h>

#include "bl_common.h"
#include "bl_queues.h"
#include "bl_time.h"
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
Bedclock_IDF_V08 : Implement multiple parallel processes
Bedclock_IDF_V09 : Internal Espressif SSD1306 driver including LVGL
Bedclock_IDF_V10 : First working version
Bedclock_IDF_V11 : Try using ESP-IDF driver for SSD1306

To do:
Init display first, with splash screen
Display with graphics instead of text
Animate graphics on display
Store data in NVS
Make capacitive touch interrupt controlled
Only display clock when time is synched
*/

#define mn_tag "main"

void app_main()
{
    time_init();
    queues_init();
    display_init();
    keyboard_init();
    light_init();

    // hp_timer_t auto_restart_timer;
    // hp_timer_t single_shot_timer;

    // hp_timer_init_ms(&auto_restart_timer,3000, true);
    // hp_timer_init_ms(&single_shot_timer,4500, false);

    while(true) {
        ESP_LOGI(mn_tag, "Main blip");

        // Test the timers
        // if(hp_timer_lapsed(&auto_restart_timer)) ESP_LOGI(mn_tag, "Auto restart timer %" PRId64, auto_restart_timer.last_reset_time);
        // if(hp_timer_lapsed(&single_shot_timer)) ESP_LOGI(mn_tag, "Single shot timer");
		vTaskDelay(pdMS_TO_TICKS(1000));
    }
}