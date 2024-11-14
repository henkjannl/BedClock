#include <stdio.h>
#include "hp_pixel_buffer.h

static uint8_t m0_bytes[] = {
    0x0c,0x1e,0x3f,0x3f,0x3f,0x3f,0x1e,0x0c };

static uint8_t m1_bytes[] = {
    0x04,0x0f,0x0f,0x0e,0x0e,0x0e,0x0e,0x04 };

static uint8_t m2_bytes[] = {
    0x1e,0x3f,0x3f,0x3c,0x1e,0x1f,0x3f,0x1e };

static uint8_t m3_bytes[] = {
    0x0c,0x1e,0x3f,0x3e,0x3c,0x3f,0x1e,0x0c };

static uint8_t m4_bytes[] = {
    0x08,0x1c,0x1e,0x1f,0x1f,0x3f,0x1e,0x08 };

static uint8_t m5_bytes[] = {
    0x1e,0x3f,0x1f,0x3f,0x3e,0x3e,0x1f,0x0e };

static uint8_t m6_bytes[] = {
    0x1c,0x3e,0x0f,0x1f,0x3f,0x3f,0x3e,0x1c };

static uint8_t m7_bytes[] = {
    0x1e,0x3f,0x3e,0x1c,0x1c,0x0e,0x0e,0x04 };

static uint8_t m8_bytes[] = {
    0x0c,0x1e,0x3f,0x1e,0x3f,0x3f,0x1e,0x0c };

static uint8_t m9_bytes[] = {
    0x0c,0x1e,0x3f,0x3f,0x3e,0x3c,0x1e,0x0c };

static uint8_t m_colon_bytes[] = {
    0x00,0x06,0x0f,0x0f,0x06,0x0f,0x0f,0x06 };

static uint8_t m_dot_bytes[] = {
    0x00,0x00,0x00,0x00,0x00,0x06,0x0f,0x06 };

static uint8_t m_i_bytes[] = {
    0x02,0x07,0x07,0x07,0x07,0x07,0x07,0x07 };

static uint8_t m_K_bytes[] = {
    0x6f,0xff,0xff,0x3f,0x7f,0xff,0xef,0xef };

static uint8_t m_m_bytes[] = {
    0x00,0x17,0x3f,0x7f,0x7f,0x7f,0x7f,0x7f };

static uint8_t m_n_bytes[] = {
    0x00,0x0f,0x3f,0x3f,0x3b,0x3b,0x3b,0x3f };

static uint8_t m_perc_bytes[] = {
    0xde,0xff,0x7f,0xff,0xfc,0xfe,0xff,0x7b };

static uint8_t m_space_bytes[] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };

static uint8_t m_V_bytes[] = {
    0x42,0xe7,0xe7,0x7e,0x7e,0x3c,0x3c,0x18 };

static uint8_t v0_bytes[] = {
    0x00,0x0c,0x12,0x12,0x12,0x12,0x0c,0x00 };

static uint8_t v1_bytes[] = {
    0x00,0x04,0x06,0x04,0x04,0x04,0x04,0x00 };

static uint8_t v2_bytes[] = {
    0x00,0x0c,0x12,0x10,0x0c,0x02,0x1e,0x00 };

static uint8_t v3_bytes[] = {
    0x00,0x0c,0x12,0x18,0x10,0x12,0x0c,0x00 };

static uint8_t v4_bytes[] = {
    0x00,0x08,0x0c,0x0a,0x0a,0x1e,0x08,0x00 };

static uint8_t v5_bytes[] = {
    0x00,0x1e,0x02,0x1e,0x10,0x10,0x0e,0x00 };

static uint8_t v6_bytes[] = {
    0x00,0x1c,0x02,0x0e,0x12,0x12,0x1c,0x00 };

static uint8_t v7_bytes[] = {
    0x00,0x1e,0x10,0x08,0x08,0x04,0x04,0x00 };

static uint8_t v8_bytes[] = {
    0x00,0x0c,0x12,0x0c,0x12,0x12,0x0c,0x00 };

static uint8_t v9_bytes[] = {
    0x00,0x0c,0x12,0x12,0x1c,0x10,0x0c,0x00 };

static uint8_t v_colon_bytes[] = {
    0x00,0x00,0x06,0x06,0x00,0x06,0x06,0x00 };

static uint8_t v_dot_bytes[] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x06,0x00 };

static uint8_t v_i_bytes[] = {
    0x00,0x02,0x00,0x02,0x02,0x02,0x02,0x00 };

static uint8_t v_K_bytes[] = {
    0x00,0x66,0x36,0x1e,0x3e,0x66,0x46,0x00 };

static uint8_t v_m_bytes[] = {
    0x00,0x00,0x16,0x2a,0x2a,0x2a,0x2a,0x00 };

static uint8_t v_n_bytes[] = {
    0x00,0x00,0x0e,0x12,0x12,0x12,0x12,0x00 };

static uint8_t v_perc_bytes[] = {
    0x00,0x4e,0x2a,0x1e,0x78,0x54,0x72,0x00 };

static uint8_t v_space_bytes[] = {
    0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00 };

static uint8_t v_V_bytes[] = {
    0x00,0x42,0x42,0x24,0x24,0x18,0x18,0x00 };

