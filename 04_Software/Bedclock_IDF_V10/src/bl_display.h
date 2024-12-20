#pragma once

#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <esp_system.h>
#include <esp_log.h>
#include <esp_timer.h>

#include <stdio.h>
#include <string.h>

#include "ssd1306.h"

#include "bl_queues.h"
#include "bl_characters.h"
#include "hp_timer.h"

#define MAX_DISPLAY_ITEM 6
#define dp_tag "display"

SSD1306_t display;

// Datastructure prepared for graphical screen layout
// typedef struct {
//     int16_t text_top;
//     int16_t circle_top;
//     int16_t screen_offset;
// } screen_items_t;

// const screen_items_t screenitems[] = {
//   { .text_top =   0,   .circle_top =   0,   .screen_offset =    0  },  // 0: idle
//   { .text_top =  32,   .circle_top =   0,   .screen_offset =  -32  },  // 1: time
//   { .text_top =  64,   .circle_top =  68,   .screen_offset =  -56  },  // 2: intensity
//   { .text_top =  80,   .circle_top =  84,   .screen_offset =  -72  },  // 3: color
//   { .text_top =  96,   .circle_top = 100,   .screen_offset =  -88  },  // 4: timer
//   { .text_top = 112,   .circle_top = 116,   .screen_offset = -104  }   // 5: display
// };

const uint8_t DISPLAY_INTENSITY[] = {
    0x01,    // .display_intensity = 0
    0x0F,    // .display_intensity = 1
    0xFF,    // .display_intensity = 2
};

uint8_t display_get_string_width(const char* time) {
    uint8_t width = 0;
    size_t length = strlen(time);
    for(int i=0; i<length; i++) width+=character(time[i]).width;
    return width;
}

void display_time(const char* time, uint8_t x, uint8_t y) {
    size_t length = strlen(time);
    for(int i=0; i<length; i++) {
        character_t c = character(time[i]);
        ssd1306_bitmaps(&display, x, y, c.bytes, c.width, c.height, false);
        x+=c.width;
    }
}

#include "driver/i2c.h"
void display_reset(SSD1306_t * dev) {
	i2c_cmd_handle_t cmd = i2c_cmd_link_create();
	// i2c_master_start(cmd);
	// i2c_master_write_byte(cmd, (dev->_address << 1) | I2C_MASTER_WRITE, true);
	// i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_CMD_STREAM, true);
	i2c_master_write_byte(cmd, OLED_CMD_DISPLAY_OFF, true);				// AE
	// i2c_master_write_byte(cmd, OLED_CMD_SET_MUX_RATIO, true);			// A8
	// if (dev->_height == 64) i2c_master_write_byte(cmd, 0x3F, true);
	// if (dev->_height == 32) i2c_master_write_byte(cmd, 0x1F, true);
	// i2c_master_write_byte(cmd, OLED_CMD_SET_DISPLAY_OFFSET, true);		// D3
	// i2c_master_write_byte(cmd, 0x00, true);
	// //i2c_master_write_byte(cmd, OLED_CONTROL_BYTE_DATA_STREAM, true);	// 40
	// i2c_master_write_byte(cmd, OLED_CMD_SET_DISPLAY_START_LINE, true);	// 40
	// //i2c_master_write_byte(cmd, OLED_CMD_SET_SEGMENT_REMAP, true);		// A1
	// if (dev->_flip) {
	// 	i2c_master_write_byte(cmd, OLED_CMD_SET_SEGMENT_REMAP_0, true); // A0
	// } else {
	// 	i2c_master_write_byte(cmd, OLED_CMD_SET_SEGMENT_REMAP_1, true);	// A1
	// }
	// i2c_master_write_byte(cmd, OLED_CMD_SET_COM_SCAN_MODE, true);		// C8
	// i2c_master_write_byte(cmd, OLED_CMD_SET_DISPLAY_CLK_DIV, true);		// D5
	// i2c_master_write_byte(cmd, 0x80, true);
	// i2c_master_write_byte(cmd, OLED_CMD_SET_COM_PIN_MAP, true);			// DA
	// if (dev->_height == 64) i2c_master_write_byte(cmd, 0x12, true);
	// if (dev->_height == 32) i2c_master_write_byte(cmd, 0x02, true);
	// i2c_master_write_byte(cmd, OLED_CMD_SET_CONTRAST, true);			// 81
	// i2c_master_write_byte(cmd, 0xFF, true);
	// i2c_master_write_byte(cmd, OLED_CMD_DISPLAY_RAM, true);				// A4
	// i2c_master_write_byte(cmd, OLED_CMD_SET_VCOMH_DESELCT, true);		// DB
	// i2c_master_write_byte(cmd, 0x40, true);
	// i2c_master_write_byte(cmd, OLED_CMD_SET_MEMORY_ADDR_MODE, true);	// 20
	// //i2c_master_write_byte(cmd, OLED_CMD_SET_HORI_ADDR_MODE, true);	// 00
	// i2c_master_write_byte(cmd, OLED_CMD_SET_PAGE_ADDR_MODE, true);		// 02
	// // Set Lower Column Start Address for Page Addressing Mode
	// i2c_master_write_byte(cmd, 0x00, true);
	// // Set Higher Column Start Address for Page Addressing Mode
	// i2c_master_write_byte(cmd, 0x10, true);
	// i2c_master_write_byte(cmd, OLED_CMD_SET_CHARGE_PUMP, true);			// 8D
	// i2c_master_write_byte(cmd, 0x14, true);
	// i2c_master_write_byte(cmd, OLED_CMD_DEACTIVE_SCROLL, true);			// 2E
	// i2c_master_write_byte(cmd, OLED_CMD_DISPLAY_NORMAL, true);			// A6
	i2c_master_write_byte(cmd, OLED_CMD_DISPLAY_ON, true);				// AF
	i2c_master_stop(cmd);

	// esp_err_t res = i2c_master_cmd_begin(dev->_i2c_num, cmd, I2C_TICKS_TO_WAIT);
	// if (res == ESP_OK) {
	// 	ESP_LOGI(TAG, "OLED configured successfully");
	// } else {
	// 	ESP_LOGE(TAG, "OLED configuration failed. code: 0x%.2X", res);
	// }
	i2c_cmd_link_delete(cmd);
}

