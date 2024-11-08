#include "esp_timer.h"
#include "esp_err.h"
#include <stdio.h>
#include <inttypes.h>
#include "hp_timer.h"

/*
Different sub-projects to port the Arduino project to ESP-IDF
    Bedclock_IDF_V01 : Test WS2812 led chain
    Bedclock_IDF_V02 : Test SSD1306 OLED screen
    Bedclock_IDF_V03 : Test capacitive touch sensors
    Bedclock_IDF_V04 : Test WiFi
    Bedclock_IDF_V05 : Test time sync with timeserver
    Bedclock_IDF_V06 : Port timer object using esp_timer_get_time() / 1000;
    Bedclock_IDF_V07 : Test FreeRTOS
    Bedclock_IDF_V08 : Implement multiple parallel processes
    Bedclock_IDF_V09 : Internal Espressif SSD1306 driver including LVGL
    Bedclock_IDF_V10 : First working version of application
    Bedclock_IDF_V11 : First working version of pixel buffer
    Bedclock_IDF_V12 : First working version with graphics layout
*/

void app_main() {
    hp_timer_t timer_500_ms_autoreset;
    hp_timer_t timer_1250_ms_no_reset;

    hp_timer_init_ms(&timer_500_ms_autoreset, 500, true);
    hp_timer_init_ms(&timer_1250_ms_no_reset, 1250, false);

    while(true) {
        if(hp_timer_lapsed(&timer_500_ms_autoreset)) printf("%" PRId64 " timer_500_ms_autoreset lapsed\n", esp_timer_get_time() );
        if(hp_timer_lapsed(&timer_1250_ms_no_reset)) printf("%" PRId64 "timer_1250_ms_no_reset lapsed\n", esp_timer_get_time() );
    }
}