#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/rmt_tx.h"
#include "esp_log.h"

#define RMT_TX_CHANNEL RMT_CHANNEL_0  // RMT channel to use
#define WS2812_PIN GPIO_NUM_23        // GPIO pin connected to WS2812
#define NUM_LEDS 16                   // Number of WS2812 LEDs in the chain
#define WS2812_BITS_PER_LED 24        // 8 bits for each of R, G, B
#define MAX_INTENSITY 32              // Maximum value of R, G and B to prevent brownout of ESP32 

#define T0H 14  // 0 bit high time (350ns)
#define T1H 52  // 1 bit high time (700ns)
#define T0L 52  // 0 bit low time (800ns)
#define T1L 14  // 1 bit low time (600ns)

// Function to configure the RMT for WS2812
void ws2812_init(void)
{
    rmt_tx_channel_config_t tx_config = {
        .gpio_num = WS2812_PIN,
        .clk_src = RMT_CLK_SRC_DEFAULT,
        .resolution_hz = 80000000,  // 80MHz
        .mem_block_symbols = 64,
        .trans_queue_depth = 4,
        .intr_priority = 0,
    };

    rmt_new_tx_channel(&tx_config, NULL);

    // Enable the RMT TX channel
    rmt_enable(tx_config.channel);
}

uint8_t min(uint8_t a, uint8_t b) 
{
  return a<b ? a : b;    
}

// Function to convert RGB to RMT item array
void ws2812_set_colors(uint8_t colors[][3])
{
    rmt_symbol_word_t items[NUM_LEDS * WS2812_BITS_PER_LED];

    for (int led = 0; led < NUM_LEDS; led++) {
        colors[led][0] = min(colors[led][0],MAX_INTENSITY);
        colors[led][1] = min(colors[led][1],MAX_INTENSITY);
        colors[led][2] = min(colors[led][2],MAX_INTENSITY);

        uint32_t color = (colors[led][1] << 16) | (colors[led][0] << 8) | colors[led][2]; // GRB format

        for (int i = 0; i < WS2812_BITS_PER_LED; i++) {
            if (color & (1 << (23 - i))) {
                // Bit is 1
                items[led * WS2812_BITS_PER_LED + i].duration0 = T1H;
                items[led * WS2812_BITS_PER_LED + i].level0 = 1;
                items[led * WS2812_BITS_PER_LED + i].duration1 = T1L;
                items[led * WS2812_BITS_PER_LED + i].level1 = 0;
            } else {
                // Bit is 0
                items[led * WS2812_BITS_PER_LED + i].duration0 = T0H;
                items[led * WS2812_BITS_PER_LED + i].level0 = 1;
                items[led * WS2812_BITS_PER_LED + i].duration1 = T0L;
                items[led * WS2812_BITS_PER_LED + i].level1 = 0;
            }
        }
    }

    // Create a TX transaction
    rmt_tx_trans_desc_t tx_desc = {
        .symbols = items,
        .num_symbols = NUM_LEDS * WS2812_BITS_PER_LED,
    };

    // Send the data
    rmt_tx_send(tx_desc.channel, &tx_desc, pdMS_TO_TICKS(100));
}

void app_main(void)
{
    ws2812_init();
    
    uint8_t colors[NUM_LEDS][3];

    while (1) {
        // Set all LEDs to red
        for (int i = 0; i < NUM_LEDS; i++) {
            colors[i][0] = MAX_INTENSITY;  // Red
            colors[i][1] = 0;    // Green
            colors[i][2] = 0;    // Blue
        }
        ws2812_set_colors(colors);
        vTaskDelay(pdMS_TO_TICKS(250));

        // Set all LEDs to green
        for (int i = 0; i < NUM_LEDS; i++) {
            colors[i][0] = 0;    // Red
            colors[i][1] = MAX_INTENSITY;  // Green
            colors[i][2] = 0;    // Blue
        }
        ws2812_set_colors(colors);
        vTaskDelay(pdMS_TO_TICKS(250));

        // Set all LEDs to blue
        for (int i = 0; i < NUM_LEDS; i++) {
            colors[i][0] = 0;    // Red
            colors[i][1] = 0;    // Green
            colors[i][2] = MAX_INTENSITY;  // Blue
        }
        ws2812_set_colors(colors);
        vTaskDelay(pdMS_TO_TICKS(250));
    }
}