void display_refresh() {
    uint8_t num_chars;
    char buffer [50];

    // Copy settings to a temporary struct
    static common_settings_t copy_of_settings;
    if (xSemaphoreTake(mutex_change_settings, portMAX_DELAY) == pdTRUE) {
        copy_of_settings = common_settings;
        xSemaphoreGive(mutex_change_settings);
    }

    // Measure how long redrawing takes
    // int64_t t1 = esp_timer_get_time();
    display_reset(&display);
    ssd1306_contrast(&display, DISPLAY_INTENSITY[copy_of_settings.display_intensity]);
    ESP_LOGI(dp_tag, "Contrast %d",  DISPLAY_INTENSITY[copy_of_settings.display_intensity]);

    ssd1306_clear_screen(&display, false);

    // Item 0 means nothing is shown on the screen
    if(common_settings.selected_item==0) return;

    // Show time on line 1

    time_t now = 0;
    struct tm timeinfo = { 0 };
    time(&now);
    localtime_r(&now, &timeinfo);
    sprintf(buffer, "%02d:%02d", timeinfo.tm_hour, timeinfo.tm_min);
    ssd1306_display_text(&display, 1, buffer, 5, false);

    switch(copy_of_settings.selected_item) {
        case 2:
          // Show intensity setting
          num_chars = sprintf(buffer, "Intensity %s%s%s%s%s",
            copy_of_settings.led_intensity==0 ? "O" : ".",
            copy_of_settings.led_intensity==1 ? "O" : ".",
            copy_of_settings.led_intensity==2 ? "O" : ".",
            copy_of_settings.led_intensity==3 ? "O" : ".",
            copy_of_settings.led_intensity==4 ? "O" : "."
          );
          ssd1306_display_text(&display, 3, buffer, num_chars, false);
        break;

        case 3:
          // Show color
          num_chars = sprintf(buffer, "Color %s%s%s%s",
            copy_of_settings.led_color==0 ? "O" : ".",
            copy_of_settings.led_color==1 ? "O" : ".",
            copy_of_settings.led_color==2 ? "O" : ".",
            copy_of_settings.led_color==3 ? "O" : "."
          );
          ssd1306_display_text(&display, 3, buffer, num_chars, false);
        break;

        case 4:
          // Show timer
          num_chars = sprintf(buffer, "Timer %s%s%s%s%s",
            copy_of_settings.led_timer==0 ? "O" : ".",
            copy_of_settings.led_timer==1 ? "O" : ".",
            copy_of_settings.led_timer==2 ? "O" : ".",
            copy_of_settings.led_timer==3 ? "O" : ".",
            copy_of_settings.led_timer==4 ? "O" : "."
          );
          ssd1306_display_text(&display, 3, buffer, num_chars, false);
        break;

        case 5:
          // Show display intensity
          num_chars = sprintf(buffer, "Display %s%s%s",
            copy_of_settings.display_intensity==0 ? "O" : ".",
            copy_of_settings.display_intensity==1 ? "O" : ".",
            copy_of_settings.display_intensity==2 ? "O" : "."
          );
          ssd1306_display_text(&display, 3, buffer, num_chars, false);
        break;
    }

    // int64_t t2 = esp_timer_get_time();

    // ESP_LOGI(dp_tag, "t1=%" PRId64 " t2=%" PRId64 " dt=%" PRId64 ". ", t1, t2, t2-t1);

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
                            refresh_display = true;
                            break;

                        case 2: // Modify LED intensity
                            ESP_LOGI(dp_tag, "Increase LED intensity");
                            if (xSemaphoreTake(mutex_change_settings, portMAX_DELAY) == pdTRUE) {
                                common_settings.led_intensity++;
                                if(common_settings.led_intensity>COMMON_MAX_LED_INTENSITY)
                                    common_settings.led_intensity=0;
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
                                xSemaphoreGive(mutex_change_settings);
                            }

                            // Report to the light that settings were changed
                            queue_send_message(light_queue, CMD_SETTINGS_CHANGED);

                            refresh_display = true;
                        break;

                        case 5: // Modify display intensity
                            ESP_LOGI(dp_tag, "Increase display intensity");
                            if (xSemaphoreTake(mutex_change_settings, portMAX_DELAY) == pdTRUE) {
                                common_settings.display_intensity++;
                                if(common_settings.display_intensity>COMMON_MAX_DISPLAY_INTENSITY)
                                    common_settings.display_intensity=0;
                                xSemaphoreGive(mutex_change_settings);
                            }

                            // Report to the light that settings were changed
                            queue_send_message(light_queue, CMD_SETTINGS_CHANGED);

                            refresh_display = true;
                        break;

                        default:
                            ESP_LOGI(dp_tag, "Item %d cannot be changed ", common_settings.selected_item);
                    } // case CMD_BTN_RIGHT_PRESSED -> switch(common_settings.selected_item) {
                break;

                case CMD_LIGHT_SWITCHED_ON:
                    // If the user switches the light on, the clock will also be displayed
                    if (xSemaphoreTake(mutex_change_settings, portMAX_DELAY) == pdTRUE) {
                        if(common_settings.selected_item==0) common_settings.selected_item=1;
                        xSemaphoreGive(mutex_change_settings);
                    }
                    hp_timer_reset(&sleep_timer);
                    refresh_display = true;
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

        if(refresh_display) {
            display_refresh();
            refresh_display = false;
        } // if(refresh_display) {

        vTaskDelay(pdMS_TO_TICKS(50));
    } // while true
} // void task_display(void *z)

