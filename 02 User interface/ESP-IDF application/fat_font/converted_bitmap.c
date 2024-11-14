#include <stdio.h>
#include "hp_pixel_font.h"
#include "hp_pixel_buffer.h"


uint8_t m0_bytes[] = {
    0x3c,0x7e,0xff,0xff,0xff,0xff,0x7e,0x3c };

uint8_t m1_bytes[] = {
    0x3e,0x7f,0x7f,0x7c,0x7c,0x7e,0xff,0x7e };

uint8_t m2_bytes[] = {
    0x7e,0x00,0xfe,0x00,0xff,0x00,0xfe,0x00,0xfc,0x01,0xfe,0x01,0xff,0x01,0xfe,
    0x00 };

uint8_t m3_bytes[] = {
    0x7e,0x00,0xff,0x00,0xfe,0x00,0xfe,0x01,0xfc,0x01,0xfe,0x01,0xff,0x00,0x7e,
    0x00 };

uint8_t m4_bytes[] = {
    0x30,0x78,0x7c,0x7e,0xff,0xff,0x7e,0x20 };

uint8_t m5_bytes[] = {
    0xfe,0x00,0xff,0x01,0x7f,0x00,0xff,0x00,0xff,0x01,0xfe,0x01,0xff,0x00,0x7e,
    0x00 };

uint8_t m6_bytes[] = {
    0x3c,0x7e,0x7f,0xff,0xff,0xff,0x7e,0x3c };

uint8_t m7_bytes[] = {
    0x3e,0x7f,0x7f,0x7e,0x3c,0x3e,0x1e,0x0c };

uint8_t m8_bytes[] = {
    0x3c,0x7e,0xff,0x7e,0xff,0xff,0x7e,0x3c };

uint8_t m9_bytes[] = {
    0x3c,0x7e,0xff,0xff,0xfe,0xfc,0x7e,0x3c };

uint8_t m_colon_bytes[] = {
    0x06,0x0f,0x0f,0x06,0x06,0x0f,0x0f,0x06 };

uint8_t m_dot_bytes[] = {
    0x00,0x00,0x00,0x06,0x0f,0x0f,0x0f,0x06 };

uint8_t m_space_bytes[] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };

uint8_t m_V_bytes[] = {
    0xce,0x01,0xff,0x03,0xfe,0x01,0xfe,0x00,0xfc,0x00,0x7c,0x00,0x78,0x00,0x30,
    0x00 };

uint8_t v0_bytes[] = {
    0x00,0x3c,0x66,0x66,0x66,0x7e,0x3c,0x00 };

uint8_t v1_bytes[] = {
    0x00,0x3e,0x38,0x38,0x38,0x38,0x7e,0x00 };

uint8_t v2_bytes[] = {
    0x00,0x00,0x7c,0x00,0x7e,0x00,0x70,0x00,0x38,0x00,0xfc,0x00,0xfe,0x00,0x00,
    0x00 };

uint8_t v3_bytes[] = {
    0x00,0x00,0x7e,0x00,0x70,0x00,0xfc,0x00,0xf0,0x00,0xf0,0x00,0x7e,0x00,0x00,
    0x00 };

uint8_t v4_bytes[] = {
    0x00,0x30,0x38,0x34,0x76,0x7e,0x20,0x00 };

uint8_t v5_bytes[] = {
    0x00,0x00,0xfe,0x00,0x0e,0x00,0x7e,0x00,0xfe,0x00,0xf0,0x00,0x7e,0x00,0x00,
    0x00 };

uint8_t v6_bytes[] = {
    0x00,0x3c,0x06,0x7e,0x66,0x66,0x3c,0x00 };

uint8_t v7_bytes[] = {
    0x00,0x3e,0x3e,0x38,0x18,0x1c,0x0c,0x00 };

uint8_t v8_bytes[] = {
    0x00,0x3c,0x66,0x3c,0x66,0x66,0x3c,0x00 };

uint8_t v9_bytes[] = {
    0x00,0x3c,0x66,0x66,0x7c,0x70,0x3c,0x00 };

uint8_t v_colon_bytes[] = {
    0x00,0x06,0x06,0x00,0x00,0x06,0x06,0x00 };

uint8_t v_dot_bytes[] = {
    0x00,0x00,0x00,0x00,0x06,0x06,0x06,0x00 };

uint8_t v_space_bytes[] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };

uint8_t v_V_bytes[] = {
    0x00,0x00,0xce,0x01,0xcc,0x00,0x7c,0x00,0x78,0x00,0x38,0x00,0x30,0x00,0x00,
    0x00 };

