#pragma once

#include <driver/gpio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/semphr.h>
#include <esp_system.h>
#include <esp_log.h>
#include <esp_timer.h>

#include <stdio.h>

#include "ssd1306.h"

#include "bl_queues.h"

#define MAX_DISPLAY_ITEM 6
#define tag "display"

SSD1306_t display;

typedef struct {
    int16_t text_top;
    int16_t circle_top;
    int16_t screen_offset;
} screen_items_t;

const screen_items_t screenitems[] = {
  { .text_top =   0,   .circle_top =   0,   .screen_offset =    0  },  // idle
  { .text_top =  32,   .circle_top =   0,   .screen_offset =  -32  },  // time
  { .text_top =  64,   .circle_top =  68,   .screen_offset =  -56  },  // intensity
  { .text_top =  80,   .circle_top =  84,   .screen_offset =  -72  },  // color
  { .text_top =  96,   .circle_top = 100,   .screen_offset =  -88  },  // timer
  { .text_top = 112,   .circle_top = 116,   .screen_offset = -104  }   // display
};


void display_refresh() {

    uint8_t num_chars;
    char buffer [50];

    // Measure how long redrawing takes
    int64_t t1 = esp_timer_get_time();

    ssd1306_clear_screen(&display, false);

    // Item 0 means nothing is shown on the screen
    if(common_settings.selected_item==0) return;

    // Show time on line 1
    ssd1306_display_text(&display, 1, "12:34", 5, false);

    switch(common_settings.selected_item) {
        case 2:
          // Show intensity setting
          num_chars = sprintf(buffer, "Intensity %s%s%s%s", 
            common_settings.led_intensity==0 ? "X" : "O",
            common_settings.led_intensity==1 ? "X" : "O",
            common_settings.led_intensity==2 ? "X" : "O",
            common_settings.led_intensity==3 ? "X" : "O"
          );
          ssd1306_display_text(&display, 3, buffer, num_chars, false);
        break;

        case 3:
          // Show color
          num_chars = sprintf(buffer, "LED color %s%s%s%s", 
            common_settings.led_color==0 ? "X" : "O",
            common_settings.led_color==1 ? "X" : "O",
            common_settings.led_color==2 ? "X" : "O",
            common_settings.led_color==3 ? "X" : "O"
          );
          ssd1306_display_text(&display, 3, buffer, num_chars, false);
        break;

        case 4:
          // Show timer
          num_chars = sprintf(buffer, "Timer %s%s%s%s", 
            common_settings.led_timer==0 ? "X" : "O",
            common_settings.led_timer==1 ? "X" : "O",
            common_settings.led_timer==2 ? "X" : "O",
            common_settings.led_timer==3 ? "X" : "O"
          );
          ssd1306_display_text(&display, 3, buffer, num_chars, false);
        break;

        case 5:
          // Show display intensity
          num_chars = sprintf(buffer, "Display %s%s%s%s", 
            common_settings.display_intensity==0 ? "X" : "O",
            common_settings.display_intensity==1 ? "X" : "O",
            common_settings.display_intensity==2 ? "X" : "O",
            common_settings.display_intensity==3 ? "X" : "O"
          );
          ssd1306_display_text(&display, 3, buffer, num_chars, false);
        break;
    } 

    int64_t t2 = esp_timer_get_time();

    sprintf (buffer, "t1=%" PRId64 " t2=%" PRId64 " dt=%" PRId64 ". ", t1, t2, t2-t1);
    ESP_LOGI(tag, "%s", buffer);

}

