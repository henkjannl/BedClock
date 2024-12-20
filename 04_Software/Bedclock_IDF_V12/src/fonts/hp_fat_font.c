#include <stdio.h>
#include "hp_fat_font.h"
#include "hp_pixel_buffer.h"

static uint8_t m0_bytes[] = {
    0x3c,0x7e,0xff,0xff,0xff,0xff,0x7e,0x3c };

static uint8_t m1_bytes[] = {
    0x3e,0x7f,0x7f,0x7c,0x7c,0x7e,0xff,0x7e };

static  uint8_t m2_bytes[] = {
    0x7e,0x00,0xfe,0x00,0xff,0x00,0xfe,0x00,0xfc,0x01,0xfe,0x01,0xff,0x01,0xfe,
    0x00 };

static  uint8_t m3_bytes[] = {
    0x7e,0x00,0xff,0x00,0xfe,0x00,0xfe,0x01,0xfc,0x01,0xfe,0x01,0xff,0x00,0x7e,
    0x00 };

static  uint8_t m4_bytes[] = {
    0x30,0x78,0x7c,0x7e,0xff,0xff,0x7e,0x20 };

static  uint8_t m5_bytes[] = {
    0xfe,0x00,0xff,0x01,0x7f,0x00,0xff,0x00,0xff,0x01,0xfe,0x01,0xff,0x00,0x7e,
    0x00 };

static  uint8_t m6_bytes[] = {
    0x3c,0x7e,0x7f,0xff,0xff,0xff,0x7e,0x3c };

static  uint8_t m7_bytes[] = {
    0x3e,0x7f,0x7f,0x7e,0x3c,0x3e,0x1e,0x0c };

static  uint8_t m8_bytes[] = {
    0x3c,0x7e,0xff,0x7e,0xff,0xff,0x7e,0x3c };

static  uint8_t m9_bytes[] = {
    0x3c,0x7e,0xff,0xff,0xfe,0xfc,0x7e,0x3c };

static  uint8_t m_colon_bytes[] = {
    0x06,0x0f,0x0f,0x06,0x06,0x0f,0x0f,0x06 };

static  uint8_t m_dot_bytes[] = {
    0x00,0x00,0x00,0x06,0x0f,0x0f,0x0f,0x06 };

static  uint8_t m_space_bytes[] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };

static  uint8_t m_V_bytes[] = {
    0xce,0x01,0xff,0x03,0xfe,0x01,0xfe,0x00,0xfc,0x00,0x7c,0x00,0x78,0x00,0x30,
    0x00 };

static  uint8_t v0_bytes[] = {
    0x00,0x3c,0x66,0x66,0x66,0x7e,0x3c,0x00 };

static  uint8_t v1_bytes[] = {
    0x00,0x3e,0x38,0x38,0x38,0x38,0x7e,0x00 };

static  uint8_t v2_bytes[] = {
    0x00,0x00,0x7c,0x00,0x7e,0x00,0x70,0x00,0x38,0x00,0xfc,0x00,0xfe,0x00,0x00,
    0x00 };

static  uint8_t v3_bytes[] = {
    0x00,0x00,0x7e,0x00,0x70,0x00,0xfc,0x00,0xf0,0x00,0xf0,0x00,0x7e,0x00,0x00,
    0x00 };

static  uint8_t v4_bytes[] = {
    0x00,0x30,0x38,0x34,0x76,0x7e,0x20,0x00 };

static  uint8_t v5_bytes[] = {
    0x00,0x00,0xfe,0x00,0x0e,0x00,0x7e,0x00,0xfe,0x00,0xf0,0x00,0x7e,0x00,0x00,
    0x00 };

static  uint8_t v6_bytes[] = {
    0x00,0x3c,0x06,0x7e,0x66,0x66,0x3c,0x00 };

static  uint8_t v7_bytes[] = {
    0x00,0x3e,0x3e,0x38,0x18,0x1c,0x0c,0x00 };

static  uint8_t v8_bytes[] = {
    0x00,0x3c,0x66,0x3c,0x66,0x66,0x3c,0x00 };

static  uint8_t v9_bytes[] = {
    0x00,0x3c,0x66,0x66,0x7c,0x70,0x3c,0x00 };

static  uint8_t v_colon_bytes[] = {
    0x00,0x06,0x06,0x00,0x00,0x06,0x06,0x00 };

static  uint8_t v_dot_bytes[] = {
    0x00,0x00,0x00,0x00,0x06,0x06,0x06,0x00 };

static  uint8_t v_space_bytes[] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };

static  uint8_t v_V_bytes[] = {
    0x00,0x00,0xce,0x01,0xcc,0x00,0x7c,0x00,0x78,0x00,0x38,0x00,0x30,0x00,0x00,
    0x00 };

const hp_bitmap_t glyphs[] = {
    { .width =  8, .height = 8, .bitmap = v0_bytes,      .mask = m0_bytes },       //  0 = '0'
    { .width =  8, .height = 8, .bitmap = v1_bytes,      .mask = m1_bytes },       //  1 = '1'
    { .width =  9, .height = 8, .bitmap = v2_bytes,      .mask = m2_bytes },       //  2 = '2'
    { .width =  9, .height = 8, .bitmap = v3_bytes,      .mask = m3_bytes },       //  3 = '3'
    { .width =  8, .height = 8, .bitmap = v4_bytes,      .mask = m4_bytes },       //  4 = '4'
    { .width =  9, .height = 8, .bitmap = v5_bytes,      .mask = m5_bytes },       //  5 = '5'
    { .width =  8, .height = 8, .bitmap = v6_bytes,      .mask = m6_bytes },       //  6 = '6'
    { .width =  7, .height = 8, .bitmap = v7_bytes,      .mask = m7_bytes },       //  7 = '7'
    { .width =  8, .height = 8, .bitmap = v8_bytes,      .mask = m8_bytes },       //  8 = '8'
    { .width =  8, .height = 8, .bitmap = v9_bytes,      .mask = m9_bytes },       //  9 = '9'
    { .width =  4, .height = 8, .bitmap = v_colon_bytes, .mask = m_colon_bytes },  // 10 = ':'
    { .width =  4, .height = 8, .bitmap = v_dot_bytes,   .mask = m_dot_bytes },    // 11 = '.'
    { .width =  3, .height = 8, .bitmap = v_space_bytes, .mask = m_space_bytes },  // 12 = ' '
    { .width = 10, .height = 8, .bitmap = v_V_bytes,     .mask = m_V_bytes },      // 13 = 'V'
};

// Function that returns a bitmap given a character
const hp_bitmap_t *hp_fat_font(const unsigned char c) {
    if( c=='0') return &glyphs[ 0];
    if( c=='1') return &glyphs[ 1];
    if( c=='2') return &glyphs[ 2];
    if( c=='3') return &glyphs[ 3];
    if( c=='4') return &glyphs[ 4];
    if( c=='5') return &glyphs[ 5];
    if( c=='6') return &glyphs[ 6];
    if( c=='7') return &glyphs[ 7];
    if( c=='8') return &glyphs[ 8];
    if( c=='9') return &glyphs[ 9];
    if( c==':') return &glyphs[10];
    if( c=='.') return &glyphs[11];
    if( c==' ') return &glyphs[12];
    if( c=='V') return &glyphs[13];
    return NULL;
}
