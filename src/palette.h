#ifndef __PALETTE_H__
#define __PALETTE_H__

#include "image.h"

typedef struct {
    unsigned int size;
    unsigned int max_iter;
    rgb_t *color_array;
    rgb_t *rgb_max_iter;
} palette_t;

/**
 * Create a random Palette
 *
 * FIXME: This implementation of random palettes isn't good since the colors
 * change too much. A good aproach is to implement some kind of gradient
 * palette generator.
 */
palette_t *palette_generate(int seed, int size, int max_iter,
                            rgb_t *rgb_max_iter);

/**
 * Create a Palette from a ppm file. The file should be a N x 1 image.
 */
palette_t *palette_load_ppm(const char *file_name, int max_iter,
                            rgb_t *rgb_max_iter);

void palette_destroy(palette_t *palette);

/**
 * Get color for iteration.
 */
rgb_t palette_get_color(palette_t *palette, int iter);

void palette_get_color_smooth(palette_t *palette, rgb_t *result_color,
                              double iter);

#endif /* __PALETTE_H__ */

