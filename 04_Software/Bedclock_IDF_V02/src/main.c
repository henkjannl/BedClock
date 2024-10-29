#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/touch_pad.h"
#include "esp_log.h"

#include "ssd1306.h"
// #include "font8x8_basic.h"

#define TOUCH_PAD_NO_CHANGE   (-1)
#define TOUCH_THRESH_NO_USE   (0)
#define TOUCHPAD_FILTER_TOUCH_PERIOD (10)

#define TOUCH_LEFT 0   //  Left button is GPIO  4 = Touch0
#define TOUCH_TOP 6    //   Top button is GPIO 14 = Touch6
#define TOUCH_RIGHT 5  // Right button is GPIO 12 = Touch5

#define tag "TOUCH"

void app_main(void)
{
	// Setup display
	SSD1306_t dev;
	// int center, top, bottom;
	char lineChar[20];

#if CONFIG_I2C_INTERFACE
	ESP_LOGI(tag, "INTERFACE is i2c");
	ESP_LOGI(tag, "CONFIG_SDA_GPIO=%d",CONFIG_SDA_GPIO);
	ESP_LOGI(tag, "CONFIG_SCL_GPIO=%d",CONFIG_SCL_GPIO);
	ESP_LOGI(tag, "CONFIG_RESET_GPIO=%d",CONFIG_RESET_GPIO);
	i2c_master_init(&dev, CONFIG_SDA_GPIO, CONFIG_SCL_GPIO, CONFIG_RESET_GPIO);
#endif // CONFIG_I2C_INTERFACE

#if CONFIG_SPI_INTERFACE
	ESP_LOGI(tag, "INTERFACE is SPI");
	ESP_LOGI(tag, "CONFIG_MOSI_GPIO=%d",CONFIG_MOSI_GPIO);
	ESP_LOGI(tag, "CONFIG_SCLK_GPIO=%d",CONFIG_SCLK_GPIO);
	ESP_LOGI(tag, "CONFIG_CS_GPIO=%d",CONFIG_CS_GPIO);
	ESP_LOGI(tag, "CONFIG_DC_GPIO=%d",CONFIG_DC_GPIO);
	ESP_LOGI(tag, "CONFIG_RESET_GPIO=%d",CONFIG_RESET_GPIO);
	spi_master_init(&dev, CONFIG_MOSI_GPIO, CONFIG_SCLK_GPIO, CONFIG_CS_GPIO, CONFIG_DC_GPIO, CONFIG_RESET_GPIO);
#endif // CONFIG_SPI_INTERFACE

#if CONFIG_FLIP
	dev._flip = true;
	ESP_LOGW(tag, "Flip upside down");
#endif

#if CONFIG_SSD1306_128x64
	ESP_LOGI(tag, "Panel is 128x64");
	ssd1306_init(&dev, 128, 64);
#endif // CONFIG_SSD1306_128x64

#if CONFIG_SSD1306_128x32
	ESP_LOGI(tag, "Panel is 128x32");
	ssd1306_init(&dev, 128, 32);
#endif // CONFIG_SSD1306_128x32

	ssd1306_clear_screen(&dev, false);
	ssd1306_contrast(&dev, 0xff);
	// ssd1306_display_text_x3(&dev, 0, "Hello", 5, false);
	// vTaskDelay(3000 / portTICK_PERIOD_MS);
	
    // Initialize touch pad peripheral.
    // The default fsm mode is software trigger mode.
    ESP_ERROR_CHECK(touch_pad_init());

    // Set reference voltage for charging/discharging
    // In this case, the high reference valtage will be 2.7V - 1V = 1.7V
    // The low reference voltage will be 0.5
    // The larger the range, the larger the pulse count value.
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);

	touch_pad_config(TOUCH_LEFT,  TOUCH_THRESH_NO_USE);
	touch_pad_config(TOUCH_TOP,   TOUCH_THRESH_NO_USE);
	touch_pad_config(TOUCH_RIGHT, TOUCH_THRESH_NO_USE);

    touch_pad_filter_start(TOUCHPAD_FILTER_TOUCH_PERIOD);

	while(1) {

		uint16_t touch_value;
		uint16_t touch_filter_value;
		// bool btn_left, btn_top, btn_right;

		// Read left button
		touch_pad_read_raw_data(TOUCH_LEFT, &touch_value);
		touch_pad_read_filtered(TOUCH_LEFT, &touch_filter_value);
		snprintf ( lineChar, sizeof(lineChar)-1, " Left %d  ", touch_filter_value );
		ssd1306_display_text(&dev, 0, lineChar, sizeof(lineChar), false);

		// Read top button
		touch_pad_read_raw_data(TOUCH_TOP, &touch_value);
		touch_pad_read_filtered(TOUCH_TOP, &touch_filter_value);
		snprintf ( lineChar, sizeof(lineChar)-1, "  Top %d  ", touch_filter_value );
		ssd1306_display_text(&dev, 1, lineChar, sizeof(lineChar), false);

		// Read right button
		touch_pad_read_raw_data(TOUCH_RIGHT, &touch_value);
		touch_pad_read_filtered(TOUCH_RIGHT, &touch_filter_value);
		snprintf ( lineChar, sizeof(lineChar)-1, "Right %d  ", touch_value );
		ssd1306_display_text(&dev, 2, lineChar, sizeof(lineChar), false);

		vTaskDelay(200 / portTICK_PERIOD_MS);
	}

}

