#include <stdio.h>
#include "driver/i2c.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_system.h"
#include "esp_lcd_io_i2c.h"
#include "esp_err.h"
#include "esp_log.h"

#include "bl_common.h"
#include "hp_timer.h"
#include "hp_stepping_float.h"
#include "hp_pixel_buffer.h"
#include "hp_pixel_canvas.h"

#define MAX_DISPLAY_ITEM 6
#define dp_tag "display"

// Global variables that manage screen animation
hp_stepping_float_t screen_y;
hp_stepping_float_t indicator_y;

/* ===================================
     Bitmaps to build up the screen
   ===================================*/

uint8_t labels_bytes[] = {
    0x02,0x00,0x00,0x00,0x02,0x00,0x02,0x08,0x00,0x00,0x10,0x00,0xd2,0x9c,0xd3,
    0x78,0xba,0x08,0x32,0x49,0x34,0x05,0x92,0x08,0x12,0x49,0x14,0x39,0x92,0x04,
    0x12,0xc9,0x17,0x41,0x12,0x05,0x12,0x49,0x10,0x41,0x12,0x02,0x12,0x99,0x13,
    0x3d,0x32,0x02,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x01,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1c,0x40,0x00,
    0x00,0x00,0x00,0x22,0x40,0x00,0x00,0x00,0x00,0x02,0x40,0x00,0x00,0x00,0x00,
    0x02,0x4e,0x1c,0x07,0x00,0x00,0x02,0x51,0x22,0x01,0x00,0x00,0x02,0x51,0x22,
    0x01,0x00,0x00,0x02,0x51,0x22,0x01,0x00,0x00,0x22,0x51,0x22,0x01,0x00,0x00,
    0x1c,0x8e,0x1c,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0xbf,0x00,0x00,0x00,0x00,0x00,0x04,0x00,0x00,0x00,0x00,0x00,0x84,0xda,0x71,
    0x1a,0x00,0x00,0x84,0x26,0x89,0x06,0x00,0x00,0x84,0x22,0x89,0x02,0x00,0x00,
    0x84,0x22,0xf9,0x02,0x00,0x00,0x84,0x22,0x09,0x02,0x00,0x00,0x84,0x22,0x71,
    0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x1e,0x00,0x00,
    0x00,0x00,0x00,0x22,0x02,0x80,0x00,0x00,0x00,0x42,0x00,0x80,0x00,0x00,0x00,
    0x42,0xf2,0x9e,0x78,0x11,0x00,0x42,0x0a,0xa2,0x40,0x11,0x00,0x42,0x72,0xa2,
    0x78,0x09,0x00,0x42,0x82,0xa2,0x44,0x0a,0x00,0x22,0x82,0xa2,0x44,0x04,0x00,
    0x1e,0x7a,0x1e,0x79,0x04,0x00,0x00,0x00,0x02,0x00,0x04,0x00,0x00,0x00,0x02,
    0x00,0x02,0x00 };

uint8_t circle_open_bytes[] = {
    0x00,0x00,0x38,0x00,0x44,0x00,0x82,0x00,0x82,0x00,0x82,0x00,0x44,0x00,0x38,
    0x00,0x00,0x00,0x00,0x00 };

uint8_t circle_closed_bytes[] = {
    0x00,0x00,0x38,0x00,0x7c,0x00,0xfe,0x00,0xfe,0x00,0xfe,0x00,0x7c,0x00,0x38,
    0x00,0x00,0x00,0x00,0x00 };

uint8_t indicator_bytes[] = {
    0x00,0x00,0x1e,0x00,0x3e,0x00,0xfe,0x00,0xfe,0x01,0xfe,0x01,0xfe,0x00,0x3e,
    0x00,0x0e,0x00,0x00,0x00 };

const hp_bitmap_t labels          = { .width = 48, .height = 48, .buffer = labels_bytes };
const hp_bitmap_t circle_open     = { .width = 10, .height = 10, .buffer = circle_open_bytes };
const hp_bitmap_t circle_closed   = { .width = 10, .height = 10, .buffer = circle_closed_bytes };
const hp_bitmap_t indicator       = { .width = 10, .height = 10, .buffer = indicator_bytes };

const uint8_t DISPLAY_INTENSITY[] = {
    0x01,    // .display_intensity = 0
    0x0F,    // .display_intensity = 1
    0xFF,    // .display_intensity = 2
};

// Position of screen
const int16_t Y_POS[] = {
    28, // 0 = Display sleeping
     0, // 1 = Showing time
   -26, // 2 = LED Intensity
   -38, // 3 = LED Color
   -50, // 4 = LED timer
   -62  // 5 = Display timer
};

