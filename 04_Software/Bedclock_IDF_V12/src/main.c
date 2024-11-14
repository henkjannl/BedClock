#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <esp_system.h>
#include <esp_log.h>

#include "bl_common.h"
#include "bl_time.h"
#include "bl_keyboard.h"
#include "bl_display.h"
#include "bl_light.h"
#include "hp_timer.h"

const char *VERSION = "V1.3.0";

/*
Version history
    v1.1.0 Included version number and splash screen
    v1.2.0 Pressing left and right buttons resets the device instead of the display
    v1.3.0 hp_bitmap_t extended with mask
           Fonts in separate directory
           hp_version_font has masks
           Included splash screen on startup

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

To do:
* Consider renaming interpolation library
* Make capacitive touch interrupt controlled
* Only display clock when time is synched
*/

#define mn_tag "main"

void app_main()
{

/*
    // Test stepping module
    hp_stepping_float_t sf_linear;   hp_stepping_float_init(&sf_linear,  0, 1, 100, ST_LINEAR );
    hp_stepping_float_t sf_cycloid;  hp_stepping_float_init(&sf_cycloid, 0, 1, 100, ST_CYCLOID);
    hp_stepping_float_t sf_spring;   hp_stepping_float_init(&sf_spring,  0, 1, 100, ST_SPRING );
    hp_stepping_float_t sf_click;    hp_stepping_float_init(&sf_click,   0, 1, 100, ST_CLICK  );
    hp_stepping_float_t sf_accel;    hp_stepping_float_init(&sf_accel,   0, 1, 100, ST_ACCEL  );
    hp_stepping_float_t sf_damped;   hp_stepping_float_init(&sf_damped,  0, 1, 100, ST_DAMPED );
    hp_stepping_float_t sf_bounce;   hp_stepping_float_init(&sf_bounce,  0, 1, 100, ST_BOUNCE );
    hp_stepping_float_t sf_bow;      hp_stepping_float_init(&sf_bow,     0, 1, 100, ST_BOW    );
    hp_stepping_float_t sf_step;     hp_stepping_float_init(&sf_step,    0, 1, 100, ST_STEP   );

    float o[9];

    for(uint8_t i=0; i<101; i++) {
        o[0] = hp_stepping_float_step(&sf_linear);
        o[1] = hp_stepping_float_step(&sf_cycloid);
        o[2] = hp_stepping_float_step(&sf_spring);
        o[3] = hp_stepping_float_step(&sf_click);
        o[4] = hp_stepping_float_step(&sf_accel);
        o[5] = hp_stepping_float_step(&sf_damped);
        o[6] = hp_stepping_float_step(&sf_bounce);
        o[7] = hp_stepping_float_step(&sf_bow);
        o[8] = hp_stepping_float_step(&sf_step);

        printf("%d,%f,%f,%f,%f,%f,%f,%f,%f,%f\n",
        i, o[0], o[1], o[2], o[3], o[4], o[5], o[6], o[7], o[8]);
    }
    printf("\n\n\n");

    while(true) {};
*/

    common_init();    // initialize queues and read settings from non volatile storage
    display_init(VERSION);
    time_init();      // sync clock with timeserver
    keyboard_init();
    light_init();


    while(true) {
        ESP_LOGI(mn_tag, "Main blip. %s", debug);
		vTaskDelay(pdMS_TO_TICKS(1000));
    }
}