void task_display(void *z)
{
    queue_command_t btn = 0;

    while(1)
    {
        if (xQueueReceive(keyboard_to_display_queue, &btn, pdMS_TO_TICKS(100)) == true) 
        {

            switch(btn) {
                case BTN_LEFT:
                    ESP_LOGI(tag, "item received BTN_LEFT"); 
                    if (xSemaphoreTake(mutex_change_settings, portMAX_DELAY) == pdTRUE) {
                        common_settings.selected_item++;
                        if(common_settings.selected_item>COMMON_MAX_SELECTED_ITEM) 
                            common_settings.selected_item=1;
                        xSemaphoreGive(mutex_change_settings);
                    }
                break;

                case BTN_TOP:
                    ESP_LOGI(tag, "item received BTN_TOP"); 
                    if (xSemaphoreTake(mutex_change_settings, portMAX_DELAY) == pdTRUE) {
                        common_settings.led_on = !common_settings.led_on;
                        xSemaphoreGive(mutex_change_settings);
                    }
                break;

                case BTN_RIGHT:
                    ESP_LOGI(tag, "item received BTN_RIGHT"); 
    
                    switch(common_settings.selected_item) {
    
                        case 2: // Modify LED intensity
                            ESP_LOGI(tag, "Increase LED intensity"); 
                            if (xSemaphoreTake(mutex_change_settings, portMAX_DELAY) == pdTRUE) {
                                common_settings.led_intensity++;
                                if(common_settings.led_intensity>COMMON_MAX_LED_INTENSITY)
                                    common_settings.led_intensity=0;
                                xSemaphoreGive(mutex_change_settings);
                            }
                        break;

                        case 3: // Modify LED color 
                            ESP_LOGI(tag, "Increase LED color"); 
                            if (xSemaphoreTake(mutex_change_settings, portMAX_DELAY) == pdTRUE) {
                                common_settings.led_color++;
                                if(common_settings.led_color>COMMON_MAX_LED_COLOR)
                                    common_settings.led_color=0;
                                xSemaphoreGive(mutex_change_settings);
                            }
                        break;

                        case 4: // Modify LED timer 
                            ESP_LOGI(tag, "Increase LED timer"); 
                            if (xSemaphoreTake(mutex_change_settings, portMAX_DELAY) == pdTRUE) {
                                common_settings.led_timer++;
                                if(common_settings.led_timer>COMMON_MAX_LED_TIMER)
                                    common_settings.led_timer=0;
                                xSemaphoreGive(mutex_change_settings);
                            }
                        break;

                        case 5: // Modify display intensity 
                            ESP_LOGI(tag, "Increase display intensity"); 
                            if (xSemaphoreTake(mutex_change_settings, portMAX_DELAY) == pdTRUE) {
                                common_settings.display_intensity++;
                                if(common_settings.display_intensity>COMMON_MAX_DISPLAY_INTENSITY)
                                    common_settings.display_intensity=0;
                                xSemaphoreGive(mutex_change_settings);
                            }
                        break;

                        default:
                            ESP_LOGI(tag, "Item %d cannot be changed ", common_settings.selected_item); 
                    }
                break;

                default:
                    ESP_LOGI(tag, "Received unknown button"); 
            }

            display_refresh();
        } 

        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}

void display_init() {

#if CONFIG_I2C_INTERFACE
	ESP_LOGI(tag, "INTERFACE is i2c");
	ESP_LOGI(tag, "CONFIG_SDA_GPIO=%d",CONFIG_SDA_GPIO);
	ESP_LOGI(tag, "CONFIG_SCL_GPIO=%d",CONFIG_SCL_GPIO);
	ESP_LOGI(tag, "CONFIG_RESET_GPIO=%d",CONFIG_RESET_GPIO);
	i2c_master_init(&display, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
#endif // CONFIG_I2C_INTERFACE

#if CONFIG_SPI_INTERFACE
	ESP_LOGI(tag, "INTERFACE is SPI");
	ESP_LOGI(tag, "CONFIG_MOSI_GPIO=%d",CONFIG_MOSI_GPIO);
	ESP_LOGI(tag, "CONFIG_SCLK_GPIO=%d",CONFIG_SCLK_GPIO);
	ESP_LOGI(tag, "CONFIG_CS_GPIO=%d",CONFIG_CS_GPIO);
	ESP_LOGI(tag, "CONFIG_DC_GPIO=%d",CONFIG_DC_GPIO);
	ESP_LOGI(tag, "CONFIG_RESET_GPIO=%d",CONFIG_RESET_GPIO);
	spi_master_init(&display, CONFIG_MOSI_GPIO, CONFIG_SCLK_GPIO, CONFIG_CS_GPIO, CONFIG_DC_GPIO, CONFIG_RESET_GPIO);
#endif // CONFIG_SPI_INTERFACE

#if CONFIG_FLIP
	display._flip = true;
	ESP_LOGW(tag, "Flip upside down");
#endif

#if CONFIG_SSD1306_128x64
	ESP_LOGI(tag, "Panel is 128x64");
	ssd1306_init(&display, 128, 64);
#endif // CONFIG_SSD1306_128x64

#if CONFIG_SSD1306_128x32
	ESP_LOGI(tag, "Panel is 128x32");
	ssd1306_init(&display, 128, 32);
#endif // CONFIG_SSD1306_128x32

	ssd1306_clear_screen(&display, false);

	ssd1306_contrast(&display, 0xff);

    xTaskCreatePinnedToCore(task_display, "task_display", 4096, NULL, 1, NULL, 0);
}