const hp_bitmap_t glyphs[] = {
    { .width = 8, .height = 8, .bitmap = m0_bytes .mask = m0_bytes },  // [0] = m0
    { .width = 8, .height = 8, .bitmap = m1_bytes .mask = m1_bytes },  // [1] = m1
    { .width = 9, .height = 8, .bitmap = m2_bytes .mask = m2_bytes },  // [2] = m2
    { .width = 9, .height = 8, .bitmap = m3_bytes .mask = m3_bytes },  // [3] = m3
    { .width = 8, .height = 8, .bitmap = m4_bytes .mask = m4_bytes },  // [4] = m4
    { .width = 9, .height = 8, .bitmap = m5_bytes .mask = m5_bytes },  // [5] = m5
    { .width = 8, .height = 8, .bitmap = m6_bytes .mask = m6_bytes },  // [6] = m6
    { .width = 7, .height = 8, .bitmap = m7_bytes .mask = m7_bytes },  // [7] = m7
    { .width = 8, .height = 8, .bitmap = m8_bytes .mask = m8_bytes },  // [8] = m8
    { .width = 8, .height = 8, .bitmap = m9_bytes .mask = m9_bytes },  // [9] = m9
    { .width = 4, .height = 8, .bitmap = m_colon_bytes .mask = m_colon_bytes },  // [10] = m_:
    { .width = 4, .height = 8, .bitmap = m_dot_bytes .mask = m_dot_bytes },  // [11] = m_.
    { .width = 3, .height = 8, .bitmap = m_space_bytes .mask = m_space_bytes },  // [12] = m_space
    { .width = 10, .height = 8, .bitmap = m_V_bytes .mask = m_V_bytes },  // [13] = m_V
    { .width = 8, .height = 8, .bitmap = v0_bytes .mask = m0_bytes },  // [14] = v0
    { .width = 8, .height = 8, .bitmap = v1_bytes .mask = m1_bytes },  // [15] = v1
    { .width = 9, .height = 8, .bitmap = v2_bytes .mask = m2_bytes },  // [16] = v2
    { .width = 9, .height = 8, .bitmap = v3_bytes .mask = m3_bytes },  // [17] = v3
    { .width = 8, .height = 8, .bitmap = v4_bytes .mask = m4_bytes },  // [18] = v4
    { .width = 9, .height = 8, .bitmap = v5_bytes .mask = m5_bytes },  // [19] = v5
    { .width = 8, .height = 8, .bitmap = v6_bytes .mask = m6_bytes },  // [20] = v6
    { .width = 7, .height = 8, .bitmap = v7_bytes .mask = m7_bytes },  // [21] = v7
    { .width = 8, .height = 8, .bitmap = v8_bytes .mask = m8_bytes },  // [22] = v8
    { .width = 8, .height = 8, .bitmap = v9_bytes .mask = m9_bytes },  // [23] = v9
    { .width = 4, .height = 8, .bitmap = v_colon_bytes .mask = m_colon_bytes },  // [24] = v_:
    { .width = 4, .height = 8, .bitmap = v_dot_bytes .mask = m_dot_bytes },  // [25] = v_.
    { .width = 3, .height = 8, .bitmap = v_space_bytes .mask = m_space_bytes },  // [26] = v_space
    { .width = 10, .height = 8, .bitmap = v_V_bytes .mask = m_V_bytes },  // [27] = v_V
}; // const hp_bitmap_t glyphs[]

const hp_bitmap_t *hp_pixel_glyph(char c) {
    if(c=='m0') return &glyphs[0];
    if(c=='m1') return &glyphs[1];
    if(c=='m2') return &glyphs[2];
    if(c=='m3') return &glyphs[3];
    if(c=='m4') return &glyphs[4];
    if(c=='m5') return &glyphs[5];
    if(c=='m6') return &glyphs[6];
    if(c=='m7') return &glyphs[7];
    if(c=='m8') return &glyphs[8];
    if(c=='m9') return &glyphs[9];
    if(c=='m_:') return &glyphs[10];
    if(c=='m_.') return &glyphs[11];
    if(c=='m_ ') return &glyphs[12];
    if(c=='m_V') return &glyphs[13];
    if(c=='v0') return &glyphs[14];
    if(c=='v1') return &glyphs[15];
    if(c=='v2') return &glyphs[16];
    if(c=='v3') return &glyphs[17];
    if(c=='v4') return &glyphs[18];
    if(c=='v5') return &glyphs[19];
    if(c=='v6') return &glyphs[20];
    if(c=='v7') return &glyphs[21];
    if(c=='v8') return &glyphs[22];
    if(c=='v9') return &glyphs[23];
    if(c=='v_:') return &glyphs[24];
    if(c=='v_.') return &glyphs[25];
    if(c=='v_ ') return &glyphs[26];
    if(c=='v_V') return &glyphs[27];
  //replace last item by return of value
}