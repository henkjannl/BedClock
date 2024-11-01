/*******************************************************************************
 * Size: 16 px
 * Bpp: 1
 * Opts: --bpp 1 --size 16 --font C:/Users/henkj/Documents/GitHub/BedClock/02 User interface/SquareLine Studio/assets/bahnschrift.ttf -o C:/Users/henkj/Documents/GitHub/BedClock/02 User interface/SquareLine Studio/assets\ui_font_Bahnschrit.c --format lvgl -r 0x20-0x7f --no-compress --no-prefilter
 ******************************************************************************/

#include "../ui.h"

#ifndef UI_FONT_BAHNSCHRIT
#define UI_FONT_BAHNSCHRIT 1
#endif

#if UI_FONT_BAHNSCHRIT

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xff, 0xff, 0xc,

    /* U+0022 "\"" */
    0xff, 0xff,

    /* U+0023 "#" */
    0x12, 0x9, 0x4, 0x8f, 0xf1, 0x61, 0xb0, 0x91,
    0xfe, 0x24, 0x12, 0x9, 0x0,

    /* U+0024 "$" */
    0x30, 0xfb, 0xd7, 0x8f, 0x1e, 0x1f, 0x1f, 0x36,
    0x6f, 0xdf, 0xe3, 0x0,

    /* U+0025 "%" */
    0xf1, 0x3c, 0xcf, 0x23, 0xd0, 0x4, 0x2, 0x0,
    0xb8, 0x5b, 0x26, 0xc9, 0xb4, 0x38,

    /* U+0026 "&" */
    0x3c, 0x19, 0x86, 0x61, 0x80, 0x70, 0xc, 0xf,
    0x9b, 0x36, 0xc7, 0x31, 0xc7, 0xf8,

    /* U+0027 "'" */
    0xff,

    /* U+0028 "(" */
    0x13, 0x66, 0xcc, 0xcc, 0xcc, 0x66, 0x33,

    /* U+0029 ")" */
    0x8c, 0x66, 0x33, 0x33, 0x33, 0x66, 0xcc,

    /* U+002A "*" */
    0x21, 0x3e, 0xe5, 0x0,

    /* U+002B "+" */
    0x30, 0xc3, 0x3f, 0x30, 0xc0,

    /* U+002C "," */
    0xfe,

    /* U+002D "-" */
    0xf8,

    /* U+002E "." */
    0xc0,

    /* U+002F "/" */
    0x6, 0x8, 0x30, 0x60, 0x83, 0x6, 0x8, 0x30,
    0x40, 0x83, 0x4, 0x18, 0x30, 0x0,

    /* U+0030 "0" */
    0x7d, 0x8f, 0x1e, 0x3c, 0x78, 0xf1, 0xe3, 0xc7,
    0x8d, 0xf0,

    /* U+0031 "1" */
    0x7f, 0xb3, 0x33, 0x33, 0x33, 0x30,

    /* U+0032 "2" */
    0x7d, 0x8f, 0x18, 0x30, 0xe3, 0x86, 0x18, 0x71,
    0xc3, 0xf8,

    /* U+0033 "3" */
    0x3d, 0x8f, 0x18, 0x30, 0x47, 0x1, 0x83, 0x87,
    0x8d, 0xf0,

    /* U+0034 "4" */
    0x8, 0x18, 0x10, 0x30, 0x30, 0x66, 0x66, 0xc6,
    0xff, 0x6, 0x6,

    /* U+0035 "5" */
    0xff, 0xc, 0x3e, 0xcc, 0x30, 0xc3, 0xcf, 0x37,
    0x80,

    /* U+0036 "6" */
    0x18, 0x30, 0xc1, 0x86, 0x1f, 0xb1, 0xe3, 0xc7,
    0x8d, 0xf0,

    /* U+0037 "7" */
    0xff, 0x3c, 0xc2, 0x8, 0x61, 0x84, 0x10, 0xc3,
    0x0,

    /* U+0038 "8" */
    0x7d, 0x8f, 0x1e, 0x34, 0x47, 0x31, 0xe3, 0xc7,
    0x8d, 0xf0,

    /* U+0039 "9" */
    0x7d, 0x8f, 0x1e, 0x3c, 0x6f, 0xc3, 0xc, 0x18,
    0x60, 0xc0,

    /* U+003A ":" */
    0xc0, 0x3,

    /* U+003B ";" */
    0xc0, 0x3f, 0x80,

    /* U+003C "<" */
    0x9, 0xdc, 0xe3, 0x84,

    /* U+003D "=" */
    0xf8, 0x1, 0xf0,

    /* U+003E ">" */
    0x87, 0x1c, 0xee, 0x40,

    /* U+003F "?" */
    0x7b, 0x3c, 0xc3, 0x1c, 0x63, 0x8c, 0x30, 0x3,
    0x0,

    /* U+0040 "@" */
    0x1f, 0x83, 0xc, 0x60, 0x6c, 0xf3, 0xd9, 0xbd,
    0x9b, 0xd9, 0xbd, 0x9b, 0xd9, 0xbd, 0x9b, 0xce,
    0xe6, 0x0, 0x70, 0x1, 0xf0,

    /* U+0041 "A" */
    0xc, 0x3, 0x1, 0xe0, 0x58, 0x12, 0xc, 0xc2,
    0x11, 0xfc, 0x61, 0x90, 0x2c, 0xc,

    /* U+0042 "B" */
    0xfe, 0xc3, 0xc3, 0xc3, 0xc2, 0xfc, 0xc2, 0xc3,
    0xc3, 0xc7, 0xfe,

    /* U+0043 "C" */
    0x3c, 0x63, 0xc3, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
    0xc3, 0x63, 0x3c,

    /* U+0044 "D" */
    0xfc, 0xc6, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0xc3, 0xc6, 0xfc,

    /* U+0045 "E" */
    0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xfe, 0xc0, 0xc0,
    0xc0, 0xc0, 0xff,

    /* U+0046 "F" */
    0xff, 0x83, 0x6, 0xc, 0x1f, 0xf0, 0x60, 0xc1,
    0x83, 0x0,

    /* U+0047 "G" */
    0x3c, 0x66, 0xc3, 0xc0, 0xc0, 0xcf, 0xc3, 0xc3,
    0xc3, 0x66, 0x3c,

    /* U+0048 "H" */
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xff, 0xc3, 0xc3,
    0xc3, 0xc3, 0xc3,

    /* U+0049 "I" */
    0xff, 0xff, 0xfc,

    /* U+004A "J" */
    0x6, 0xc, 0x18, 0x30, 0x60, 0xc1, 0x83, 0x6,
    0x99, 0xe0,

    /* U+004B "K" */
    0xc3, 0x63, 0x31, 0x99, 0x8d, 0x87, 0xe3, 0xb1,
    0x8c, 0xc3, 0x61, 0xb0, 0x60,

    /* U+004C "L" */
    0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
    0xc0, 0xc0, 0xff,

    /* U+004D "M" */
    0xc0, 0xf8, 0x7e, 0x1f, 0x87, 0xf3, 0xf4, 0xbd,
    0xef, 0x33, 0xcc, 0xf0, 0x3c, 0xc,

    /* U+004E "N" */
    0xc1, 0xf0, 0xf8, 0x7e, 0x3d, 0x9e, 0x4f, 0x37,
    0x8f, 0xc3, 0xe1, 0xf0, 0x60,

    /* U+004F "O" */
    0x3c, 0x66, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0xc3, 0x66, 0x3c,

    /* U+0050 "P" */
    0xfe, 0xc7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc0, 0xc0,
    0xc0, 0xc0, 0xc0,

    /* U+0051 "Q" */
    0x3c, 0x33, 0x30, 0xd8, 0x6c, 0x36, 0x1b, 0xd,
    0x96, 0xcf, 0x33, 0x8f, 0x60,

    /* U+0052 "R" */
    0xfe, 0xc3, 0xc3, 0xc3, 0xc3, 0xfe, 0xcc, 0xc4,
    0xc6, 0xc3, 0xc3,

    /* U+0053 "S" */
    0x1f, 0x38, 0x98, 0xc, 0x7, 0x81, 0xf8, 0x3e,
    0x3, 0x1, 0xb1, 0x9f, 0x80,

    /* U+0054 "T" */
    0xff, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18,
    0x18, 0x18, 0x18,

    /* U+0055 "U" */
    0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3, 0xc3,
    0xc3, 0x66, 0x3c,

    /* U+0056 "V" */
    0xc1, 0xa0, 0xd8, 0x4c, 0x62, 0x31, 0x90, 0xd8,
    0x2c, 0x1c, 0xe, 0x3, 0x0,

    /* U+0057 "W" */
    0xc2, 0xa, 0x18, 0xd0, 0xc6, 0xce, 0x26, 0x79,
    0x12, 0xd8, 0xb2, 0xc7, 0x94, 0x38, 0xe0, 0xc3,
    0x6, 0x18,

    /* U+0058 "X" */
    0x41, 0x31, 0x8d, 0x86, 0xc1, 0xc0, 0x40, 0x70,
    0x6c, 0x22, 0x31, 0xb0, 0x60,

    /* U+0059 "Y" */
    0xc3, 0xc3, 0x66, 0x66, 0x3c, 0x3c, 0x18, 0x18,
    0x18, 0x18, 0x18,

    /* U+005A "Z" */
    0xfe, 0xc, 0x30, 0x61, 0x82, 0xc, 0x30, 0x61,
    0x83, 0xf8,

    /* U+005B "[" */
    0xfb, 0x6d, 0xb6, 0xdb, 0x6d, 0xb8,

    /* U+005C "\\" */
    0xc0, 0x81, 0x83, 0x2, 0x6, 0x4, 0x8, 0x18,
    0x10, 0x30, 0x60, 0x40, 0xc1, 0x80,

    /* U+005D "]" */
    0xed, 0xb6, 0xdb, 0x6d, 0xb6, 0xf8,

    /* U+005E "^" */
    0x30, 0xc7, 0xb3,

    /* U+005F "_" */
    0xfe,

    /* U+0060 "`" */
    0x99, 0x80,

    /* U+0061 "a" */
    0x7c, 0xc, 0x1b, 0xfc, 0x78, 0xf1, 0xbf,

    /* U+0062 "b" */
    0xc1, 0x83, 0x6, 0xee, 0x78, 0xf1, 0xe3, 0xc7,
    0x8f, 0xf0,

    /* U+0063 "c" */
    0x7d, 0xcb, 0x6, 0xc, 0x18, 0x39, 0x3e,

    /* U+0064 "d" */
    0x6, 0xc, 0x1b, 0xbc, 0xf8, 0xf1, 0xe3, 0xc7,
    0x8d, 0xf8,

    /* U+0065 "e" */
    0x79, 0xcb, 0x17, 0xec, 0x18, 0x39, 0x3e,

    /* U+0066 "f" */
    0x3b, 0x19, 0xf6, 0x31, 0x8c, 0x63, 0x18,

    /* U+0067 "g" */
    0x77, 0x9f, 0x1e, 0x3c, 0x78, 0xf1, 0xbf, 0x6,
    0x8d, 0xf0,

    /* U+0068 "h" */
    0xc1, 0x83, 0x6, 0xee, 0x78, 0xf1, 0xe3, 0xc7,
    0x8f, 0x18,

    /* U+0069 "i" */
    0xc3, 0xff, 0xfc,

    /* U+006A "j" */
    0x60, 0x36, 0xdb, 0x6d, 0xb7, 0x80,

    /* U+006B "k" */
    0xc0, 0xc0, 0xc0, 0xc6, 0xcc, 0xd8, 0xf8, 0xe8,
    0xcc, 0xc6, 0xc6,

    /* U+006C "l" */
    0xdb, 0x6d, 0xb6, 0xd9, 0x80,

    /* U+006D "m" */
    0xdd, 0xee, 0x63, 0xc6, 0x3c, 0x63, 0xc6, 0x3c,
    0x63, 0xc6, 0x3c, 0x63,

    /* U+006E "n" */
    0xdd, 0xcf, 0x1e, 0x3c, 0x78, 0xf1, 0xe3,

    /* U+006F "o" */
    0x7d, 0x8f, 0x1e, 0x3c, 0x78, 0xfb, 0xbe,

    /* U+0070 "p" */
    0xdd, 0xcf, 0x1e, 0x3c, 0x78, 0xf1, 0xfe, 0xc1,
    0x83, 0x0,

    /* U+0071 "q" */
    0x77, 0x9f, 0x1e, 0x3c, 0x78, 0xf1, 0xbf, 0x6,
    0xc, 0x18,

    /* U+0072 "r" */
    0xdf, 0x8c, 0x30, 0xc3, 0xc, 0x30,

    /* U+0073 "s" */
    0x7d, 0x8b, 0x7, 0xe3, 0xe0, 0xf1, 0xfe,

    /* U+0074 "t" */
    0x66, 0xf6, 0x66, 0x66, 0x63,

    /* U+0075 "u" */
    0xc7, 0x8f, 0x1e, 0x3c, 0x78, 0xf1, 0xbf,

    /* U+0076 "v" */
    0x43, 0x66, 0x66, 0x24, 0x3c, 0x3c, 0x18, 0x18,

    /* U+0077 "w" */
    0x8c, 0x79, 0x8b, 0x39, 0x2d, 0x65, 0xac, 0xe7,
    0xc, 0x61, 0xc,

    /* U+0078 "x" */
    0x62, 0x26, 0x3c, 0x18, 0x18, 0x3c, 0x66, 0x62,

    /* U+0079 "y" */
    0xc7, 0x8d, 0x13, 0x62, 0x85, 0xe, 0x8, 0x10,
    0x61, 0x80,

    /* U+007A "z" */
    0xfc, 0x31, 0x8c, 0x31, 0x8c, 0x3f,

    /* U+007B "{" */
    0x36, 0x66, 0x66, 0x66, 0xc6, 0x66, 0x66, 0x63,

    /* U+007C "|" */
    0xff, 0xff, 0xff, 0xc0,

    /* U+007D "}" */
    0xe1, 0x8c, 0x63, 0x18, 0xc6, 0x19, 0x8c, 0x63,
    0x18, 0xdc,

    /* U+007E "~" */
    0xe6, 0x70
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 69, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 69, .box_w = 2, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 4, .adv_w = 97, .box_w = 4, .box_h = 4, .ofs_x = 1, .ofs_y = 7},
    {.bitmap_index = 6, .adv_w = 160, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 19, .adv_w = 148, .box_w = 7, .box_h = 13, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 31, .adv_w = 176, .box_w = 10, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 45, .adv_w = 167, .box_w = 10, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 59, .adv_w = 46, .box_w = 2, .box_h = 4, .ofs_x = 1, .ofs_y = 7},
    {.bitmap_index = 60, .adv_w = 87, .box_w = 4, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 67, .adv_w = 87, .box_w = 4, .box_h = 14, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 74, .adv_w = 110, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 6},
    {.bitmap_index = 78, .adv_w = 135, .box_w = 6, .box_h = 6, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 83, .adv_w = 58, .box_w = 2, .box_h = 4, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 84, .adv_w = 123, .box_w = 5, .box_h = 1, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 85, .adv_w = 58, .box_w = 2, .box_h = 1, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 86, .adv_w = 97, .box_w = 7, .box_h = 15, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 100, .adv_w = 141, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 110, .adv_w = 85, .box_w = 4, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 116, .adv_w = 132, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 126, .adv_w = 135, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 136, .adv_w = 146, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 147, .adv_w = 139, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 156, .adv_w = 131, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 166, .adv_w = 129, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 175, .adv_w = 144, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 185, .adv_w = 131, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 195, .adv_w = 58, .box_w = 2, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 197, .adv_w = 58, .box_w = 2, .box_h = 10, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 200, .adv_w = 109, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 204, .adv_w = 118, .box_w = 5, .box_h = 4, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 207, .adv_w = 110, .box_w = 5, .box_h = 6, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 211, .adv_w = 111, .box_w = 6, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 220, .adv_w = 228, .box_w = 12, .box_h = 14, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 241, .adv_w = 166, .box_w = 10, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 255, .adv_w = 165, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 266, .adv_w = 158, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 277, .adv_w = 168, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 288, .adv_w = 153, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 299, .adv_w = 143, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 309, .adv_w = 166, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 320, .adv_w = 174, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 331, .adv_w = 71, .box_w = 2, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 334, .adv_w = 129, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 344, .adv_w = 164, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 357, .adv_w = 147, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 368, .adv_w = 200, .box_w = 10, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 382, .adv_w = 179, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 395, .adv_w = 165, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 406, .adv_w = 159, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 417, .adv_w = 168, .box_w = 9, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 430, .adv_w = 167, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 441, .adv_w = 158, .box_w = 9, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 454, .adv_w = 123, .box_w = 8, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 465, .adv_w = 168, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 476, .adv_w = 150, .box_w = 9, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 489, .adv_w = 217, .box_w = 13, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 507, .adv_w = 145, .box_w = 9, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 520, .adv_w = 129, .box_w = 8, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 531, .adv_w = 139, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 541, .adv_w = 75, .box_w = 3, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 547, .adv_w = 97, .box_w = 7, .box_h = 15, .ofs_x = -1, .ofs_y = -2},
    {.bitmap_index = 561, .adv_w = 75, .box_w = 3, .box_h = 15, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 567, .adv_w = 129, .box_w = 6, .box_h = 4, .ofs_x = 1, .ofs_y = 8},
    {.bitmap_index = 570, .adv_w = 112, .box_w = 7, .box_h = 1, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 571, .adv_w = 75, .box_w = 3, .box_h = 4, .ofs_x = 1, .ofs_y = 9},
    {.bitmap_index = 573, .adv_w = 136, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 580, .adv_w = 139, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 590, .adv_w = 128, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 597, .adv_w = 138, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 607, .adv_w = 137, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 614, .adv_w = 81, .box_w = 5, .box_h = 11, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 621, .adv_w = 138, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 631, .adv_w = 143, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 641, .adv_w = 64, .box_w = 2, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 644, .adv_w = 69, .box_w = 3, .box_h = 14, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 650, .adv_w = 135, .box_w = 8, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 661, .adv_w = 74, .box_w = 3, .box_h = 11, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 666, .adv_w = 219, .box_w = 12, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 678, .adv_w = 143, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 685, .adv_w = 140, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 692, .adv_w = 139, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 702, .adv_w = 138, .box_w = 7, .box_h = 11, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 712, .adv_w = 109, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 718, .adv_w = 133, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 725, .adv_w = 84, .box_w = 4, .box_h = 10, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 730, .adv_w = 142, .box_w = 7, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 737, .adv_w = 128, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 745, .adv_w = 198, .box_w = 11, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 756, .adv_w = 132, .box_w = 8, .box_h = 8, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 764, .adv_w = 124, .box_w = 7, .box_h = 11, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 774, .adv_w = 127, .box_w = 6, .box_h = 8, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 780, .adv_w = 92, .box_w = 4, .box_h = 16, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 788, .adv_w = 71, .box_w = 2, .box_h = 13, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 792, .adv_w = 92, .box_w = 5, .box_h = 16, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 802, .adv_w = 132, .box_w = 6, .box_h = 2, .ofs_x = 1, .ofs_y = 4}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/



