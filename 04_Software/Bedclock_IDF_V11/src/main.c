#include <stdio.h>
#include "driver/i2c.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_system.h"
#include "esp_lcd_io_i2c.h"
#include "esp_err.h"
#include "esp_log.h"

#include "hp_pixel_buffer.h"

#define tag "main"

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
    Bedclock_IDF_V12 : Scrolling example LVGL
*/

#define MN_LCD_H_RES          128
#define MN_LCD_V_RES          32
// #define MN_LCD_V_RES          64
#define MN_I2C_HOST_ID        I2C_NUM_0
#define MN_I2C_SDA_GPIO       21
#define MN_I2C_SCL_GPIO       22
#define MN_LCD_PIXEL_CLOCK_HZ (400 * 1000)
#define MN_I2C_DEV_ADDR       0x3C

const uint8_t smiley[] = { 0xc3,0x81,0x24,0x00,0x42,0x24,0x99,0xc3 };

const uint8_t letter_p[] = { 0x0f,0x11,0x11,0x0f,0x01,0x01,0x01,0x00 };
const uint8_t example_8x16[] = { 0xff,0xff,0x18,0x18,0x18,0x18,0x18,0x18,0x18,0x00,0x3c,0x7e,0x7e,0x7e,0x3c,
0x00 };

const uint8_t pbe_18x8[] = { 0xcf,0xf3,0x01,0x51,0x14,0x00,0x51,0x14,0x00,0xcf,0xf3,0x01,0x41,0x14,0x00,
0x41,0x14,0x00,0xc1,0xf3,0x01,0x00,0x00,0x00 };

void app_main() {

    const i2c_config_t conf = {
        .mode             = I2C_MODE_MASTER,
        .sda_io_num       = MN_I2C_SDA_GPIO,
        .scl_io_num       = MN_I2C_SCL_GPIO,
        .sda_pullup_en    = GPIO_PULLUP_ENABLE,
        .scl_pullup_en    = GPIO_PULLUP_ENABLE,
        .master.clk_speed = MN_LCD_PIXEL_CLOCK_HZ,
    };

    ESP_ERROR_CHECK(i2c_param_config(MN_I2C_HOST_ID, &conf));

    ESP_ERROR_CHECK(i2c_driver_install(MN_I2C_HOST_ID, I2C_MODE_MASTER, 0, 0, 0));

    esp_lcd_panel_io_handle_t io_handle = NULL;

    esp_lcd_panel_io_i2c_config_t io_config = {
        .dev_addr = MN_I2C_DEV_ADDR,
        .control_phase_bytes = 1,  // According to SSD1306 datasheet
        .dc_bit_offset       = 6,  // According to SSD1306 datasheet
        .lcd_cmd_bits        = 8,  // According to SSD1306 datasheet
        .lcd_param_bits      = 8,  // According to SSD1306 datasheet
    };

    esp_lcd_new_panel_io_i2c( (esp_lcd_i2c_bus_handle_t)MN_I2C_HOST_ID, &io_config, &io_handle);

    esp_lcd_panel_handle_t panel_handle = NULL;

    esp_lcd_panel_ssd1306_config_t esp_lcd_panel_ssd1306_config = {
        .height = 32,
    };

    esp_lcd_panel_dev_config_t panel_config = {
        .bits_per_pixel = 1,
        .reset_gpio_num = -1,
        .vendor_config = &esp_lcd_panel_ssd1306_config,
    };

    esp_lcd_new_panel_ssd1306(io_handle, &panel_config, &panel_handle);
    esp_lcd_panel_reset(panel_handle);
    esp_lcd_panel_init(panel_handle);
    esp_lcd_panel_disp_on_off(panel_handle, true);

    /*
       Initialisation of the panel finished
                                            */


    hp_bitmap_t pbe_example;
    hp_bitmap_copy_from_array(&pbe_example, pbe_18x8, 18, 8);

    hp_bitmap_t transpose;
    hp_bitmap_transpose(&pbe_example, &transpose);

    // Clear the pixel buffer
    hp_bitmap_clear_canvas();

    // Write stuff on the pixel buffer
    hp_bitmap_draw_bitmap(&pbe_example,  -4, -4);
    hp_bitmap_draw_bitmap(&pbe_example, 20, 26);
    hp_bitmap_draw_bitmap(&pbe_example, 40, 4);
    hp_bitmap_draw_bitmap(&pbe_example, 128-18, 32-8);
    hp_bitmap_draw_bitmap(&transpose, 60, 0);

    // UPload the pixel buffer to the SSD1306
    hp_write_canvas(panel_handle);

    while(true) {

    }

    esp_lcd_panel_del(panel_handle);
    esp_lcd_panel_io_del(io_handle);
    i2c_driver_delete(MN_I2C_HOST_ID);
}