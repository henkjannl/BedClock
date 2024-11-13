#include <string.h>
#include <math.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/rmt_tx.h"
#include "esp_log.h"

#include "esp_timer.h"
#include "esp_err.h"
#include <stdio.h>

#include "led_strip_encoder.h"

#include "bl_common.h"
#include "hp_stepping_float.h"

#define lt_tag "light"

#define RMT_LED_STRIP_RESOLUTION_HZ 10000000 // 10MHz resolution, 1 tick = 0.1us (led strip needs a high resolution)
#define RMT_LED_STRIP_GPIO_NUM      23
#define LED_NUM_LEDS                16
#define LED_MAX_BRIGHTNESS          40

const float LED_INTENSITY[] = {
    0.025,  // .led_intensity = 0
    0.063,  // .led_intensity = 1
    0.158,  // .led_intensity = 2
    0.398,  // .led_intensity = 3
    1.000,  // .led_intensity = 4
};

const uint32_t LED_TIMER_MS[] = {
     5*60*1000,    // .led_timer = 0
     7*60*1000,    // .led_timer = 1
    10*60*1000,    // .led_timer = 2
    14*60*1000,    // .led_timer = 3
    20*60*1000,    // .led_timer = 4
};

typedef struct {
    float r,g,b;
} led_color_t;

// Average of all channels is 1
const led_color_t LED_COLORS[] = {
  { .r = 1.000,  .g = 1.000,  .b = 1.000 },    // 0 = White
  { .r = 1.179,  .g = 1.151,  .b = 0.670 },    // 1 = Yellow
  { .r = 1.281,  .g = 0.990,  .b = 0.729 },    // 2 = Orange
  { .r = 1.600,  .g = 0.700,  .b = 0.700 },    // 3 = Red
};

/*
LED algorithm

example:
- LEDs are currently off
- maximum brightness is 40
- target intensity is 2 (=0.16)
- target color is 2 (orange: r=1.333 g=1.000 b=0.667)
- light switches on in 20 steps

  red channel is stepping from 0 to 0.16*1.333*40 = 8.433 with steps of 0.422
green channel is stepping from 0 to 0.16*1.000*40 = 6.325 with steps of 0.316
 blue channel is stepping from 0 to 0.16*0.667*40 = 4.216 with steps of 0.211

Values of the red channel: 0.00, 0.42, 0.84, 1.26, 1.69, 2.11, 2.53, 2.95, 3.37, etc.

To produce non-integer intensities, a fraction of the 16 LEDs switch to the next value
*/


// This function determines the intensity of the LEDs
uint8_t led_value(double value, uint16_t mask_bit) {

    // Ensure the light goes off when supposed to
    // to prevent one led remains on
    if(value<0.07) return 0; // little more than 1/16

    // The order in which the LEDs in the 4x4 grid are switched on
    const uint16_t SUB_BRIGHTNESS[] = {0x0000, 0x0008, 0x0208, 0x020A, 0x0A0A, 0x2A0A, 0x2A8A, 0x2AAA, 0xAAAA,
        0xAABA, 0xABBA, 0xABBE, 0xEBBE, 0xEBBF, 0xFBBF, 0xFBFF };

    // Round value used as the intensity of the LED
    uint8_t i= (int) value;

    // The fraction determines if the LEDs intensity is increased
    // to approximate the requested average intensity of the panel
    uint8_t remainder = (uint8_t) 16*(value-i);
    if( SUB_BRIGHTNESS[remainder] & mask_bit ) i++;

    return i;
};