/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 95, .glyph_id_start = 1,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    }
};

/*-----------------
 *    KERNING
 *----------------*/


/*Map glyph_ids to kern left classes*/
static const uint8_t kern_left_class_mapping[] =
{
    0, 0, 0, 1, 2, 3, 0, 0,
    1, 0, 0, 4, 5, 6, 7, 6,
    8, 3, 0, 9, 10, 11, 12, 0,
    13, 0, 13, 6, 6, 5, 5, 5,
    0, 3, 14, 15, 3, 3, 16, 17,
    3, 18, 18, 19, 20, 21, 18, 18,
    3, 22, 23, 24, 25, 26, 3, 27,
    27, 28, 29, 30, 0, 0, 0, 5,
    0, 0, 31, 32, 33, 0, 33, 34,
    31, 31, 35, 35, 31, 23, 31, 31,
    33, 33, 31, 36, 31, 0, 31, 37,
    37, 38, 37, 31, 0, 0, 0, 5
};

/*Map glyph_ids to kern right classes*/
static const uint8_t kern_right_class_mapping[] =
{
    0, 0, 0, 1, 0, 2, 0, 0,
    1, 0, 0, 3, 4, 5, 6, 5,
    7, 2, 8, 0, 9, 10, 11, 12,
    13, 0, 0, 5, 5, 4, 4, 4,
    0, 2, 14, 15, 2, 15, 15, 15,
    2, 15, 15, 16, 15, 15, 15, 15,
    2, 15, 2, 15, 2, 17, 2, 18,
    19, 20, 21, 22, 0, 23, 0, 24,
    0, 0, 25, 0, 26, 26, 26, 27,
    26, 0, 28, 28, 0, 0, 25, 25,
    26, 25, 26, 25, 29, 30, 31, 32,
    32, 33, 32, 25, 0, 0, 0, 4
};