static const hp_bitmap_t glyphs[] = {
    { .width = 6, .height = 8, .bitmap = m0_bytes, .mask = m0_bytes },  // [0] = m0
    { .width = 4, .height = 8, .bitmap = m1_bytes, .mask = m1_bytes },  // [1] = m1
    { .width = 6, .height = 8, .bitmap = m2_bytes, .mask = m2_bytes },  // [2] = m2
    { .width = 6, .height = 8, .bitmap = m3_bytes, .mask = m3_bytes },  // [3] = m3
    { .width = 6, .height = 8, .bitmap = m4_bytes, .mask = m4_bytes },  // [4] = m4
    { .width = 6, .height = 8, .bitmap = m5_bytes, .mask = m5_bytes },  // [5] = m5
    { .width = 6, .height = 8, .bitmap = m6_bytes, .mask = m6_bytes },  // [6] = m6
    { .width = 6, .height = 8, .bitmap = m7_bytes, .mask = m7_bytes },  // [7] = m7
    { .width = 6, .height = 8, .bitmap = m8_bytes, .mask = m8_bytes },  // [8] = m8
    { .width = 6, .height = 8, .bitmap = m9_bytes, .mask = m9_bytes },  // [9] = m9
    { .width = 4, .height = 8, .bitmap = m_colon_bytes, .mask = m_colon_bytes },  // [10] = m_:
    { .width = 4, .height = 8, .bitmap = m_dot_bytes, .mask = m_dot_bytes },  // [11] = m_.
    { .width = 3, .height = 8, .bitmap = m_i_bytes, .mask = m_i_bytes },  // [12] = m_i
    { .width = 8, .height = 8, .bitmap = m_K_bytes, .mask = m_K_bytes },  // [13] = m_K
    { .width = 7, .height = 8, .bitmap = m_m_bytes, .mask = m_m_bytes },  // [14] = m_m
    { .width = 6, .height = 8, .bitmap = m_n_bytes, .mask = m_n_bytes },  // [15] = m_n
    { .width = 8, .height = 8, .bitmap = m_perc_bytes, .mask = m_perc_bytes },  // [16] = m_perc
    { .width = 4, .height = 8, .bitmap = m_space_bytes, .mask = m_space_bytes },  // [17] = m_space
    { .width = 8, .height = 8, .bitmap = m_V_bytes, .mask = m_V_bytes },  // [18] = m_V
    { .width = 6, .height = 8, .bitmap = v0_bytes, .mask = m0_bytes },  // [19] = v0
    { .width = 4, .height = 8, .bitmap = v1_bytes, .mask = m1_bytes },  // [20] = v1
    { .width = 6, .height = 8, .bitmap = v2_bytes, .mask = m2_bytes },  // [21] = v2
    { .width = 6, .height = 8, .bitmap = v3_bytes, .mask = m3_bytes },  // [22] = v3
    { .width = 6, .height = 8, .bitmap = v4_bytes, .mask = m4_bytes },  // [23] = v4
    { .width = 6, .height = 8, .bitmap = v5_bytes, .mask = m5_bytes },  // [24] = v5
    { .width = 6, .height = 8, .bitmap = v6_bytes, .mask = m6_bytes },  // [25] = v6
    { .width = 6, .height = 8, .bitmap = v7_bytes, .mask = m7_bytes },  // [26] = v7
    { .width = 6, .height = 8, .bitmap = v8_bytes, .mask = m8_bytes },  // [27] = v8
    { .width = 6, .height = 8, .bitmap = v9_bytes, .mask = m9_bytes },  // [28] = v9
    { .width = 4, .height = 8, .bitmap = v_colon_bytes, .mask = m_colon_bytes },  // [29] = v_:
    { .width = 4, .height = 8, .bitmap = v_dot_bytes, .mask = m_dot_bytes },  // [30] = v_.
    { .width = 3, .height = 8, .bitmap = v_i_bytes, .mask = m_i_bytes },  // [31] = v_i
    { .width = 8, .height = 8, .bitmap = v_K_bytes, .mask = m_K_bytes },  // [32] = v_K
    { .width = 7, .height = 8, .bitmap = v_m_bytes, .mask = m_m_bytes },  // [33] = v_m
    { .width = 6, .height = 8, .bitmap = v_n_bytes, .mask = m_n_bytes },  // [34] = v_n
    { .width = 8, .height = 8, .bitmap = v_perc_bytes, .mask = m_perc_bytes },  // [35] = v_perc
    { .width = 4, .height = 8, .bitmap = v_space_bytes, .mask = m_space_bytes },  // [36] = v_space
    { .width = 8, .height = 8, .bitmap = v_V_bytes, .mask = m_V_bytes },  // [37] = v_V
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
    if(c=='m_i') return &glyphs[12];
    if(c=='m_K') return &glyphs[13];
    if(c=='m_m') return &glyphs[14];
    if(c=='m_n') return &glyphs[15];
    if(c=='m_perc') return &glyphs[16];
    if(c=='m_ ') return &glyphs[17];
    if(c=='m_V') return &glyphs[18];
    if(c=='v0') return &glyphs[19];
    if(c=='v1') return &glyphs[20];
    if(c=='v2') return &glyphs[21];
    if(c=='v3') return &glyphs[22];
    if(c=='v4') return &glyphs[23];
    if(c=='v5') return &glyphs[24];
    if(c=='v6') return &glyphs[25];
    if(c=='v7') return &glyphs[26];
    if(c=='v8') return &glyphs[27];
    if(c=='v9') return &glyphs[28];
    if(c=='v_:') return &glyphs[29];
    if(c=='v_.') return &glyphs[30];
    if(c=='v_i') return &glyphs[31];
    if(c=='v_K') return &glyphs[32];
    if(c=='v_m') return &glyphs[33];
    if(c=='v_n') return &glyphs[34];
    if(c=='v_perc') return &glyphs[35];
    if(c=='v_ ') return &glyphs[36];
    if(c=='v_V') return &glyphs[37];
  //replace last item by return of value
}