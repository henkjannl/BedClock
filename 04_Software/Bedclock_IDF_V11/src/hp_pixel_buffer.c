#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "esp_lcd_panel_ops.h"
#include "esp_log.h"

#include "hp_pixel_buffer.h"

#define tag_buf "hp_pixel_buffer"

uint16_t hp_bitmap_bytes_per_row(const uint16_t width) {
    // Indicates how many bytes are needed for each row of pixels
    return ( (int) ((width-1)/8)) + 1;
}

uint16_t hp_bitmap_buffersize(const hp_bitmap_t *bitmap) {
    return hp_bitmap_bytes_per_row(bitmap->width) * bitmap->height;
}

int16_t hp_bitmap_pos_to_byte(const hp_bitmap_t *bitmap, const int16_t x, const int16_t y) {
    // Indicates the index of the byte in which the pixel at (x,y) resides
    return y*hp_bitmap_bytes_per_row(bitmap->width)+ ((int) (x/8));
}

int16_t hp_bitmap_pos_to_bit(const int16_t x) {
    // Indicates the index of the bit within the byte of the indicated pixel
    return x % 8;
}

void hp_bitmap_init(hp_bitmap_t *bitmap, const uint16_t width, const uint16_t height) {
    bitmap->width = width;
    bitmap->height = height;

    // Release memory if it was used before
    // if(bitmap->buffer != NULL) free(bitmap->buffer);

    // Reserve buffer for new bitmap
    bitmap->buffer = malloc(hp_bitmap_buffersize(bitmap));
}

void hp_bitmap_clear(hp_bitmap_t *bitmap) {
    for(uint16_t b=0; b<hp_bitmap_buffersize(bitmap); b++) bitmap->buffer[b]=0;
}

void hp_bitmap_delete(hp_bitmap_t *bitmap) {
    free(bitmap->buffer);
}

void hp_bitmap_copy(const hp_bitmap_t *input, hp_bitmap_t *output) {
    hp_bitmap_init(output, input->width, input->height);
    memcpy(output->buffer, input->buffer, hp_bitmap_buffersize(output) );
}

void hp_bitmap_transpose(const hp_bitmap_t *input, hp_bitmap_t *output) {
    hp_bitmap_init(output, input->height, input->width);

    uint16_t bpr_out = hp_bitmap_bytes_per_row(output->width);

    for(uint16_t y = 0; y<output->height; y++) { // Loop over each row of the output
        for(uint16_t b=0; b<bpr_out; b++) {      // Loop over each byte in the row
            output->buffer[bpr_out*y+b]=0;
            for(uint8_t i=0; i<8; i++) {         // Loop over each pixel in the byte
                uint16_t x = 8*b+i;

                if(x<output->width) { // Pixel should be part of bitmap
                // Pixel in output (x,y) = maps to input(y,x)
                    if( (input->buffer[hp_bitmap_pos_to_byte(input,  y, x)] & (0x01 << (y % 8)) ) !=0 )
                        output->buffer[y*bpr_out+b] |= (0x01 << i);
                }
            }
        }
    }
}

void hp_bitmap_invert(hp_bitmap_t *bitmap) {
    for(uint16_t b=0; b<hp_bitmap_buffersize(bitmap); b++) bitmap->buffer[b] = 0xFF - bitmap->buffer[b];
}

void hp_bitmap_copy_from_array(hp_bitmap_t *bitmap, const uint8_t array[], const uint16_t width, const uint16_t height) {
    // We should trust that the size of array[] is sufficient
    hp_bitmap_init(bitmap, width, height);
    memcpy(bitmap->buffer, array, hp_bitmap_buffersize(bitmap) );
}

void hp_bitmap_print(hp_bitmap_t *bitmap) {
    char c[bitmap->width+1];
    c[bitmap->width] = 0x00; // Make null terminated
    uint16_t bytes_per_row = hp_bitmap_bytes_per_row(bitmap->width);

    for(uint16_t row = 0; row<bitmap->height; row++) {
        for(uint16_t b=0; b<bytes_per_row; b++) {
            for(uint8_t i=0; i<8; i++) {
                c[8*b+i] = ( ( (bitmap->buffer[bytes_per_row*row+b]>>i) & 0x01) ==0 ) ? ' ' : 'O';
            }
        }
        printf("%s\n", c);
    }
}


