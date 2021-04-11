
/*******************************************************************************
* image
* filename: unsaved
* name: Orange
*
* preset name: Monochrome
* data block size: 8 bit(s), uint8_t
* RLE compression enabled: no
* conversion type: Monochrome, Diffuse Dither 128
* bits per pixel: 1
*
* preprocess:
*  main scan direction: top_to_bottom
*  line scan direction: forward
*  inverse: no
*******************************************************************************/

/*
 typedef struct {
     const uint8_t *data;
     uint16_t width;
     uint16_t height;
     uint8_t dataSize;
     } tImage;
*/
#include <stdint.h>



static const uint8_t image_data_Orange[48] = {
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 
    0x66, 0x65, 0x66, 
    0x95, 0x55, 0x54, 
    0x55, 0x57, 0x44, 
    0x95, 0x57, 0x46, 
    0x57, 0x75, 0x54, 
    0x95, 0x55, 0x54, 
    0x55, 0x55, 0x54, 
    0x75, 0x55, 0x77, 
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00
};
const tImage Orange = { image_data_Orange, 24, 16,
    8 };

