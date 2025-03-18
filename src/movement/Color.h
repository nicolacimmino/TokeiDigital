#ifndef __COLOR_H__
#define __COLOR_H__

#include <stdint.h>

// Mostrly based on bits and bobs from:
//  https://stackoverflow.com/questions/3018313/algorithm-to-convert-rgb-to-hsv-and-hsv-to-rgb-in-range-0-255-for-both

namespace Color
{
    typedef struct RGB
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    } RGB;

    typedef struct HSV
    {
        uint8_t h;
        uint8_t s;
        uint8_t v;
    } HSV;

    RGB toRGB(HSV hsv);
    HSV toHSV(RGB rgb);
}

#endif