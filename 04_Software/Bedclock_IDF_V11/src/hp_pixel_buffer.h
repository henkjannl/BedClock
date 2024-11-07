#pragma once

#include <stdio.h>
#include <stdbool.h>
#include <string.h>

typedef struct {
    uint16_t width;
    uint16_t height;
    uint8_t *buffer;
} hp_bitmap_t;


uint16_t hp_bitmap_bytes_per_row(const uint16_t width);
uint16_t hp_bitmap_buffersize(const hp_bitmap_t *bitmap);
int16_t hp_bitmap_pos_to_byte(const hp_bitmap_t *bitmap, const int16_t x, const int16_t y);
void hp_bitmap_init(hp_bitmap_t *bitmap, const uint16_t width, const uint16_t height);
void hp_bitmap_clear(hp_bitmap_t *bitmap);
void hp_bitmap_delete(hp_bitmap_t *bitmap);
void hp_bitmap_copy(const hp_bitmap_t *input, hp_bitmap_t *output);
void hp_bitmap_transpose(const hp_bitmap_t *input, hp_bitmap_t *output);
void hp_bitmap_invert(hp_bitmap_t *bitmap);
void hp_bitmap_copy_from_array(hp_bitmap_t *bitmap, const uint8_t array[], const uint16_t width, const uint16_t height);
void hp_bitmap_print(hp_bitmap_t *bitmap);


void hp_bitmap_clear_canvas();
void hp_bitmap_draw_bitmap(hp_bitmap_t *bitmap, int16_t x, int16_t y);
void hp_write_canvas(esp_lcd_panel_handle_t panel);