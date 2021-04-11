
/*******************************************************************************
* image
* filename: unsaved
* name: Yellow
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



static const uint8_t image_data_Yellow[48] = {
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 
    0x56, 0x93, 0xaa, 
    0x54, 0x92, 0xaa, 
    0x54, 0x92, 0xaa, 
    0x2c, 0x92, 0xaa, 
    0x24, 0x92, 0x3c, 
    0x28, 0x92, 0xa6, 
    0x24, 0x92, 0x94, 
    0x2e, 0xdb, 0x34, 
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00
};
const tImage Yellow = { image_data_Yellow, 24, 16,
    8 };

