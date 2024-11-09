#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "esp_lcd_panel_ops.h"
#include "esp_log.h"

#include "hp_pixel_buffer.h"
#include "hp_pixel_font.h"

#define tag_can "hp_pixel_canvas"

// Global pixel buffer
uint8_t canvas[4][128];

void hp_bitmap_clear_canvas() {
    for(uint8_t page = 0; page<4; page++)
        for(uint8_t col = 0; col<128; col++)
            canvas[page][col]=0;
}

// Draw a bitmap on the canvas
void hp_bitmap_draw_bitmap(const hp_bitmap_t *bitmap, int16_t x, int16_t y) {

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

                        if( (bitmap->buffer[im_byte] & (0x01 << im_bit) ) != 0) {
                            canvas[page][col] |= (0x01 << (bit) );           // Set pixel
                        } else {
                            canvas[page][col] &= (0xFF - (0x01 << (bit) ) ); // Clear pixel
                        }

                    } // pixel is in the image
                } // for bit
            } // if x_im
        } // for column
    }  // for page
}

uint16_t hp_bitmap_text_width(char *text, uint8_t num_chars) {
    uint16_t result = 0;
    for(uint8_t c=0; c<num_chars; c++ ) result+=hp_pixel_glyph(text[c])->width;
    return result;
}

void hp_bitmap_draw_text(int16_t x, int16_t y, const char *text, const uint8_t num_chars) {
    for(uint8_t c=0; c<num_chars; c++ ) {
        hp_bitmap_draw_bitmap(hp_pixel_glyph(text[c]), x, y);
        x+=hp_pixel_glyph(text[c])->width;
    }
}

void hp_bitmap_write_canvas(esp_lcd_panel_handle_t panel) {
    for(uint8_t page = 0; page<4; page++)
        esp_lcd_panel_draw_bitmap(panel,  0, 8*page, 128, 8*page+8, canvas[page]);
}
