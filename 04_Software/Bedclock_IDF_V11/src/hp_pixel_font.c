#include <stdio.h>
#include "hp_pixel_font.h"
#include "hp_pixel_buffer.h"


uint8_t c_0_bytes[] = {
    0x7c,0x00,0xd6,0x00,0x83,0x01,0x83,0x01,0x01,0x01,0x83,0x01,0x81,0x01,0x03,
    0x01,0x81,0x01,0x83,0x01,0x01,0x01,0x83,0x01,0x83,0x01,0x83,0x00,0xee,0x00,
    0x3c,0x00 };

uint8_t c_1_bytes[] = {
    0x1c,0x1f,0x09,0x18,0x18,0x08,0x18,0x18,0x08,0x18,0x18,0x08,0x18,0x18,0x08,
    0x18 };

uint8_t c_2_bytes[] = {
    0x7c,0x00,0xd6,0x00,0x82,0x01,0x83,0x01,0x80,0x01,0x80,0x00,0xc0,0x00,0x40,
    0x00,0x60,0x00,0x30,0x00,0x18,0x00,0x08,0x00,0x0c,0x00,0x06,0x00,0xb7,0x01,
    0xff,0x01 };

uint8_t c_3_bytes[] = {
    0x78,0x00,0xee,0x00,0x82,0x00,0x83,0x01,0x80,0x01,0x80,0x00,0xc0,0x00,0x70,
    0x00,0xd0,0x00,0x80,0x01,0x80,0x01,0x00,0x01,0x83,0x01,0x83,0x01,0xee,0x00,
    0x78,0x00 };

uint8_t c_4_bytes[] = {
    0x60,0x00,0x20,0x00,0x30,0x00,0x10,0x00,0x10,0x00,0x18,0x00,0x08,0x01,0x8c,
    0x01,0x84,0x00,0x86,0x01,0x86,0x01,0x83,0x00,0xff,0x03,0xa4,0x01,0x80,0x01,
    0x80,0x00 };

uint8_t c_5_bytes[] = {
    0xff,0x00,0x4a,0x00,0x03,0x00,0x03,0x00,0x02,0x00,0x7b,0x00,0xef,0x00,0x82,
    0x00,0x82,0x01,0x80,0x01,0x80,0x00,0x80,0x01,0x83,0x01,0x82,0x00,0xee,0x00,
    0x3c,0x00 };

uint8_t c_6_bytes[] = {
    0x30,0x00,0x10,0x00,0x18,0x00,0x08,0x00,0x0c,0x00,0x04,0x00,0x2e,0x00,0x7e,
    0x00,0xc6,0x00,0x83,0x01,0x83,0x01,0x81,0x01,0x83,0x00,0x82,0x00,0xee,0x00,
    0x3c,0x00 };

uint8_t c_7_bytes[] = {
    0xff,0x00,0xcb,0x00,0x81,0x00,0xc3,0x00,0xc0,0x00,0x40,0x00,0x60,0x00,0x60,
    0x00,0x20,0x00,0x30,0x00,0x30,0x00,0x10,0x00,0x30,0x00,0x18,0x00,0x18,0x00,
    0x08,0x00 };

uint8_t c_8_bytes[] = {
    0x78,0x00,0xee,0x01,0x82,0x01,0x02,0x01,0x03,0x01,0x02,0x01,0x86,0x01,0xfc,
    0x00,0xee,0x00,0x82,0x01,0x03,0x03,0x03,0x03,0x03,0x01,0x82,0x01,0xee,0x01,
    0x78,0x00 };

uint8_t c_9_bytes[] = {
    0x3c,0x00,0xf6,0x00,0x83,0x00,0x83,0x01,0x81,0x01,0x83,0x00,0x83,0x01,0xc6,
    0x00,0xfc,0x00,0x50,0x00,0x60,0x00,0x20,0x00,0x30,0x00,0x30,0x00,0x18,0x00,
    0x08,0x00 };

uint8_t c_colon_bytes[] = {
    0x00,0x00,0x00,0x00,0x01,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03,
    0x01 };

uint8_t c_space_bytes[] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
    0x00 };

const hp_bitmap_t glyphs[] = {
    { .width = 10, .height = 16, .buffer = c_0_bytes },  // [0] = 0
    { .width = 7, .height = 16, .buffer = c_1_bytes },  // [1] = 1
    { .width = 10, .height = 16, .buffer = c_2_bytes },  // [2] = 2
    { .width = 10, .height = 16, .buffer = c_3_bytes },  // [3] = 3
    { .width = 11, .height = 16, .buffer = c_4_bytes },  // [4] = 4
    { .width = 10, .height = 16, .buffer = c_5_bytes },  // [5] = 5
    { .width = 10, .height = 16, .buffer = c_6_bytes },  // [6] = 6
    { .width = 10, .height = 16, .buffer = c_7_bytes },  // [7] = 7
    { .width = 11, .height = 16, .buffer = c_8_bytes },  // [8] = 8
    { .width = 10, .height = 16, .buffer = c_9_bytes },  // [9] = 9
    { .width = 3, .height = 16, .buffer = c_colon_bytes },  // [10] = :
    { .width = 4, .height = 16, .buffer = c_space_bytes },  // [11] = space
}; // const hp_bitmap_t glyphs[]

const hp_bitmap_t *hp_pixel_glyph(char c) {
    if(c=='0') return &glyphs[0];
    if(c=='1') return &glyphs[1];
    if(c=='2') return &glyphs[2];
    if(c=='3') return &glyphs[3];
    if(c=='4') return &glyphs[4];
    if(c=='5') return &glyphs[5];
    if(c=='6') return &glyphs[6];
    if(c=='7') return &glyphs[7];
    if(c=='8') return &glyphs[8];
    if(c=='9') return &glyphs[9];
    if(c==':') return &glyphs[10];
    return &glyphs[11]; // default value, return space
}