void task_light(void *arg) {
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
    bool update_light = true;

    hp_stepping_float_t red   = HP_STEPPING_FLOAT_INIT;
    hp_stepping_float_t green = HP_STEPPING_FLOAT_INIT;
    hp_stepping_float_t blue  = HP_STEPPING_FLOAT_INIT;

    common_settings_t copy_of_settings;

    hp_timer_t led_off_timer;
    hp_timer_init_ms(&led_off_timer, LED_TIMER_MS[common_settings.led_timer], false);

    // int64_t t1, t2; // Measure time for debug purposes

    while(true) {
        if (xQueueReceive(light_queue, &cmd, 0) == true) {

            switch(cmd) {
                case CMD_BTN_TOP_PRESSED:
                    // Single bit does not need protection by semaphore
                    common_settings.led_on = !common_settings.led_on;

                    if(common_settings.led_on) {
                        ESP_LOGI(lt_tag, "Light manually switched on");
                        queue_send_message(display_queue, CMD_LIGHT_SWITCHES_ON);
                        hp_timer_reset(&led_off_timer);
                    } else {
                        ESP_LOGI(lt_tag, "Light manually switched off");
                        queue_send_message(display_queue, CMD_LIGHT_SWITCHES_OFF);
                    }
                    update_light = true;
                break;

                case CMD_ADJUST_LIGHT:
                    ESP_LOGI(lt_tag, "Display is adjusting the light, switch it on to show the effect");
                    common_settings.led_on = true;
                    hp_timer_reset(&led_off_timer);
                    update_light = true;
                break;

                case CMD_TIMER_CHANGED:
                    // Timer is changed, but light settings remain the same
                    hp_timer_reset(&led_off_timer);
                    update_light = true;
                break;

                default:
                    ESP_LOGI(lt_tag, "Command not recognized");
            } // switch(cmd) {
        } // xQueueReceive(light_queue, &cmd, 0) == true

        if(hp_timer_lapsed(&led_off_timer)) {
            ESP_LOGI(lt_tag, "Light switched off by timer");
            common_settings.led_on = false;

            // Notify the screen so the clock can switch off too
            queue_send_message(display_queue, CMD_LIGHT_SWITCHES_OFF);
            update_light = true;
        } // if(hp_timer_lapsed(&led_off_timer)) {

        // Settings have been changed, define a new target for the color channels.
        if(update_light) {

            if (xSemaphoreTake(mutex_change_settings, 0) == pdTRUE) {
                copy_of_settings = common_settings;
                xSemaphoreGive(mutex_change_settings);
            }

            if(common_settings.led_on) {
                hp_stepping_float_target(&red,   LED_INTENSITY[copy_of_settings.led_intensity]*
                                                LED_MAX_BRIGHTNESS*
                                                LED_COLORS[copy_of_settings.led_color].r, 500);

                hp_stepping_float_target(&green, LED_INTENSITY[copy_of_settings.led_intensity]*
                                                LED_MAX_BRIGHTNESS*
                                                LED_COLORS[copy_of_settings.led_color].g, 500);

                hp_stepping_float_target(&blue,  LED_INTENSITY[copy_of_settings.led_intensity]*
                                                LED_MAX_BRIGHTNESS*
                                                LED_COLORS[copy_of_settings.led_color].b, 500);
            } else {
                hp_stepping_float_target(&red,   0, 2000);
                hp_stepping_float_target(&green, 0, 2000);
                hp_stepping_float_target(&blue,  0, 2000);
            }
            // ESP_LOGI(lt_tag, "R: %.3f G: %.3f B: %.3f", red.target, green.target, blue.target);
            // for(int i=0; i<20; i++)
            //     ESP_LOGI(lt_tag, "R: %d G: %d B: %d", led_value(red.target), led_value(green.target), led_value(blue.target));

            // Reset the timer to switch off the LEDs
            hp_set_interval_ms(&led_off_timer, LED_TIMER_MS[copy_of_settings.led_timer]);

            update_light=false;
        } // if(update_light)

        if( hp_stepping_float_finished(&red)) {
            // ESP_LOGI(lt_tag, "R: %.3f G: %.3f B: %.3f", red.value, green.value, blue.value);
            vTaskDelay(pdMS_TO_TICKS(250));
        } else {
            // t1 = esp_timer_get_time();

            uint16_t mask_bit = 0x01;
            uint8_t led_strip_pixels[LED_NUM_LEDS * 3];

            for (int i = 0; i < 3*LED_NUM_LEDS; i+=3) {
                // Order of color channels in WS2812B is GRB
                led_strip_pixels[i + 1] = led_value(red.value  , mask_bit);
                led_strip_pixels[i + 0] = led_value(green.value, mask_bit);
                led_strip_pixels[i + 2] = led_value(blue.value , mask_bit);

                hp_stepping_float_step(&red);
                hp_stepping_float_step(&green);
                hp_stepping_float_step(&blue);

                mask_bit = mask_bit << 1;
            } // i

            // Flush RGB values to LEDs
            ESP_ERROR_CHECK(rmt_transmit(led_chan, led_encoder, led_strip_pixels, sizeof(led_strip_pixels), &tx_config));
            ESP_ERROR_CHECK(rmt_tx_wait_all_done(led_chan, portMAX_DELAY));

            // t2 = esp_timer_get_time();
            // snprintf(debug, sizeof(debug), "Update light took %d ms", (int) (t2-t1)/1000);

            vTaskDelay(pdMS_TO_TICKS(10)); // Doing 1500 steps @10ms/step = 1.5 s

        }  // if( hp_stepping_float_finished(&red)) {
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