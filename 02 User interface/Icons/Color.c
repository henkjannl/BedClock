
/*******************************************************************************
* image
* filename: unsaved
* name: Color
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



static const uint8_t image_data_Color[32] = {
    0x00, 0x00, 
    0x00, 0x00, 
    0x0f, 0x40, 
    0x1c, 0x64, 
    0x3e, 0xd8, 
    0x27, 0xb0, 
    0x27, 0x7c, 
    0x3c, 0xb8, 
    0x1f, 0xb8, 
    0x1c, 0xf0, 
    0x1d, 0xf0, 
    0x0f, 0x98, 
    0x03, 0x98, 
    0x01, 0xf0, 
    0x00, 0x00, 
    0x00, 0x00
};
const tImage Color = { image_data_Color, 16, 16,
    8 };

