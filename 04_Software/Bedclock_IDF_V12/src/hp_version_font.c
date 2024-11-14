#include <stdio.h>
#include "hp_version_font.h"
#include "hp_pixel_buffer.h"

uint8_t version_font_0_bytes[] = {
    0x0e,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x0e };

uint8_t version_font_1_bytes[] = {
    0x06,0x05,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04 };

uint8_t version_font_2_bytes[] = {
    0x0e,0x11,0x11,0x10,0x08,0x08,0x04,0x02,0x01,0x1f };

uint8_t version_font_3_bytes[] = {
    0x0e,0x11,0x10,0x10,0x0c,0x10,0x10,0x11,0x11,0x0e };

uint8_t version_font_4_bytes[] = {
    0x08,0x08,0x04,0x04,0x22,0x22,0x21,0x3f,0x20,0x20 };

uint8_t version_font_5_bytes[] = {
    0x1f,0x01,0x01,0x0d,0x13,0x10,0x10,0x10,0x11,0x0e };

uint8_t version_font_6_bytes[] = {
    0x0c,0x02,0x02,0x01,0x0f,0x11,0x11,0x11,0x11,0x0e };

uint8_t version_font_7_bytes[] = {
    0x1f,0x11,0x11,0x08,0x08,0x08,0x08,0x04,0x04,0x04 };

uint8_t version_font_8_bytes[] = {
    0x1e,0x21,0x21,0x21,0x1e,0x21,0x21,0x21,0x21,0x1e };

uint8_t version_font_9_bytes[] = {
    0x0e,0x11,0x11,0x11,0x11,0x1e,0x08,0x08,0x04,0x04 };

uint8_t version_font_colon_bytes[] = {
    0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x01 };

uint8_t version_font_dot_bytes[] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01 };

uint8_t version_font_space_bytes[] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };

uint8_t version_font_v_bytes[] = {
    0x41,0x41,0x22,0x22,0x22,0x14,0x14,0x14,0x08,0x08 };

const hp_bitmap_t version_font_glyphs[] = {
    { .width = 6, .height = 10, .buffer = version_font_0_bytes },      //  0 = '0'
    { .width = 4, .height = 10, .buffer = version_font_1_bytes },      //  1 = '1'
    { .width = 6, .height = 10, .buffer = version_font_2_bytes },      //  2 = '2'
    { .width = 6, .height = 10, .buffer = version_font_3_bytes },      //  3 = '3'
    { .width = 7, .height = 10, .buffer = version_font_4_bytes },      //  4 = '4'
    { .width = 6, .height = 10, .buffer = version_font_5_bytes },      //  5 = '5'
    { .width = 6, .height = 10, .buffer = version_font_6_bytes },      //  6 = '6'
    { .width = 6, .height = 10, .buffer = version_font_7_bytes },      //  7 = '7'
    { .width = 7, .height = 10, .buffer = version_font_8_bytes },      //  8 = '8'
    { .width = 6, .height = 10, .buffer = version_font_9_bytes },      //  9 = '9'
    { .width = 2, .height = 10, .buffer = version_font_colon_bytes },  // 10 = ':'
    { .width = 2, .height = 10, .buffer = version_font_dot_bytes },    // 11 = '.'
    { .width = 2, .height = 10, .buffer = version_font_space_bytes },  // 12 = ' '
    { .width = 8, .height = 10, .buffer = version_font_v_bytes },      // 13 = 'v'
};

 const char char_map[] = {
    ['0'] = 0, ['1'] = 1, ['2'] = 2, ['3'] = 3, ['4'] = 4,
    ['5'] = 5, ['6'] = 6, ['7'] = 7, ['8'] = 8, ['9'] = 9,
    [':'] = 10, ['.'] = 11, [' '] = 12, ['v'] = 13
};

// Function that returns a bitmap given a character
const hp_bitmap_t *hp_time_font(char c) {
    if (char_map[c]) {
        return &version_font_glyphs[char_map[c]];
    } else {
        return &version_font_glyphs[char_map[' ']];
    }
}
