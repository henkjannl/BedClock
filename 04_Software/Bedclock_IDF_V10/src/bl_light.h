#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/rmt_tx.h"
#include "esp_log.h"

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/rmt_tx.h"
#include "led_strip_encoder.h"

#include "bl_common.h"
#include "bl_queues.h"

#define RMT_LED_STRIP_RESOLUTION_HZ 10000000 // 10MHz resolution, 1 tick = 0.1us (led strip needs a high resolution)
#define RMT_LED_STRIP_GPIO_NUM      23

#define MAX_BRIGHTNESS 40

#define EXAMPLE_LED_NUMBERS         16
#define EXAMPLE_CHASE_SPEED_MS      20

const uint8_t led_intensity[] = {
     1,       // .led_intensity = 0
     3,       // .led_intensity = 1
     6,       // .led_intensity = 2
    16,       // .led_intensity = 3
    40,       // .led_intensity = 4
};

const uint32_t led_timer_ms[] = {
     5*60*1000,    // .led_timer = 0
     7*60*1000,    // .led_timer = 1
    10*60*1000,    // .led_timer = 2
    14*60*1000,    // .led_timer = 3
    20*60*1000,    // .led_timer = 4
};

#define lt_tag "light"

static uint8_t led_strip_pixels[EXAMPLE_LED_NUMBERS * 3];

void task_light(void *arg)
{
    ESP_LOGI(lt_tag, "Light task started...");

    ESP_LOGI(lt_tag, "Create RMT TX channel");
    rmt_channel_handle_t led_chan = NULL;
    rmt_tx_channel_config_t tx_chan_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT, // select source clock
        .gpio_num = RMT_LED_STRIP_GPIO_NUM,
        .mem_block_symbols = 64, // increase the block size can make the LED less flickering
        .resolution_hz = RMT_LED_STRIP_RESOLUTION_HZ,
        .trans_queue_depth = 4, // set the number of transactions that can be pending in the background
    };
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &led_chan));

    ESP_LOGI(lt_tag, "Install led strip encoder");
    rmt_encoder_handle_t led_encoder = NULL;
    led_strip_encoder_config_t encoder_config = {
        .resolution = RMT_LED_STRIP_RESOLUTION_HZ,
    };
    ESP_ERROR_CHECK(rmt_new_led_strip_encoder(&encoder_config, &led_encoder));

    ESP_LOGI(lt_tag, "Enable RMT TX channel");
    ESP_ERROR_CHECK(rmt_enable(led_chan));

    ESP_LOGI(lt_tag, "Start LED rainbow chase");
    rmt_transmit_config_t tx_config = {
        .loop_count = 0, // no transfer loop
    };

    common_command_t cmd;
    bool update_light = false;
    bool led_on = true;

    uint32_t red = 0;
    uint32_t green = 0;
    uint32_t blue = 0;

    hp_timer_t led_off_timer;
    hp_timer_init_ms(&led_off_timer, led_timer_ms[common_settings.led_timer], false);

    while(true) {
        if (xQueueReceive(keyboard_to_light_queue, &cmd, pdMS_TO_TICKS(100)) == true) {

            switch(cmd) {
                case CMD_BTN_TOP_PRESSED:
                    led_on = !led_on;
                    ESP_LOGI(lt_tag, "Light manually switched %s", led_on ? "on" : "off");                     
                    hp_timer_reset(&led_off_timer);
                    update_light = true;
                break;

                case CMD_SETTINGS_CHANGED:
                    hp_timer_reset(&led_off_timer);
                    update_light = true;
                break; 

                default:
                    ESP_LOGI(lt_tag, "Command not recognized");                     
            }
        }

        if(hp_timer_lapsed(&led_off_timer)) {
            ESP_LOGI(lt_tag, "Light switched off by timer");                     
            led_on = false;
            update_light = true;
        }

        if(update_light) {

            green = 0;
            blue = 0;
            red = 0;

            if (xSemaphoreTake(mutex_change_settings, portMAX_DELAY) == pdTRUE) {
                if(led_on) {
                    green = led_intensity[common_settings.led_intensity];
                    blue = led_intensity[common_settings.led_intensity];
                    red = led_intensity[common_settings.led_intensity];
                }
                hp_set_interval_ms(&led_off_timer, led_timer_ms[common_settings.led_timer]);
                xSemaphoreGive(mutex_change_settings);

            }                    

            for (int i = 0; i < 3; i++) {
                for (int j = i; j < EXAMPLE_LED_NUMBERS; j += 3) {
                    led_strip_pixels[j * 3 + 0] = green;
                    led_strip_pixels[j * 3 + 1] = blue;
                    led_strip_pixels[j * 3 + 2] = red;
                }
                // Flush RGB values to LEDs
                ESP_ERROR_CHECK(rmt_transmit(led_chan, led_encoder, led_strip_pixels, sizeof(led_strip_pixels), &tx_config));
                ESP_ERROR_CHECK(rmt_tx_wait_all_done(led_chan, portMAX_DELAY));
                // vTaskDelay(pdMS_TO_TICKS(EXAMPLE_CHASE_SPEED_MS));
                // memset(led_strip_pixels, 0, sizeof(led_strip_pixels));
                // ESP_ERROR_CHECK(rmt_transmit(led_chan, led_encoder, led_strip_pixels, sizeof(led_strip_pixels), &tx_config));
                // ESP_ERROR_CHECK(rmt_tx_wait_all_done(led_chan, portMAX_DELAY));
            }
        } // if(update_light) 
        vTaskDelay(pdMS_TO_TICKS(100));
    }  // while true
}  // task_light(void *arg)

void light_init(void)
{
    ESP_LOGI(lt_tag, "Starting light task...");
    xTaskCreatePinnedToCore(task_light,    // TaskFunction_t pxTaskCode,
                            "task_light",  // const char * const pcName,
                            4096,           // const uint32_t ulStackDepth,
                            // 16384,            // const uint32_t ulStackDepth,
                            NULL,             // void * const pvParameters,
                            2,                // UBaseType_t uxPriority,
                            NULL,             // TaskHandle_t * const pxCreatedTask,
                            0);               // const BaseType_t xCoreID );

}