/*Kern values between classes*/
static const int8_t kern_class_values[] =
{
    0, -5, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -10, 0, -12,
    10, 0, 0, 0, 0, 0, 0, 0,
    -5, -7, 0, -5, -5, -6, -5, -5,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -10, 0, -5, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -5, 0, 0, -7, 0,
    0, 0, -1, 0, 0, 0, 0, -7,
    -2, -5, 0, 0, 0, -4, 0, -12,
    0, 0, 0, 0, 0, 0, -1, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -10, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -2, 0, 0,
    0, 0, 0, 0, 0, -11, -7, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -6, -12, -12,
    9, -17, 8, 0, 0, 0, -5, 0,
    0, 0, 0, 0, -15, 0, 0, 0,
    0, 0, 0, 0, 0, -4, 0, 0,
    0, 0, -9, -4, 0, -4, 0, -4,
    -4, 0, -4, 0, 0, 0, 0, 0,
    -4, 0, 0, 0, 0, 0, 0, 0,
    10, 0, 0, 0, 0, 0, 11, 5,
    0, 4, 0, 9, 0, 0, 0, 0,
    17, 17, 0, 17, 0, 0, 0, 0,
    -2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -7, 0, -7, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -10,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -6, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -4, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -14, 0, 0, -10, 0, -10,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 10, 0, 0, -1, 0, 0,
    0, 0, 0, 0, 0, -10, -10, 0,
    0, 0, -4, 0, 0, 0, 0, 0,
    0, 0, 3, 0, 3, -5, -11, -11,
    0, -11, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -4, 0, 0, -4,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -4, 0, -4, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 5,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -11, 0, 0, 0,
    0, 0, 0, 0, 0, -12, 3, -12,
    14, 10, 10, 0, 10, 0, 0, 0,
    0, 0, 0, 4, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -2, 0,
    -2, 10, 5, 5, 0, 5, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -6,
    0, -6, 3, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -7, 0, 0, 9,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 3, 0, 10, 5, 0, 5, 5,
    0, 0, 0, -2, -2, 0, 0, -2,
    0, -2, -6, 0, -11, -5, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 3, 0, 3, -7, -9, -9, 0,
    -9, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -1, 0, 8, -2, 0,
    0, -12, -4, -7, 0, 0, -10, 0,
    -10, 0, -11, -2, -10, 15, 0, 0,
    -2, 0, -5, 0, 0, -6, -11, 0,
    0, -6, 0, -6, -6, -5, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -1, 0,
    0, 0, -1, 0, -1, 0, 0, 0,
    -5, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -6, 0, -2, 0,
    -1, -1, 0, -1, 0, 0, 0, -4,
    -5, 0, -2, -4, 0, -4, -2, 0,
    0, -5, 0, 0, -7, 0, 0, 0,
    -1, 0, 0, 0, 0, 0, -2, 0,
    0, 0, 0, -4, 0, -12, 0, 0,
    0, 0, 0, 0, -1, 0, 0, 0,
    0, 20, 0, 0, 0, -10, 0, 0,
    0, 0, 0, 0, 0, 0, -6, 5,
    -6, 19, 15, 15, 8, 15, 6, 0,
    0, -10, -14, 0, 0, -10, 0, -10,
    -9, -5, 8, -2, 0, 0, -12, -4,
    -7, 0, 0, -10, 0, -10, 0, -11,
    1, -10, 15, 18, 13, 5, 8, -5,
    0, 0, -6, -11, 0, 0, -6, 0,
    -6, -6, -5, 0, -7, 0, 0, 9,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 3, 0, 10, 5, 0, 5, 5,
    0, 0, 0, -2, -2, 0, 0, -2,
    0, -2, -9, 0, 8, -2, 0, 0,
    -17, -4, -7, 0, 0, -10, 0, -10,
    0, -11, 1, -10, 15, 18, 13, 5,
    8, -5, 0, 0, -6, -11, 0, 0,
    -6, 0, -6, -6, -5, 0, -2, 0,
    0, 8, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 6, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -9, 0, -5, 0,
    -2, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -10, -10,
    -10, -2, -10, 0, 0, -2, 0, 0,
    0, 0, 0, 0, 0, -4, 0, -7,
    0, 0, -2, -5, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -14,
    -10, -10, -2, -10, 0, 0, -2, 0,
    0, 0, 0, 0, 0, -3, -5, -4,
    -7, 0, 0, -2, -5, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -14, -11, -11, -2, -11, 0, 0, -2,
    0, 0, 0, 0, 0, 0, -3, -5,
    -4, 0, 0, 0, 0, -10, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 8, 6, 6, 0, 6, 0, 0,
    0, -5, -4, 0, 4, -5, 0, -5,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -5, -5, 0, -5, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -16,
    -12, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -7, 0, 0, 0, 0,
    0, 0, 0, 0, -2, 0, 0, 0,
    0, 0, 0, 0, 0, 3, 0, 0,
    -15, 0, 0, 0, 0, 0, 0, 0,
    0, -4, 0, -4, -6, -2, -2, 0,
    -2, 0, 0, 0, 0, -2, 0, 0,
    0, 3, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -5, 0, 0,
    0, 0, 0, 0, 0, 0, -4, 0,
    0, 0, 0, 0, 0, 0
};


/*Collect the kern class' data in one place*/
static const lv_font_fmt_txt_kern_classes_t kern_classes =
{
    .class_pair_values   = kern_class_values,
    .left_class_mapping  = kern_left_class_mapping,
    .right_class_mapping = kern_right_class_mapping,
    .left_class_cnt      = 38,
    .right_class_cnt     = 33,
};

/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = &kern_classes,
    .kern_scale = 16,
    .cmap_num = 1,
    .bpp = 1,
    .kern_classes = 1,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif
};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t ui_font_Bahnschrit = {
#else
lv_font_t ui_font_Bahnschrit = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 16,          /*The maximum line height required by the font*/
    .base_line = 3,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 0,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if UI_FONT_BAHNSCHRIT*/

