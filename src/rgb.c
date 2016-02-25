#include "rgb.h"

bool rgb_equals(rgb_t *rgb1, rgb_t *rgb2)
{
    return (rgb1->red == rgb2->red && rgb1->green == rgb2->green
            && rgb1->blue == rgb2->blue);
}

