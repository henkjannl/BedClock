#pragma once

/**
 * @file hp_pixel_canvas.h
 * @brief defines a canvas as global variable which
 * acts as a buffer for the GDD memory in the SSD1306
 * it can clear the canvas, write bitmaps on it, and
 * send the canvas to the SSD1306.
 *
 * Everything is currently hard-coded for a SSD1306
 * with 128x32 pixels in a single screen configuration.
 *
 * Pixel data is writeen to a global uint8_t canvas[4][128]
 * variable, which is flushed to the display using the
 * hp_bitmap_write_canvas function
 *
 * The I2C bust first needs to configured to provide a
 * valid esp_lcd_panel_handle_t to the display, see:
 * 
 * https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/lcd.html#i2c-lcd-panel
 * 
*/

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "esp_lcd_panel_ops.h"
#include "esp_log.h"

#include "hp_pixel_buffer.h"

/**
 * @brief Clears the canvas
 */
void hp_bitmap_clear_canvas();

/**
 * @brief Clears the canvas
 *
 * @param bitmap pointer to a hp_bitmap_t to be written on the canvas. If the bitmap
 *               has a mask, all '1' pixels of the bitmap will be written on the canvas
 *               but only pixels where the mask is '1' will be cleared
 * @param x horizontal location at which the bitmap will be drawn. Can also be negative.
 * @param y vertical location at which the bitmap will be drawn. Can also be negative.
 */
void hp_bitmap_draw_bitmap(const hp_bitmap_t *bitmap, int16_t x, int16_t y);

/**
 * @brief Draws a string to the canvas
 *
 * @param font pointer to a hp_font_t to be used for the text
 * @param x horizontal location at which the text will be drawn. This regards the top left corner of the first character of the text
 * @param y vertical location at which the text will be drawn. This regards the top left corner of the first character of the text
 * @param text text to be written
 * @param num_chars number of chars in text
 */
void hp_bitmap_draw_text(hp_font_t font, int16_t x, int16_t y, const unsigned char *text, uint8_t num_chars);

/**
 * @brief Returns the width of a string
 *
 * @param font pointer to a hp_font_t to be used for the text
 * @param text text to be written
 * @param num_chars number of chars in text
 * @return text width in pixels
 */
int16_t hp_bitmap_text_width(hp_font_t font, const unsigned char *text, uint8_t num_chars);

/**
 * @brief Performs some checks to see if the font and the mask are valid
 *
 * @param fontname name of the font
 * @param font pointer to a hp_font_t to be used for the text
 * @return boolean which is true if the test is passed
 */
bool hp_bitmap_check_font(char *fontname, hp_font_t font);

/**
 * @brief Sends the content of the canvas to the panel
 *
 * @param panel handle to the display
 */
void hp_bitmap_write_canvas(esp_lcd_panel_handle_t panel);
