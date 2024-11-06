#include <stdio.h>
#include "driver/i2c.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ops.h"
#include "esp_system.h"
#include "esp_lcd_io_i2c.h"
#include "esp_err.h"
#include "esp_log.h"

#define tag "main"

#define MN_LCD_H_RES          128
#define MN_LCD_V_RES          32
// #define MN_LCD_V_RES          64
#define MN_I2C_HOST_ID        I2C_NUM_0
#define MN_I2C_SDA_GPIO       21
#define MN_I2C_SCL_GPIO       22
#define MN_LCD_PIXEL_CLOCK_HZ (400 * 1000)
#define MN_I2C_DEV_ADDR       0x3C

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

    uint8_t x;
    uint8_t y;
    uint8_t bitmap[8];

    // Clear the screen
    for(int i=0; i<8; i++) bitmap[i]=0;

    for(x=0; x<128; x+=8)
        for(y=0; y<64;y+=8)
            esp_lcd_panel_draw_bitmap(panel_handle,  x, y, x+8, y+8, bitmap);

    while(true) {
        for(uint8_t i=0; i<8; i++) {
            uint8_t byte = 0x01 << i;
            for(uint8_t j=0; j<8; j++) {
                for(uint8_t k=0; k<8; k++) {
                    bitmap[k] = (j==k) ? 0xFF : byte;
                } // for k

            ESP_LOGI(tag, "i=%d byte=%d j=%d", i, byte, j);
            for(uint8_t j=0; j<8; j++) {
                x=8*j; y=0;   esp_lcd_panel_draw_bitmap(panel_handle,  x, y, x+8, y+8, bitmap);
                x=8*j; y=8*j; esp_lcd_panel_draw_bitmap(panel_handle,  x, y, x+8, y+8, bitmap);
            }

            vTaskDelay(pdMS_TO_TICKS(((i==0) && (j==0)) ? 1000 : 150));
            } // for j
        } // for i
    }




    // esp_lcd_panel_del(panel_handle);
    // esp_lcd_panel_io_del(io_handle);
    // i2c_driver_delete(MN_I2C_HOST_ID);
}