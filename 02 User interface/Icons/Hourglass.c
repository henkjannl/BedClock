
/*******************************************************************************
* image
* filename: unsaved
* name: Hourglass
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



static const uint8_t image_data_Hourglass[32] = {
    0x00, 0x00, 
    0x00, 0x00, 
    0x1f, 0xf8, 
    0x10, 0x08, 
    0x0e, 0xb0, 
    0x07, 0xe0, 
    0x03, 0xc0, 
    0x01, 0x80, 
    0x02, 0x40, 
    0x04, 0x40, 
    0x04, 0x20, 
    0x09, 0x90, 
    0x13, 0xd0, 
    0x1f, 0xf8, 
    0x00, 0x00, 
    0x00, 0x00
};
const tImage Hourglass = { image_data_Hourglass, 16, 16,
    8 };

