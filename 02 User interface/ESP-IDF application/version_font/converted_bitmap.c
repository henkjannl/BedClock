#include <stdio.h>
#include "hp_pixel_font.h"
#include "hp_pixel_buffer.h"


uint8_t BedClock_bytes[] = {
    0x7e,0x00,0x40,0x3c,0x02,0x00,0x10,0x00,0x82,0x00,0x40,0x42,0x02,0x00,0x10,
    0x00,0x82,0x00,0x40,0x42,0x02,0x00,0x10,0x00,0x82,0x78,0x7c,0x02,0xc2,0xc3,
    0x13,0x09,0x7e,0x84,0x42,0x02,0x22,0x24,0x90,0x00,0x82,0x84,0x42,0x02,0x22,
    0x24,0x50,0x00,0x82,0xfc,0x42,0x02,0x22,0x24,0x70,0x00,0x82,0x04,0x42,0x42,
    0x22,0x24,0x90,0x00,0x82,0x04,0x42,0x42,0x26,0x24,0x10,0x01,0x7e,0x78,0x7c,
    0x3c,0xcc,0xc3,0x13,0x09 };

uint8_t c_0_bytes[] = {
    0x0e,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x0e };

uint8_t c_1_bytes[] = {
    0x06,0x05,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04 };

uint8_t c_2_bytes[] = {
    0x0e,0x11,0x11,0x10,0x08,0x08,0x04,0x02,0x01,0x1f };

uint8_t c_3_bytes[] = {
    0x0e,0x11,0x10,0x10,0x0c,0x10,0x10,0x11,0x11,0x0e };

uint8_t c_4_bytes[] = {
    0x08,0x08,0x04,0x04,0x22,0x22,0x21,0x3f,0x20,0x20 };

uint8_t c_5_bytes[] = {
    0x1f,0x01,0x01,0x0d,0x13,0x10,0x10,0x10,0x11,0x0e };

uint8_t c_6_bytes[] = {
    0x0c,0x02,0x02,0x01,0x0f,0x11,0x11,0x11,0x11,0x0e };

uint8_t c_7_bytes[] = {
    0x1f,0x11,0x11,0x08,0x08,0x08,0x08,0x04,0x04,0x04 };

uint8_t c_8_bytes[] = {
    0x1e,0x21,0x21,0x21,0x1e,0x21,0x21,0x21,0x21,0x1e };

uint8_t c_9_bytes[] = {
    0x0e,0x11,0x11,0x11,0x11,0x1e,0x08,0x08,0x04,0x04 };

uint8_t c_colon_bytes[] = {
    0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x01 };

uint8_t c_dot_bytes[] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01 };

uint8_t c_space_bytes[] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };

uint8_t c_v_bytes[] = {
    0x41,0x41,0x22,0x22,0x22,0x14,0x14,0x14,0x08,0x08 };

const hp_bitmap_t glyphs[] = {
    { .width = 60, .height = 10, .buffer = BedClock_bytes },  // [0] = BedClock
    { .width = 6, .height = 10, .buffer = c_0_bytes },  // [1] = 0
    { .width = 4, .height = 10, .buffer = c_1_bytes },  // [2] = 1
    { .width = 6, .height = 10, .buffer = c_2_bytes },  // [3] = 2
    { .width = 6, .height = 10, .buffer = c_3_bytes },  // [4] = 3
    { .width = 7, .height = 10, .buffer = c_4_bytes },  // [5] = 4
    { .width = 6, .height = 10, .buffer = c_5_bytes },  // [6] = 5
    { .width = 6, .height = 10, .buffer = c_6_bytes },  // [7] = 6
    { .width = 6, .height = 10, .buffer = c_7_bytes },  // [8] = 7
    { .width = 7, .height = 10, .buffer = c_8_bytes },  // [9] = 8
    { .width = 6, .height = 10, .buffer = c_9_bytes },  // [10] = 9
    { .width = 2, .height = 10, .buffer = c_colon_bytes },  // [11] = :
    { .width = 2, .height = 10, .buffer = c_dot_bytes },  // [12] = .
    { .width = 2, .height = 10, .buffer = c_space_bytes },  // [13] = space
    { .width = 8, .height = 10, .buffer = c_v_bytes },  // [14] = v
}; // const hp_bitmap_t glyphs[]

const hp_bitmap_t *hp_pixel_glyph(char c) {
    if(c=='BedClock') return &glyphs[0];
    if(c=='0') return &glyphs[1];
    if(c=='1') return &glyphs[2];
    if(c=='2') return &glyphs[3];
    if(c=='3') return &glyphs[4];
    if(c=='4') return &glyphs[5];
    if(c=='5') return &glyphs[6];
    if(c=='6') return &glyphs[7];
    if(c=='7') return &glyphs[8];
    if(c=='8') return &glyphs[9];
    if(c=='9') return &glyphs[10];
    if(c==':') return &glyphs[11];
    if(c=='.') return &glyphs[12];
    if(c==' ') return &glyphs[13];
    if(c=='v') return &glyphs[14];
  //replace last item by return of value
}