void display_init() {

#if CONFIG_I2C_INTERFACE
	ESP_LOGI(dp_tag, "INTERFACE is i2c");
	ESP_LOGI(dp_tag, "CONFIG_SDA_GPIO=%d",CONFIG_SDA_GPIO);
	ESP_LOGI(dp_tag, "CONFIG_SCL_GPIO=%d",CONFIG_SCL_GPIO);
	ESP_LOGI(dp_tag, "CONFIG_RESET_GPIO=%d",CONFIG_RESET_GPIO);
	i2c_master_init(&display, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
#endif // CONFIG_I2C_INTERFACE

#if CONFIG_SPI_INTERFACE
	ESP_LOGI(dp_tag, "INTERFACE is SPI");
	ESP_LOGI(dp_tag, "CONFIG_MOSI_GPIO=%d",CONFIG_MOSI_GPIO);
	ESP_LOGI(dp_tag, "CONFIG_SCLK_GPIO=%d",CONFIG_SCLK_GPIO);
	ESP_LOGI(dp_tag, "CONFIG_CS_GPIO=%d",CONFIG_CS_GPIO);
	ESP_LOGI(dp_tag, "CONFIG_DC_GPIO=%d",CONFIG_DC_GPIO);
	ESP_LOGI(dp_tag, "CONFIG_RESET_GPIO=%d",CONFIG_RESET_GPIO);
	spi_master_init(&display, CONFIG_MOSI_GPIO, CONFIG_SCLK_GPIO, CONFIG_CS_GPIO, CONFIG_DC_GPIO, CONFIG_RESET_GPIO);
#endif // CONFIG_SPI_INTERFACE

#if CONFIG_FLIP
	display._flip = true;
	ESP_LOGW(dp_tag, "Flip upside down");
#endif

#if CONFIG_SSD1306_128x32
	ESP_LOGI(dp_tag, "Panel is 128x32");
	ssd1306_init(&display, 128, 32);
#endif // CONFIG_SSD1306_128x32

#if CONFIG_SSD1306_128x64
	ESP_LOGI(dp_tag, "Panel is 128x64");
	ssd1306_init(&display, 128, 64);
#endif // CONFIG_SSD1306_128x64

	// ssd1306_clear_screen(&display, false);
	// ssd1306_contrast(&display, 0xff);
    display_refresh();
    xTaskCreatePinnedToCore(task_display, "task_display", 4096, NULL, 1, NULL, 0);
}