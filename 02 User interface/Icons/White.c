
/*******************************************************************************
* image
* filename: unsaved
* name: White
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



static const uint8_t image_data_White[48] = {
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 
    0x14, 0x95, 0xd8, 
    0x15, 0x54, 0x90, 
    0x15, 0x54, 0xa0, 
    0x07, 0x74, 0x98, 
    0x1d, 0x54, 0xb0, 
    0x0b, 0x4a, 0x90, 
    0x0b, 0x54, 0x90, 
    0x0a, 0x0a, 0x98, 
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00
};
const tImage White = { image_data_White, 24, 16,
    8 };

