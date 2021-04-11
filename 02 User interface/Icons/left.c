
/*******************************************************************************
* image
* filename: unsaved
* name: left
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



static const uint8_t image_data_left[24] = {
    0x02, 0x00, 
    0x06, 0x00, 
    0x0e, 0x00, 
    0x1f, 0xf0, 
    0x3f, 0xf0, 
    0x7f, 0xf0, 
    0x7f, 0xf0, 
    0x3f, 0xf0, 
    0x1f, 0xf0, 
    0x0e, 0x00, 
    0x06, 0x00, 
    0x02, 0x00
};
const tImage left = { image_data_left, 12, 12,
    8 };

