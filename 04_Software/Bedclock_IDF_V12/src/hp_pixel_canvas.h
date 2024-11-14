#pragma once

/*
hp_pixel_canvas defines a canvas as global variable which
acts as a buffer for the GDD memory in the SSD1306
it can clear the canvas, write bitmaps on it, and
send the canvas to the SSD1306
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "esp_lcd_panel_ops.h"
#include "esp_log.h"

#include "hp_pixel_buffer.h"

void hp_bitmap_clear_canvas();
void hp_bitmap_draw_bitmap(const hp_bitmap_t *bitmap, int16_t x, int16_t y);
void hp_bitmap_draw_text(hp_font_t font, int16_t x, int16_t y, const unsigned char *text, uint8_t num_chars);
int16_t hp_bitmap_text_width(hp_font_t font, const unsigned char *text, uint8_t num_chars);
bool hp_bitmap_check_font(char *fontname, hp_font_t font);
void hp_bitmap_write_canvas(esp_lcd_panel_handle_t panel);
