#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "esp_lcd_panel_ops.h"
#include "esp_log.h"

#include "hp_pixel_buffer.h"

#define tag_can "hp_pixel_canvas"

#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))


// Global pixel buffer for the SSD1306 display
uint8_t canvas[4][128];

void hp_bitmap_clear_canvas() {
    memset(canvas, 0, sizeof(canvas));
}

// Draw a bitmap on the canvas
void hp_bitmap_draw_bitmap(const hp_bitmap_t *bitmap, int16_t x, int16_t y) {

    bool has_mask = (bitmap->mask != NULL);

    for(uint8_t page = 0; page<4; page++) {

        for(uint8_t col = 0; col<128; col++) {

            int16_t x_im = col-x;
            int16_t im_bit = hp_bitmap_pos_to_bit(x_im);

            if( (x_im>=0) && (x_im<bitmap->width) ) {
                for(uint8_t bit = 0; bit<8; bit++) {

                    int16_t y_im = 8*page+bit-y;

                    // Check if this pixel is in the image
                    if( (y_im>=0) && (y_im<bitmap->height) ) {

                        uint16_t im_byte = hp_bitmap_pos_to_byte(bitmap, x_im, y_im);

                        if( (bitmap->bitmap[im_byte] & (0x01 << im_bit) ) != 0) {
                            canvas[page][col] |= (0x01 << (bit) );           // Set pixel
                        } else {
                            // Clear the pixel if no mask is used or if the mask has the pixel set
                            if(!has_mask || ( (bitmap->mask[im_byte] & (0x01 << im_bit) ) != 0) )
                                canvas[page][col] &= (0xFF - (0x01 << (bit) ) ); // Clear pixel
                        }

                    } // pixel is in the image
                } // for bit
            } // if x_im
        } // for column
    }  // for page
}

int16_t hp_bitmap_text_width(hp_font_t font, const unsigned char *text, uint8_t num_chars) {
    uint16_t result = 0;
    for(uint8_t c=0; c<num_chars; c++ ) {
        const hp_bitmap_t *bitmap = font(text[c]);
        if(bitmap != NULL) result+=bitmap->width;
    }

    return result;
}

void hp_bitmap_draw_text(hp_font_t font, int16_t x, int16_t y, const unsigned char *text, const uint8_t num_chars) {
    for(uint8_t c=0; c<num_chars; c++ ) {
        const hp_bitmap_t *bitmap = font(text[c]);
        if(bitmap!=NULL) {
            hp_bitmap_draw_bitmap(bitmap, x, y);
            x+=bitmap->width;
        }
    }
}

bool hp_bitmap_check_font(char *fontname, hp_font_t font) {
    bool result = true;

    ESP_LOGI(tag_can, "Checking font %s", fontname);
    for(unsigned char c = 0x00; c<0x80; c++) {
        const hp_bitmap_t *bitmap = font(c);

        if(bitmap != NULL) {

            ESP_LOGI(tag_can, " - Glyph %c: w = %d, h = %d",
                c, bitmap->width, bitmap->height);

            // if(hp_bitmap_buffersize(bitmap) != ARRAY_SIZE(bitmap->bitmap)) {
            //     ESP_LOGE(tag_can, " - Glyph %c: Bitmap size is %d, should be %d",
            //         c, ARRAY_SIZE(bitmap->bitmap), hp_bitmap_buffersize(bitmap) );
            //         result = false;
            // }

            // if(bitmap->mask != NULL)
            //     if(hp_bitmap_buffersize(bitmap) != ARRAY_SIZE(bitmap->mask)) {
            //         ESP_LOGE(tag_can, " - Glyph %c: Mask size is %d, should be %d",
            //             c, ARRAY_SIZE(bitmap->mask), hp_bitmap_buffersize(bitmap) );
            //         result = false;
            //     }
        }
    }
    return result;
}

void hp_bitmap_write_canvas(esp_lcd_panel_handle_t panel) {
    for(uint8_t page = 0; page<4; page++)
        esp_lcd_panel_draw_bitmap(panel,  0, 8*page, 128, 8*page+8, canvas[page]);
}