// Indicator
const int16_t Y_IND[] = {
    -1, // 0 = Display sleeping, indicator should remain at current position
    -1, // 1 = Showing time,     indicator should remain at current position
    36, // 2 = LED Intensity
    48, // 3 = LED Color
    60, // 4 = LED timer
    72  // 5 = Display timer
};


/* ===================================
     Configuration data of the display
   ===================================*/
#define MN_LCD_H_RES          128
#define MN_LCD_V_RES          32
#define MN_I2C_HOST_ID        I2C_NUM_0
#define MN_I2C_SDA_GPIO       21
#define MN_I2C_SCL_GPIO       22
#define MN_LCD_PIXEL_CLOCK_HZ (370 * 1000)
#define MN_I2C_DEV_ADDR       0x3C

const i2c_config_t conf = {
    .mode             = I2C_MODE_MASTER,
    .sda_io_num       = MN_I2C_SDA_GPIO,
    .scl_io_num       = MN_I2C_SCL_GPIO,
    .sda_pullup_en    = GPIO_PULLUP_ENABLE,
    .scl_pullup_en    = GPIO_PULLUP_ENABLE,
    .master.clk_speed = MN_LCD_PIXEL_CLOCK_HZ,
};

esp_lcd_panel_io_handle_t io_handle;

esp_lcd_panel_io_i2c_config_t io_config = {
    .dev_addr = MN_I2C_DEV_ADDR,
    .control_phase_bytes = 1,  // According to SSD1306 datasheet
    .dc_bit_offset       = 6,  // According to SSD1306 datasheet
    .lcd_cmd_bits        = 8,  // According to SSD1306 datasheet
    .lcd_param_bits      = 8,  // According to SSD1306 datasheet
};

esp_lcd_panel_handle_t panel_handle = NULL;

esp_lcd_panel_ssd1306_config_t esp_lcd_panel_ssd1306_config = {
    .height = 32,
};

esp_lcd_panel_dev_config_t panel_config = {
    .bits_per_pixel = 1,
    .reset_gpio_num = -1,
    .vendor_config = &esp_lcd_panel_ssd1306_config,
};

void display_contrast(uint8_t contrast) {
    esp_lcd_panel_io_tx_param(io_handle, 0x81, &contrast, 1);
}

void display_reset() {
    // // Proposed by ChatGPT, highly experimental
    esp_lcd_panel_io_tx_param(io_handle, 0xAE, NULL, 0); // Switch display off
    vTaskDelay(pdMS_TO_TICKS(100)); // Wait 100 ms
    esp_lcd_panel_io_tx_param(io_handle, 0xAF, NULL, 0); // Switch display on
    // esp_lcd_panel_reset(panel_handle); // not helpful: assumes reset gpio connected
}

