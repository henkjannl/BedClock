#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    uint16_t width;
    uint16_t height;
    uint8_t *buffer;
} hp_bitmap_t;

// Define the hp_font_t type that is a
// function which takes a character and returns a pointer to a hp_bitmap_t
typedef const hp_bitmap_t* (*hp_font_t)(char c);

uint16_t hp_bitmap_bytes_per_row(const uint16_t width);
uint16_t hp_bitmap_buffersize(const hp_bitmap_t *bitmap);
int16_t hp_bitmap_pos_to_byte(const hp_bitmap_t *bitmap, const int16_t x, const int16_t y);
int16_t hp_bitmap_pos_to_bit(const int16_t x);

void hp_bitmap_init(hp_bitmap_t *bitmap, const uint16_t width, const uint16_t height);
void hp_bitmap_clear(hp_bitmap_t *bitmap);
void hp_bitmap_delete(hp_bitmap_t *bitmap);

void hp_bitmap_copy(const hp_bitmap_t *input, hp_bitmap_t *output);
void hp_bitmap_transpose(const hp_bitmap_t *input, hp_bitmap_t *output);
void hp_bitmap_invert(hp_bitmap_t *bitmap);
void hp_bitmap_copy_from_array(hp_bitmap_t *bitmap, const uint8_t array[], const uint16_t width, const uint16_t height);
void hp_bitmap_print(hp_bitmap_t *bitmap);
