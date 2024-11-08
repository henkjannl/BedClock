#include <stdio.h>
#include "hp_pixel_font.h"
#include "hp_pixel_buffer.h"


uint8_t indicator_left_bytes[] = {
    0x00,0x00,0x1e,0x00,0x3e,0x00,0xfe,0x00,0xfe,0x01,0xfe,0x01,0xfe,0x00,0x3e,
    0x00,0x0e,0x00,0x00,0x00 };

uint8_t indicator_right_bytes[] = {
    0x00,0x00,0xe0,0x01,0xf0,0x01,0xfc,0x01,0xfe,0x01,0xfe,0x01,0xfc,0x01,0xf0,
    0x01,0xc0,0x01,0x00,0x00 };

const hp_bitmap_t glyphs[] = {
    { .width = 10, .height = 10, .buffer = indicator_left_bytes },  // [0] = indicator_left
    { .width = 10, .height = 10, .buffer = indicator_right_bytes },  // [1] = indicator_right
}; // const hp_bitmap_t glyphs[]

const hp_bitmap_t *hp_pixel_glyph(char c) {
    if(c=='indicator_left') return &glyphs[0];
    if(c=='indicator_right') return &glyphs[1];
  //replace last item by return of value
}