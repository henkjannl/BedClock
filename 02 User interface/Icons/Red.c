
/*******************************************************************************
* image
* filename: unsaved
* name: Red
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



static const uint8_t image_data_Red[48] = {
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 
    0x01, 0x9b, 0x80, 
    0x01, 0x52, 0x80, 
    0x01, 0x52, 0x80, 
    0x01, 0x5a, 0x80, 
    0x01, 0xd2, 0x80, 
    0x01, 0x52, 0x80, 
    0x01, 0x52, 0x80, 
    0x01, 0x5b, 0x00, 
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00, 
    0x00, 0x00, 0x00
};
const tImage Red = { image_data_Red, 24, 16,
    8 };

