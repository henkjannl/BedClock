/*******************************************************************************
 * Size: 24 px
 * Bpp: 1
 * Opts: --bpp 1 --size 24 --font C:/Users/henkj/Documents/GitHub/BedClock/02 User interface/SquareLine Studio/assets/bahnschrift.ttf -o C:/Users/henkj/Documents/GitHub/BedClock/02 User interface/SquareLine Studio/assets\ui_font_Bahnschrit24.c --format lvgl -r 0x20-0x7f --no-compress --no-prefilter
 ******************************************************************************/

// #include "../ui.h"
#include "lvgl.h"

#ifndef UI_FONT_BAHNSCHRIT24
#define UI_FONT_BAHNSCHRIT24 1
#endif

#if UI_FONT_BAHNSCHRIT24

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */
    0x0,

    /* U+0021 "!" */
    0xff, 0xff, 0xff, 0x3, 0xc0,

    /* U+0022 "\"" */
    0xcf, 0x3c, 0xf3, 0xcf, 0x30,

    /* U+0023 "#" */
    0xc, 0x60, 0x63, 0x3, 0x18, 0x18, 0x87, 0xff,
    0xbf, 0xfc, 0x63, 0x3, 0x18, 0x18, 0xc0, 0xc4,
    0x6, 0x61, 0xff, 0xef, 0xff, 0x18, 0xc0, 0xc6,
    0x6, 0x30, 0x31, 0x0,

    /* U+0024 "$" */
    0x6, 0x0, 0x60, 0xf, 0xc3, 0xff, 0x76, 0x66,
    0x60, 0x66, 0x6, 0x60, 0x76, 0x3, 0xf0, 0x3f,
    0xc0, 0x7e, 0x6, 0x70, 0x63, 0x6, 0x34, 0x63,
    0x76, 0x77, 0xfe, 0x1f, 0x80, 0x60, 0x6, 0x0,

    /* U+0025 "%" */
    0x78, 0x33, 0xf1, 0xcc, 0xc6, 0x33, 0x38, 0xcc,
    0xc3, 0x37, 0xf, 0xd8, 0x1e, 0xe0, 0x3, 0x0,
    0x1c, 0x0, 0x67, 0x83, 0xbf, 0xc, 0xcc, 0x63,
    0x31, 0x8c, 0xcc, 0x3f, 0x30, 0x78,

    /* U+0026 "&" */
    0xf, 0x80, 0xfe, 0xe, 0x38, 0x60, 0xc3, 0x6,
    0x18, 0x0, 0xc0, 0x3, 0x0, 0x3c, 0x3, 0xf1,
    0xb9, 0xcd, 0x86, 0x6c, 0x1e, 0x60, 0x73, 0x87,
    0x8f, 0xfe, 0x3f, 0x38,

    /* U+0027 "'" */
    0xff, 0xf0,

    /* U+0028 "(" */
    0x18, 0xe3, 0x1c, 0x61, 0x8e, 0x30, 0xc3, 0xc,
    0x30, 0xc3, 0xc, 0x18, 0x61, 0xc3, 0xe, 0x18,
    0x0,

    /* U+0029 ")" */
    0x61, 0x83, 0xe, 0x18, 0x61, 0xc3, 0xc, 0x30,
    0xc3, 0xc, 0x30, 0xc6, 0x18, 0xe3, 0x1c, 0x60,
    0x0,

    /* U+002A "*" */
    0xc, 0x3, 0x6, 0xdb, 0xff, 0x1e, 0x7, 0x83,
    0x30, 0xcc,

    /* U+002B "+" */
    0xc, 0x3, 0x0, 0xc0, 0x30, 0xff, 0xff, 0xf0,
    0xc0, 0x30, 0xc, 0x3, 0x0,

    /* U+002C "," */
    0xff, 0xa0,

    /* U+002D "-" */
    0xff, 0xff,

    /* U+002E "." */
    0xf0,

    /* U+002F "/" */
    0x0, 0xc0, 0x70, 0x18, 0x6, 0x3, 0x80, 0xc0,
    0x30, 0x1c, 0x6, 0x3, 0x80, 0xc0, 0x30, 0x1c,
    0x6, 0x1, 0x80, 0xe0, 0x30, 0xc, 0x6, 0x1,
    0x80, 0xe0, 0x30, 0x0,

    /* U+0030 "0" */
    0x3f, 0x1f, 0xee, 0x1f, 0x3, 0xc0, 0xf0, 0x3c,
    0xf, 0x3, 0xc0, 0xf0, 0x3c, 0xf, 0x3, 0xc0,
    0xf0, 0x3e, 0x1d, 0xfe, 0x3f, 0x0,

    /* U+0031 "1" */
    0x3b, 0xff, 0x31, 0x8c, 0x63, 0x18, 0xc6, 0x31,
    0x8c, 0x63, 0x18,

    /* U+0032 "2" */
    0x3e, 0x3f, 0xb8, 0xf8, 0x30, 0x18, 0xc, 0xe,
    0x6, 0x7, 0x7, 0x7, 0x3, 0x83, 0x83, 0x83,
    0xc1, 0xff, 0xff, 0x80,

    /* U+0033 "3" */
    0x1f, 0xf, 0xe7, 0x1d, 0x83, 0x0, 0xc0, 0x30,
    0x18, 0x3e, 0xf, 0x80, 0x60, 0xc, 0x3, 0x0,
    0xd8, 0x37, 0x1c, 0xfe, 0x1f, 0x0,

    /* U+0034 "4" */
    0x3, 0x0, 0x30, 0x7, 0x0, 0x60, 0xe, 0x0,
    0xc0, 0x1c, 0x1, 0x8c, 0x38, 0xc3, 0xc, 0x70,
    0xc6, 0xc, 0xff, 0xff, 0xff, 0x0, 0xc0, 0xc,
    0x0, 0xc0,

    /* U+0035 "5" */
    0xff, 0xff, 0xf0, 0x18, 0xc, 0x7, 0xf3, 0xfd,
    0xc6, 0x1, 0x80, 0xc0, 0x60, 0x30, 0x1e, 0xf,
    0x8c, 0xfe, 0x3e, 0x0,

    /* U+0036 "6" */
    0xe, 0x7, 0x7, 0x3, 0x3, 0x81, 0x81, 0xc0,
    0xfc, 0x7f, 0x71, 0xf0, 0x78, 0x3c, 0x1e, 0xf,
    0x8e, 0xfe, 0x3e, 0x0,

    /* U+0037 "7" */
    0xff, 0xff, 0xfc, 0x1b, 0xe, 0xc3, 0x80, 0xc0,
    0x70, 0x1c, 0x6, 0x3, 0x80, 0xe0, 0x30, 0x1c,
    0x7, 0x1, 0xc0, 0xe0, 0x38, 0x0,

    /* U+0038 "8" */
    0x1f, 0xf, 0xe7, 0x1d, 0x83, 0x60, 0xd8, 0x37,
    0x18, 0xfc, 0x7f, 0x98, 0x6c, 0xf, 0x3, 0xc0,
    0xf0, 0x3e, 0x1d, 0xfe, 0x1e, 0x0,

    /* U+0039 "9" */
    0x3e, 0x3f, 0xb8, 0xf8, 0x3c, 0x1e, 0xf, 0x7,
    0xc7, 0x7f, 0x1f, 0x81, 0xc0, 0xc0, 0xe0, 0x60,
    0x70, 0x30, 0x38, 0x0,

    /* U+003A ":" */
    0xf0, 0x0, 0xf,

    /* U+003B ";" */
    0xf0, 0x0, 0x3f, 0xe8,

    /* U+003C "<" */
    0x0, 0x3, 0xf, 0x1e, 0x78, 0x78, 0x1e, 0xf,
    0x3, 0x0,

    /* U+003D "=" */
    0xff, 0xff, 0x0, 0x0, 0xff, 0xff,

    /* U+003E ">" */
    0x0, 0xc0, 0xf0, 0x78, 0x1e, 0x1e, 0x78, 0xf0,
    0xc0, 0x0,

    /* U+003F "?" */
    0x3e, 0x3f, 0xb8, 0xf8, 0x3c, 0x18, 0xc, 0xe,
    0xe, 0x6, 0x6, 0x7, 0x3, 0x1, 0x80, 0x0,
    0x0, 0x30, 0x18, 0x0,

    /* U+0040 "@" */
    0x7, 0xf8, 0x7, 0xff, 0x83, 0xc0, 0xf1, 0xc0,
    0xe, 0x60, 0x1, 0x98, 0xf8, 0x6c, 0x7f, 0xf,
    0x38, 0xe3, 0xcc, 0x18, 0xf3, 0x6, 0x3c, 0xc1,
    0x8f, 0x30, 0x63, 0xcc, 0x18, 0xf3, 0x6, 0x3c,
    0xe3, 0x8f, 0x1f, 0xff, 0xe3, 0xcf, 0x98, 0x0,
    0x7, 0x0, 0x0, 0xf0, 0x0, 0x1f, 0xf0, 0x1,
    0xfc, 0x0,

    /* U+0041 "A" */
    0x3, 0x80, 0x7, 0x0, 0x1e, 0x0, 0x3e, 0x0,
    0x6c, 0x1, 0xd8, 0x3, 0x18, 0x6, 0x30, 0x1c,
    0x70, 0x30, 0x60, 0x60, 0xc1, 0xff, 0xc3, 0xff,
    0x8e, 0x3, 0x1c, 0x7, 0x30, 0x6, 0xe0, 0xe,

    /* U+0042 "B" */
    0xff, 0xcf, 0xfe, 0xc0, 0x7c, 0x3, 0xc0, 0x3c,
    0x3, 0xc0, 0x6f, 0xfc, 0xff, 0xcc, 0x6, 0xc0,
    0x3c, 0x3, 0xc0, 0x3c, 0x3, 0xc0, 0x6f, 0xfe,
    0xff, 0x80,

    /* U+0043 "C" */
    0x1f, 0x7, 0xf9, 0xc7, 0x70, 0x7c, 0x7, 0x80,
    0x30, 0x6, 0x0, 0xc0, 0x18, 0x3, 0x0, 0x60,
    0xc, 0x7, 0xc1, 0xdc, 0x71, 0xfe, 0x1f, 0x0,

    /* U+0044 "D" */
    0xff, 0xf, 0xfc, 0xc0, 0xec, 0x7, 0xc0, 0x3c,
    0x3, 0xc0, 0x3c, 0x3, 0xc0, 0x3c, 0x3, 0xc0,
    0x3c, 0x3, 0xc0, 0x3c, 0x7, 0xc0, 0xef, 0xfc,
    0xff, 0x0,

    /* U+0045 "E" */
    0xff, 0xff, 0xff, 0x0, 0x60, 0xc, 0x1, 0x80,
    0x30, 0x6, 0x0, 0xff, 0xdf, 0xfb, 0x0, 0x60,
    0xc, 0x1, 0x80, 0x30, 0x7, 0xff, 0xff, 0xe0,

    /* U+0046 "F" */
    0xff, 0xff, 0xff, 0x0, 0x60, 0xc, 0x1, 0x80,
    0x30, 0x6, 0x0, 0xff, 0xdf, 0xfb, 0x0, 0x60,
    0xc, 0x1, 0x80, 0x30, 0x6, 0x0, 0xc0, 0x0,

    /* U+0047 "G" */
    0x1f, 0x3, 0xfc, 0x70, 0xee, 0x6, 0xc0, 0x3c,
    0x0, 0xc0, 0xc, 0x0, 0xc3, 0xfc, 0x3f, 0xc0,
    0x3c, 0x3, 0xc0, 0x3e, 0x7, 0x70, 0xe3, 0xfc,
    0x1f, 0x80,

    /* U+0048 "H" */
    0xc0, 0x3c, 0x3, 0xc0, 0x3c, 0x3, 0xc0, 0x3c,
    0x3, 0xc0, 0x3c, 0x3, 0xff, 0xff, 0xff, 0xc0,
    0x3c, 0x3, 0xc0, 0x3c, 0x3, 0xc0, 0x3c, 0x3,
    0xc0, 0x30,

    /* U+0049 "I" */
    0xff, 0xff, 0xff, 0xff, 0xc0,

    /* U+004A "J" */
    0x1, 0x80, 0xc0, 0x60, 0x30, 0x18, 0xc, 0x6,
    0x3, 0x1, 0x80, 0xc0, 0x60, 0x30, 0x18, 0xd,
    0xd, 0xfe, 0x7c, 0x0,

    /* U+004B "K" */
    0xc0, 0x76, 0x7, 0x30, 0x71, 0x87, 0xc, 0x70,
    0x63, 0x3, 0x30, 0x1b, 0xc0, 0xff, 0x7, 0xb8,
    0x38, 0xe1, 0x87, 0xc, 0x1c, 0x60, 0x73, 0x3,
    0x98, 0xe, 0xc0, 0x70,

    /* U+004C "L" */
    0xc0, 0x18, 0x3, 0x0, 0x60, 0xc, 0x1, 0x80,
    0x30, 0x6, 0x0, 0xc0, 0x18, 0x3, 0x0, 0x60,
    0xc, 0x1, 0x80, 0x30, 0x7, 0xff, 0xff, 0xe0,

    /* U+004D "M" */
    0xc0, 0xf, 0x80, 0x7e, 0x1, 0xfc, 0xf, 0xf0,
    0x3f, 0xc0, 0xff, 0x86, 0xf6, 0x1b, 0xdc, 0xef,
    0x33, 0x3c, 0xfc, 0xf1, 0xe3, 0xc7, 0x8f, 0xc,
    0x3c, 0x30, 0xf0, 0x3, 0xc0, 0xc,

    /* U+004E "N" */
    0xc0, 0x3e, 0x3, 0xe0, 0x3f, 0x3, 0xf8, 0x3d,
    0x83, 0xdc, 0x3c, 0xe3, 0xc6, 0x3c, 0x73, 0xc3,
    0xbc, 0x1b, 0xc1, 0xfc, 0xf, 0xc0, 0x7c, 0x7,
    0xc0, 0x30,

    /* U+004F "O" */
    0x1f, 0x83, 0xfc, 0x70, 0xee, 0x7, 0xc0, 0x3c,
    0x3, 0xc0, 0x3c, 0x3, 0xc0, 0x3c, 0x3, 0xc0,
    0x3c, 0x3, 0xc0, 0x3e, 0x7, 0x70, 0xe3, 0xfc,
    0x1f, 0x80,

    /* U+0050 "P" */
    0xff, 0x9f, 0xfb, 0x3, 0x60, 0x3c, 0x7, 0x80,
    0xf0, 0x1e, 0x6, 0xff, 0xdf, 0xe3, 0x0, 0x60,
    0xc, 0x1, 0x80, 0x30, 0x6, 0x0, 0xc0, 0x0,

    /* U+0051 "Q" */
    0x1f, 0x80, 0xff, 0x7, 0xe, 0x38, 0x1c, 0xc0,
    0x33, 0x0, 0xcc, 0x3, 0x30, 0xc, 0xc0, 0x33,
    0x0, 0xcc, 0x3, 0x30, 0x4c, 0xc3, 0xf3, 0x87,
    0xc7, 0xf, 0xf, 0xfe, 0x1f, 0x98,

    /* U+0052 "R" */
    0xff, 0x8f, 0xfc, 0xc0, 0xcc, 0x6, 0xc0, 0x6c,
    0x6, 0xc0, 0x6c, 0xc, 0xff, 0xcf, 0xf8, 0xc3,
    0x8c, 0x38, 0xc1, 0xcc, 0x1c, 0xc0, 0xec, 0x6,
    0xc0, 0x60,

    /* U+0053 "S" */
    0xf, 0xc3, 0xff, 0x30, 0x66, 0x0, 0x60, 0x6,
    0x0, 0x70, 0x3, 0xf0, 0x3f, 0xc0, 0x7e, 0x0,
    0x70, 0x3, 0x0, 0x34, 0x3, 0x70, 0x67, 0xfe,
    0x1f, 0x80,

    /* U+0054 "T" */
    0xff, 0xff, 0xff, 0x6, 0x0, 0x60, 0x6, 0x0,
    0x60, 0x6, 0x0, 0x60, 0x6, 0x0, 0x60, 0x6,
    0x0, 0x60, 0x6, 0x0, 0x60, 0x6, 0x0, 0x60,
    0x6, 0x0,

    /* U+0055 "U" */
    0xc0, 0x3c, 0x3, 0xc0, 0x3c, 0x3, 0xc0, 0x3c,
    0x3, 0xc0, 0x3c, 0x3, 0xc0, 0x3c, 0x3, 0xc0,
    0x3c, 0x3, 0xc0, 0x3e, 0x7, 0x70, 0xe3, 0xfc,
    0x1f, 0x80,

    /* U+0056 "V" */
    0x60, 0x1d, 0x80, 0x67, 0x3, 0x9c, 0xe, 0x30,
    0x30, 0xe1, 0xc3, 0x87, 0x6, 0x18, 0x1c, 0x60,
    0x73, 0x80, 0xcc, 0x3, 0x30, 0xf, 0xc0, 0x1e,
    0x0, 0x78, 0x1, 0xe0, 0x3, 0x0,

    /* U+0057 "W" */
    0xe0, 0x60, 0x76, 0x6, 0x7, 0x60, 0x70, 0x67,
    0xf, 0x6, 0x70, 0xf0, 0xe3, 0xf, 0xe, 0x30,
    0xd8, 0xc3, 0x9d, 0x8c, 0x39, 0x98, 0xc1, 0x99,
    0x9c, 0x1b, 0x8d, 0x81, 0xf0, 0xd8, 0x1f, 0xd,
    0x80, 0xf0, 0xf8, 0xf, 0x7, 0x0, 0xe0, 0x70,
    0xe, 0x7, 0x0,

    /* U+0058 "X" */
    0x60, 0x3b, 0x81, 0x8e, 0x1c, 0x31, 0xc1, 0xcc,
    0x6, 0xe0, 0x3e, 0x0, 0xf0, 0x7, 0x0, 0x3c,
    0x3, 0xe0, 0x1b, 0x81, 0xce, 0x1c, 0x30, 0xc1,
    0xce, 0x6, 0x60, 0x38,

    /* U+0059 "Y" */
    0xe0, 0x76, 0x6, 0x70, 0xe3, 0xc, 0x39, 0xc1,
    0x98, 0x19, 0x80, 0xf0, 0xf, 0x0, 0x60, 0x6,
    0x0, 0x60, 0x6, 0x0, 0x60, 0x6, 0x0, 0x60,
    0x6, 0x0,

    /* U+005A "Z" */
    0xff, 0xff, 0xf0, 0x1c, 0xe, 0x3, 0x81, 0xc0,
    0x60, 0x38, 0xc, 0x7, 0x1, 0x80, 0xe0, 0x70,
    0x1c, 0xe, 0x3, 0xff, 0xff, 0xc0,

    /* U+005B "[" */
    0xff, 0xf1, 0x8c, 0x63, 0x18, 0xc6, 0x31, 0x8c,
    0x63, 0x18, 0xc6, 0x31, 0x8f, 0xfc,

    /* U+005C "\\" */
    0xc0, 0x38, 0x6, 0x1, 0x80, 0x70, 0xc, 0x3,
    0x80, 0x60, 0x18, 0x7, 0x0, 0xc0, 0x30, 0xe,
    0x1, 0x80, 0x60, 0xc, 0x3, 0x0, 0xe0, 0x18,
    0x6, 0x1, 0xc0, 0x30,

    /* U+005D "]" */
    0xff, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33, 0x33,
    0x33, 0x33, 0xff,

    /* U+005E "^" */
    0xc, 0x3, 0x1, 0xe0, 0xfc, 0x73, 0x98, 0x60,

    /* U+005F "_" */
    0xff, 0xff, 0xf0,

    /* U+0060 "`" */
    0x47, 0x1c, 0x71, 0x0,

    /* U+0061 "a" */
    0x3f, 0x1f, 0xe2, 0x1c, 0x3, 0x0, 0xcf, 0xff,
    0xff, 0x3, 0xc0, 0xf8, 0x37, 0xfc, 0xfb,

    /* U+0062 "b" */
    0xc0, 0x60, 0x30, 0x18, 0xc, 0x6, 0xf3, 0xfd,
    0xc7, 0xc1, 0xe0, 0xf0, 0x78, 0x3c, 0x1e, 0xf,
    0x8f, 0xfe, 0xde, 0x0,

    /* U+0063 "c" */
    0x1f, 0x3f, 0xf8, 0x58, 0xc, 0x6, 0x3, 0x1,
    0x80, 0xc0, 0x70, 0x9f, 0xe3, 0xe0,

    /* U+0064 "d" */
    0x1, 0x80, 0xc0, 0x60, 0x30, 0x19, 0xed, 0xff,
    0xc7, 0xc1, 0xe0, 0xf0, 0x78, 0x3c, 0x1e, 0xf,
    0x8e, 0xff, 0x3d, 0x80,

    /* U+0065 "e" */
    0x1e, 0x1f, 0xe6, 0x1b, 0x7, 0xc0, 0xff, 0xff,
    0xff, 0x0, 0xc0, 0x18, 0x67, 0xfc, 0x7c,

    /* U+0066 "f" */
    0x1e, 0x7c, 0xc1, 0x83, 0x1f, 0xff, 0x98, 0x30,
    0x60, 0xc1, 0x83, 0x6, 0xc, 0x18, 0x30,

    /* U+0067 "g" */
    0x3d, 0xbf, 0xf8, 0xf8, 0x3c, 0x1e, 0xf, 0x7,
    0x83, 0xc1, 0xf1, 0xdf, 0xe7, 0xb0, 0x18, 0xd,
    0x8c, 0xfe, 0x3c, 0x0,

    /* U+0068 "h" */
    0xc0, 0x30, 0xc, 0x3, 0x0, 0xc0, 0x37, 0xcf,
    0xfb, 0x87, 0xc0, 0xf0, 0x3c, 0xf, 0x3, 0xc0,
    0xf0, 0x3c, 0xf, 0x3, 0xc0, 0xc0,

    /* U+0069 "i" */
    0xf0, 0x3f, 0xff, 0xff, 0xc0,

    /* U+006A "j" */
    0x33, 0x0, 0x3, 0x33, 0x33, 0x33, 0x33, 0x33,
    0x33, 0x33, 0xfe,

    /* U+006B "k" */
    0xc0, 0x30, 0xc, 0x3, 0x0, 0xc0, 0x30, 0xec,
    0x33, 0x1c, 0xce, 0x37, 0xf, 0xc3, 0xf8, 0xe7,
    0x30, 0xcc, 0x3b, 0x6, 0xc1, 0xc0,

    /* U+006C "l" */
    0xc6, 0x31, 0x8c, 0x63, 0x18, 0xc6, 0x31, 0x8c,
    0x63, 0x1f, 0x78,

    /* U+006D "m" */
    0xde, 0x3c, 0xff, 0x7e, 0xe3, 0xc7, 0xc1, 0x83,
    0xc1, 0x83, 0xc1, 0x83, 0xc1, 0x83, 0xc1, 0x83,
    0xc1, 0x83, 0xc1, 0x83, 0xc1, 0x83, 0xc1, 0x83,

    /* U+006E "n" */
    0xdf, 0x3f, 0xee, 0x1f, 0x3, 0xc0, 0xf0, 0x3c,
    0xf, 0x3, 0xc0, 0xf0, 0x3c, 0xf, 0x3,

    /* U+006F "o" */
    0x1e, 0x1f, 0xee, 0x1b, 0x3, 0xc0, 0xf0, 0x3c,
    0xf, 0x3, 0xc0, 0xf8, 0x67, 0xf8, 0x78,

    /* U+0070 "p" */
    0xde, 0x7f, 0xb8, 0xf8, 0x3c, 0x1e, 0xf, 0x7,
    0x83, 0xc1, 0xf1, 0xff, 0xdb, 0xcc, 0x6, 0x3,
    0x1, 0x80, 0xc0, 0x0,

    /* U+0071 "q" */
    0x3d, 0xbf, 0xf8, 0xf8, 0x3c, 0x1e, 0xf, 0x7,
    0x83, 0xc1, 0xf1, 0xdf, 0xe7, 0xb0, 0x18, 0xc,
    0x6, 0x3, 0x1, 0x80,

    /* U+0072 "r" */
    0xde, 0xfe, 0xe2, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0,
    0xc0, 0xc0, 0xc0, 0xc0,

    /* U+0073 "s" */
    0x1f, 0x87, 0xfd, 0xc3, 0x30, 0x7, 0x0, 0xff,
    0x87, 0xf8, 0x3, 0x0, 0x6c, 0x1d, 0xff, 0xf,
    0xc0,

    /* U+0074 "t" */
    0x63, 0x18, 0xcf, 0xfd, 0x8c, 0x63, 0x18, 0xc6,
    0x31, 0xe7,

    /* U+0075 "u" */
    0xc0, 0xf0, 0x3c, 0xf, 0x3, 0xc0, 0xf0, 0x3c,
    0xf, 0x3, 0xc0, 0xf8, 0x77, 0xfc, 0xfb,

    /* U+0076 "v" */
    0xc0, 0xdc, 0x39, 0x87, 0x30, 0xc7, 0x38, 0x66,
    0xc, 0xc1, 0xf8, 0x1e, 0x3, 0xc0, 0x38, 0x6,
    0x0,

    /* U+0077 "w" */
    0xc1, 0x83, 0xf0, 0xe1, 0x98, 0x70, 0xcc, 0x78,
    0xe6, 0x36, 0x61, 0x9b, 0x30, 0xd9, 0x98, 0x6c,
    0x7c, 0x3e, 0x3c, 0xe, 0x1e, 0x7, 0x7, 0x3,
    0x83, 0x0,

    /* U+0078 "x" */
    0xe1, 0xce, 0x30, 0xce, 0x1d, 0x81, 0xe0, 0x1c,
    0x7, 0x80, 0xf8, 0x3b, 0xe, 0x71, 0x87, 0x70,
    0x60,

    /* U+0079 "y" */
    0x60, 0x6c, 0x1d, 0xc3, 0x18, 0x63, 0x1c, 0x73,
    0x6, 0x60, 0xdc, 0xf, 0x1, 0xe0, 0x3c, 0x3,
    0x0, 0x60, 0x18, 0x3, 0x1, 0xe0, 0x38, 0x0,

    /* U+007A "z" */
    0xff, 0xff, 0xc0, 0xe0, 0xe0, 0xe0, 0x60, 0x70,
    0x70, 0x70, 0x70, 0x3f, 0xff, 0xf0,

    /* U+007B "{" */
    0xe, 0x3c, 0xe1, 0x83, 0x6, 0xc, 0x18, 0x30,
    0x61, 0xc7, 0xe, 0xe, 0xc, 0x18, 0x30, 0x60,
    0xc1, 0x83, 0x7, 0x7, 0x87,

    /* U+007C "|" */
    0xff, 0xff, 0xff, 0xff, 0xff,

    /* U+007D "}" */
    0xc1, 0xc1, 0xc1, 0x83, 0x6, 0xc, 0x18, 0x30,
    0x60, 0xe0, 0xf1, 0xe7, 0xc, 0x18, 0x30, 0x60,
    0xc1, 0x83, 0xe, 0x38, 0x60,

    /* U+007E "~" */
    0x71, 0xfc, 0xf3, 0xf8, 0xe0
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 103, .box_w = 1, .box_h = 1, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1, .adv_w = 104, .box_w = 2, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 6, .adv_w = 145, .box_w = 6, .box_h = 6, .ofs_x = 2, .ofs_y = 11},
    {.bitmap_index = 11, .adv_w = 240, .box_w = 13, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 39, .adv_w = 221, .box_w = 12, .box_h = 21, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 71, .adv_w = 264, .box_w = 14, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 101, .adv_w = 250, .box_w = 13, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 129, .adv_w = 68, .box_w = 2, .box_h = 6, .ofs_x = 1, .ofs_y = 11},
    {.bitmap_index = 131, .adv_w = 130, .box_w = 6, .box_h = 22, .ofs_x = 2, .ofs_y = -4},
    {.bitmap_index = 148, .adv_w = 130, .box_w = 6, .box_h = 22, .ofs_x = 0, .ofs_y = -4},
    {.bitmap_index = 165, .adv_w = 165, .box_w = 10, .box_h = 8, .ofs_x = 0, .ofs_y = 9},
    {.bitmap_index = 175, .adv_w = 203, .box_w = 10, .box_h = 10, .ofs_x = 1, .ofs_y = 2},
    {.bitmap_index = 188, .adv_w = 87, .box_w = 2, .box_h = 6, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 190, .adv_w = 184, .box_w = 8, .box_h = 2, .ofs_x = 2, .ofs_y = 5},
    {.bitmap_index = 192, .adv_w = 87, .box_w = 2, .box_h = 2, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 193, .adv_w = 145, .box_w = 10, .box_h = 22, .ofs_x = 0, .ofs_y = -3},
    {.bitmap_index = 221, .adv_w = 212, .box_w = 10, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 243, .adv_w = 128, .box_w = 5, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 254, .adv_w = 198, .box_w = 9, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 274, .adv_w = 203, .box_w = 10, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 296, .adv_w = 218, .box_w = 12, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 322, .adv_w = 209, .box_w = 9, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 342, .adv_w = 197, .box_w = 9, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 362, .adv_w = 193, .box_w = 10, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 384, .adv_w = 216, .box_w = 10, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 406, .adv_w = 197, .box_w = 9, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 426, .adv_w = 87, .box_w = 2, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 429, .adv_w = 87, .box_w = 2, .box_h = 15, .ofs_x = 1, .ofs_y = -3},
    {.bitmap_index = 433, .adv_w = 163, .box_w = 8, .box_h = 10, .ofs_x = 1, .ofs_y = 3},
    {.bitmap_index = 443, .adv_w = 176, .box_w = 8, .box_h = 6, .ofs_x = 2, .ofs_y = 5},
    {.bitmap_index = 449, .adv_w = 165, .box_w = 8, .box_h = 10, .ofs_x = 2, .ofs_y = 3},
    {.bitmap_index = 459, .adv_w = 166, .box_w = 9, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 479, .adv_w = 342, .box_w = 18, .box_h = 22, .ofs_x = 2, .ofs_y = -5},
    {.bitmap_index = 529, .adv_w = 249, .box_w = 15, .box_h = 17, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 561, .adv_w = 248, .box_w = 12, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 587, .adv_w = 238, .box_w = 11, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 611, .adv_w = 252, .box_w = 12, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 637, .adv_w = 230, .box_w = 11, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 661, .adv_w = 215, .box_w = 11, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 685, .adv_w = 248, .box_w = 12, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 711, .adv_w = 261, .box_w = 12, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 737, .adv_w = 106, .box_w = 2, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 742, .adv_w = 194, .box_w = 9, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 762, .adv_w = 246, .box_w = 13, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 790, .adv_w = 220, .box_w = 11, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 814, .adv_w = 299, .box_w = 14, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 844, .adv_w = 269, .box_w = 12, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 870, .adv_w = 248, .box_w = 12, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 896, .adv_w = 239, .box_w = 11, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 920, .adv_w = 252, .box_w = 14, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 950, .adv_w = 250, .box_w = 12, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 976, .adv_w = 236, .box_w = 12, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1002, .adv_w = 185, .box_w = 12, .box_h = 17, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1028, .adv_w = 252, .box_w = 12, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1054, .adv_w = 225, .box_w = 14, .box_h = 17, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1084, .adv_w = 325, .box_w = 20, .box_h = 17, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1127, .adv_w = 218, .box_w = 13, .box_h = 17, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1155, .adv_w = 193, .box_w = 12, .box_h = 17, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1181, .adv_w = 209, .box_w = 10, .box_h = 17, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1203, .adv_w = 113, .box_w = 5, .box_h = 22, .ofs_x = 2, .ofs_y = -4},
    {.bitmap_index = 1217, .adv_w = 145, .box_w = 10, .box_h = 22, .ofs_x = -1, .ofs_y = -3},
    {.bitmap_index = 1245, .adv_w = 113, .box_w = 4, .box_h = 22, .ofs_x = 1, .ofs_y = -4},
    {.bitmap_index = 1256, .adv_w = 193, .box_w = 10, .box_h = 6, .ofs_x = 1, .ofs_y = 11},
    {.bitmap_index = 1264, .adv_w = 167, .box_w = 10, .box_h = 2, .ofs_x = 1, .ofs_y = -2},
    {.bitmap_index = 1267, .adv_w = 113, .box_w = 5, .box_h = 5, .ofs_x = 1, .ofs_y = 14},
    {.bitmap_index = 1271, .adv_w = 204, .box_w = 10, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1286, .adv_w = 209, .box_w = 9, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1306, .adv_w = 191, .box_w = 9, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1320, .adv_w = 207, .box_w = 9, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1340, .adv_w = 206, .box_w = 10, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1355, .adv_w = 122, .box_w = 7, .box_h = 17, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 1370, .adv_w = 207, .box_w = 9, .box_h = 17, .ofs_x = 2, .ofs_y = -5},
    {.bitmap_index = 1390, .adv_w = 215, .box_w = 10, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1412, .adv_w = 96, .box_w = 2, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1417, .adv_w = 103, .box_w = 4, .box_h = 22, .ofs_x = 1, .ofs_y = -5},
    {.bitmap_index = 1428, .adv_w = 203, .box_w = 10, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1450, .adv_w = 111, .box_w = 5, .box_h = 17, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1461, .adv_w = 329, .box_w = 16, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1485, .adv_w = 215, .box_w = 10, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1500, .adv_w = 209, .box_w = 10, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1515, .adv_w = 209, .box_w = 9, .box_h = 17, .ofs_x = 2, .ofs_y = -5},
    {.bitmap_index = 1535, .adv_w = 207, .box_w = 9, .box_h = 17, .ofs_x = 2, .ofs_y = -5},
    {.bitmap_index = 1555, .adv_w = 164, .box_w = 8, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1567, .adv_w = 199, .box_w = 11, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1584, .adv_w = 125, .box_w = 5, .box_h = 16, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1594, .adv_w = 214, .box_w = 10, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1609, .adv_w = 192, .box_w = 11, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1626, .adv_w = 297, .box_w = 17, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1652, .adv_w = 199, .box_w = 11, .box_h = 12, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 1669, .adv_w = 185, .box_w = 11, .box_h = 17, .ofs_x = 0, .ofs_y = -5},
    {.bitmap_index = 1693, .adv_w = 190, .box_w = 9, .box_h = 12, .ofs_x = 2, .ofs_y = 0},
    {.bitmap_index = 1707, .adv_w = 138, .box_w = 7, .box_h = 24, .ofs_x = 2, .ofs_y = -5},
    {.bitmap_index = 1728, .adv_w = 106, .box_w = 2, .box_h = 20, .ofs_x = 2, .ofs_y = -2},
    {.bitmap_index = 1733, .adv_w = 138, .box_w = 7, .box_h = 24, .ofs_x = 1, .ofs_y = -5},
    {.bitmap_index = 1754, .adv_w = 197, .box_w = 9, .box_h = 4, .ofs_x = 2, .ofs_y = 6}
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
    0, -7, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -15, 0, -19,
    15, 0, 0, 0, 0, 0, 0, 0,
    -7, -11, 0, -7, -7, -9, -7, -7,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -15, 0, -7, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -7, 0, 0, -11, 0,
    0, 0, -2, 0, 0, 0, 0, -11,
    -4, -7, 0, 0, 0, -6, 0, -19,
    0, 0, 0, 0, 0, 0, -2, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -15, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -4, 0, 0,
    0, 0, 0, 0, 0, -17, -11, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -9, -19, -19,
    13, -26, 11, 0, 0, 0, -7, 0,
    0, 0, 0, 0, -22, 0, 0, 0,
    0, 0, 0, 0, 0, -6, 0, 0,
    0, 0, -13, -6, 0, -6, 0, -6,
    -6, 0, -6, 0, 0, 0, 0, 0,
    -6, 0, 0, 0, 0, 0, 0, 0,
    15, 0, 0, 0, 0, 0, 17, 8,
    0, 6, 0, 13, 0, 0, 0, 0,
    25, 25, 0, 25, 0, 0, 0, 0,
    -4, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -11, 0, -11, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -15,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -9, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, -6, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, -21, 0, 0, -15, 0, -15,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 15, 0, 0, -2, 0, 0,
    0, 0, 0, 0, 0, -15, -15, 0,
    0, 0, -6, 0, 0, 0, 0, 0,
    0, 0, 4, 0, 4, -7, -17, -17,
    0, -17, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -6, 0, 0, -6,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -6, 0, -6, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 8,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -17, 0, 0, 0,
    0, 0, 0, 0, 0, -19, 4, -19,
    21, 15, 15, 0, 15, 0, 0, 0,
    0, 0, 0, 6, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -4, 0,
    -4, 15, 8, 8, 0, 8, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -9,
    0, -9, 4, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -11, 0, 0, 13,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 4, 0, 15, 8, 0, 8, 8,
    0, 0, 0, -4, -4, 0, 0, -4,
    0, -4, -9, 0, -17, -7, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 4, 0, 4, -11, -13, -13, 0,
    -13, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -1, 0, 11, -4, 0,
    0, -19, -6, -10, 0, 0, -15, 0,
    -15, 0, -17, -4, -15, 23, 0, 0,
    -4, 0, -7, 0, 0, -9, -17, 0,
    0, -9, 0, -9, -9, -7, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -2, 0,
    0, 0, -2, 0, -2, 0, 0, 0,
    -7, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -9, 0, -4, 0,
    -2, -2, 0, -2, 0, 0, 0, -6,
    -7, 0, -4, -6, 0, -6, -4, 0,
    0, -7, 0, 0, -11, 0, 0, 0,
    -2, 0, 0, 0, 0, 0, -4, 0,
    0, 0, 0, -6, 0, -19, 0, 0,
    0, 0, 0, 0, -2, 0, 0, 0,
    0, 30, 0, 0, 0, -15, 0, 0,
    0, 0, 0, 0, 0, 0, -9, 8,
    -9, 28, 23, 23, 11, 23, 9, 0,
    0, -15, -21, 0, 0, -15, 0, -15,
    -13, -7, 11, -4, 0, 0, -19, -6,
    -10, 0, 0, -15, 0, -15, 0, -17,
    2, -15, 23, 26, 19, 8, 11, -7,
    0, 0, -9, -17, 0, 0, -9, 0,
    -9, -9, -7, 0, -11, 0, 0, 13,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 4, 0, 15, 8, 0, 8, 8,
    0, 0, 0, -4, -4, 0, 0, -4,
    0, -4, -13, 0, 11, -4, 0, 0,
    -26, -6, -10, 0, 0, -15, 0, -15,
    0, -17, 2, -15, 23, 26, 19, 8,
    11, -7, 0, 0, -9, -17, 0, 0,
    -9, 0, -9, -9, -7, 0, -4, 0,
    0, 11, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 9, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, -13, 0, -7, 0,
    -4, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, -15, -15,
    -15, -4, -15, 0, 0, -4, 0, 0,
    0, 0, 0, 0, 0, -6, 0, -11,
    0, 0, -4, -7, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -21,
    -15, -15, -4, -15, 0, 0, -4, 0,
    0, 0, 0, 0, 0, -5, -7, -6,
    -11, 0, 0, -4, -7, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    -21, -17, -17, -4, -17, 0, 0, -4,
    0, 0, 0, 0, 0, 0, -5, -7,
    -6, 0, 0, 0, 0, -15, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 11, 9, 9, 0, 9, 0, 0,
    0, -7, -6, 0, 6, -7, 0, -7,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -7, -7, 0, -7, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, -24,
    -19, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, -11, 0, 0, 0, 0,
    0, 0, 0, 0, -4, 0, 0, 0,
    0, 0, 0, 0, 0, 4, 0, 0,
    -22, 0, 0, 0, 0, 0, 0, 0,
    0, -6, 0, -6, -9, -4, -4, 0,
    -4, 0, 0, 0, 0, -4, 0, 0,
    0, 4, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, -7, 0, 0,
    0, 0, 0, 0, 0, 0, -6, 0,
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
const lv_font_t ui_font_Bahnschrit24 = {
#else
lv_font_t ui_font_Bahnschrit24 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 24,          /*The maximum line height required by the font*/
    .base_line = 5,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = -1,
    .underline_thickness = 1,
#endif
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if UI_FONT_BAHNSCHRIT24*/