void display_refresh(int16_t y, uint8_t indicator_y) {
    uint8_t num_chars;
    char buffer [50];
    int16_t x;

    // Copy settings to a temporary struct
    static common_settings_t settings;
    if (xSemaphoreTake(mutex_change_settings, portMAX_DELAY) == pdTRUE) {
        settings = common_settings;
        xSemaphoreGive(mutex_change_settings);
    }

    // Retrieve time
    time_t now = 0;
    struct tm timeinfo = { 0 };
    time(&now);
    localtime_r(&now, &timeinfo);
    num_chars = sprintf(buffer, "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);

    display_contrast(DISPLAY_INTENSITY[settings.display_intensity]);

    hp_bitmap_clear_canvas();
    x = 64-(hp_bitmap_text_width(buffer, num_chars)>>1);
    hp_bitmap_draw_text(x, y+7, buffer, num_chars);
    hp_bitmap_draw_bitmap(&labels, 10, y+36);
    hp_bitmap_draw_bitmap(&indicator, 0, y+indicator_y);

    // Draw circles
    for(uint8_t i=0; i<COMMON_MAX_LED_INTENSITY+1; i++)
        hp_bitmap_draw_bitmap( (i==settings.led_intensity) ? &circle_closed : &circle_open, 58+10*i, y+36);

    for(uint8_t i=0; i<COMMON_MAX_LED_COLOR+1; i++)
        hp_bitmap_draw_bitmap( (i==settings.led_color) ? &circle_closed : &circle_open, 58+10*i, y+48);

    for(uint8_t i=0; i<COMMON_MAX_LED_TIMER+1; i++)
        hp_bitmap_draw_bitmap( (i==settings.led_timer) ? &circle_closed : &circle_open, 58+10*i, y+60);

    for(uint8_t i=0; i<COMMON_MAX_DISPLAY_INTENSITY+1; i++)
        hp_bitmap_draw_bitmap( (i==settings.display_intensity) ? &circle_closed : &circle_open, 58+10*i, y+72);

    // Upload the pixel buffer to the SSD1306
    hp_bitmap_write_canvas(panel_handle);
}

void task_display(void *z)
{
    common_command_t cmd = CMD_NONE;
    hp_timer_t sleep_timer;
    hp_timer_init_ms(&sleep_timer, 20000, false);
    bool refresh_display = true;

    while(true) {

        if (xQueueReceive(display_queue, &cmd, pdMS_TO_TICKS(100)) == true) {
            switch(cmd) {
                case CMD_BTN_LEFT_PRESSED:
                    ESP_LOGI(dp_tag, "item received BTN_LEFT");
                    hp_timer_reset(&sleep_timer);
                    if (xSemaphoreTake(mutex_change_settings, portMAX_DELAY) == pdTRUE) {
                        common_settings.selected_item++;
                        if(common_settings.selected_item>COMMON_MAX_SELECTED_ITEM)
                            common_settings.selected_item=1;

                        // Move the display to a new target

                        hp_stepping_float_target(&screen_y, Y_POS[common_settings.selected_item],
                            common_settings.selected_item==0 ? 20 : 8);
                        if (common_settings.selected_item>1) hp_stepping_float_target(&indicator_y,
                            Y_IND[common_settings.selected_item],  8);

                        xSemaphoreGive(mutex_change_settings);
                    }
                    refresh_display = true;
                break;

                case CMD_BTN_RIGHT_PRESSED:
                    ESP_LOGI(dp_tag, "item received BTN_RIGHT");
                    hp_timer_reset(&sleep_timer);

                    switch(common_settings.selected_item) {

                        case 0: // Wake up clock
                            ESP_LOGI(dp_tag, "Wake up clock on right button");
                            if (xSemaphoreTake(mutex_change_settings, portMAX_DELAY) == pdTRUE) {
                                common_settings.selected_item=1;
                                xSemaphoreGive(mutex_change_settings);
                            }
                            hp_stepping_float_target(&screen_y, Y_POS[1], 8);
                            refresh_display = true;
                            break;

                        case 2: // Modify LED intensity
                            ESP_LOGI(dp_tag, "Increase LED intensity");
                            if (xSemaphoreTake(mutex_change_settings, portMAX_DELAY) == pdTRUE) {
                                common_settings.led_intensity++;
                                if(common_settings.led_intensity>COMMON_MAX_LED_INTENSITY)
                                    common_settings.led_intensity=0;
                                common_settings.settings_changed = true;
                                xSemaphoreGive(mutex_change_settings);
                            }

                            // Report to the light that settings were changed
                            // queue_send_message(light_queue, CMD_SETTINGS_CHANGED);
                            queue_send_message(light_queue, CMD_ADJUST_LIGHT);

                            refresh_display = true;
                        break;

                        case 3: // Modify LED color
                            ESP_LOGI(dp_tag, "Increase LED color");
                            if (xSemaphoreTake(mutex_change_settings, portMAX_DELAY) == pdTRUE) {
                                common_settings.led_color++;
                                if(common_settings.led_color>COMMON_MAX_LED_COLOR)
                                    common_settings.led_color=0;
                                common_settings.settings_changed = true;
                                xSemaphoreGive(mutex_change_settings);
                            }
                            // Report to the light that settings were changed
                            // queue_send_message(light_queue, CMD_SETTINGS_CHANGED);
                            queue_send_message(light_queue, CMD_ADJUST_LIGHT);

                            refresh_display = true;
                        break;

                        case 4: // Modify LED timer
                            ESP_LOGI(dp_tag, "Increase LED timer");
                            if (xSemaphoreTake(mutex_change_settings, portMAX_DELAY) == pdTRUE) {
                                common_settings.led_timer++;
                                if(common_settings.led_timer>COMMON_MAX_LED_TIMER)
                                    common_settings.led_timer=0;
                                common_settings.settings_changed = true;
                                xSemaphoreGive(mutex_change_settings);
                            }

                            // Report to the light that settings were changed
                            queue_send_message(light_queue, CMD_SETTINGS_CHANGED);

                            refresh_display = true;
                        break;

                        case 5: // Modify display intensity
                            ESP_LOGI(dp_tag, "Modify display intensity");
                            if (xSemaphoreTake(mutex_change_settings, portMAX_DELAY) == pdTRUE) {
                                common_settings.display_intensity++;
                                if(common_settings.display_intensity>COMMON_MAX_DISPLAY_INTENSITY)
                                    common_settings.display_intensity=0;
                                common_settings.settings_changed = true;
                                xSemaphoreGive(mutex_change_settings);
                            }

                            refresh_display = true;
                        break;


                        default:
                            ESP_LOGI(dp_tag, "Item %d cannot be changed ", common_settings.selected_item);
                    } // case CMD_BTN_RIGHT_PRESSED -> switch(common_settings.selected_item) {
                break;

                case CMD_LIGHT_SWITCHED_ON:
                    // If the user switches the light on, the clock will also be displayed
                    if (xSemaphoreTake(mutex_change_settings, portMAX_DELAY) == pdTRUE) {
                        if(common_settings.selected_item==0) {
                            common_settings.selected_item=1;
                            hp_stepping_float_target(&screen_y,  Y_POS[1], 8);
                            hp_timer_reset(&sleep_timer);
                            refresh_display = true;
                        }
                        xSemaphoreGive(mutex_change_settings);
                    }
                break;

                case CMD_RESET_DISPLAY:
                    ESP_LOGI(dp_tag, "Reset display");
                    display_reset();
                break;

                default:
                    ESP_LOGI(dp_tag, "Received unknown button");
            } // switch(cmd) {
        } // if (xQueueReceive(display_queue

        if(hp_timer_lapsed(&sleep_timer)) {
            ESP_LOGI(dp_tag, "Display back to sleep");
            if (xSemaphoreTake(mutex_change_settings, portMAX_DELAY) == pdTRUE) {
                common_settings.selected_item=0;
                xSemaphoreGive(mutex_change_settings);
            }
            hp_stepping_float_target(&screen_y, Y_POS[0], 15);

            // Check if settings were changed
            common_save();

            refresh_display = true;
        } // if(hp_timer_lapsed(&sleep_timer))

        //Refresh display if minute has changed
        if(common_settings.selected_item>0) {
            time_t now = 0;
            struct tm timeinfo = { 0 };
            time(&now);
            localtime_r(&now, &timeinfo);
            static int prev_min = -1;
            if( prev_min != timeinfo.tm_min ) {
                ESP_LOGI(dp_tag, "Update clock due to minute change");
                prev_min = timeinfo.tm_min;
                refresh_display=true;
            }
        }

        // Also update display if animation is still going on
        hp_stepping_float_step(&screen_y);
        if(!hp_stepping_float_finished(&screen_y)) refresh_display=true;

        hp_stepping_float_step(&indicator_y);
        if(!hp_stepping_float_finished(&indicator_y)) refresh_display=true;

        if(refresh_display) {
            display_refresh( (int16_t) screen_y.value, (int16_t) indicator_y.value );
            refresh_display = false;
        } // if(refresh_display) {

        vTaskDelay(pdMS_TO_TICKS(2));
    } // while true
} // void task_display(void *z)

void display_init() {

    // Initialize the OLED display
    ESP_ERROR_CHECK(i2c_param_config(MN_I2C_HOST_ID, &conf));
    ESP_ERROR_CHECK(i2c_driver_install(MN_I2C_HOST_ID, I2C_MODE_MASTER, 0, 0, 0));
    io_handle = NULL;
    esp_lcd_new_panel_io_i2c( (esp_lcd_i2c_bus_handle_t)MN_I2C_HOST_ID, &io_config, &io_handle);
    esp_lcd_new_panel_ssd1306(io_handle, &panel_config, &panel_handle);
    esp_lcd_panel_reset(panel_handle);
    esp_lcd_panel_init(panel_handle);
    esp_lcd_panel_disp_on_off(panel_handle, true);

    screen_y.start  =    26;
    screen_y.target =     0;
    screen_y.steps  =    25;
    screen_y.interpolation = ST_SPRING;
    screen_y.step   =     0;
    screen_y.value  =    26;

    indicator_y.start  = 36;
    indicator_y.target = 36;
    indicator_y.steps  = 16;
    indicator_y.interpolation = ST_CLICK;
    indicator_y.step   = 0;
    indicator_y.value  = 36;

    display_refresh( (int16_t) screen_y.value, (int16_t) indicator_y.value);

    xTaskCreatePinnedToCore(task_display, "task_display", 4096, NULL, 1, NULL, 0);
}