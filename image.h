#ifndef __IMAGE_H__
#define __IMAGE_H__

#include "rgb.h"

typedef struct {
	unsigned int width;
	unsigned int height;
	rgb_t **rgb_matrix;
} image_t;


void image_destroy(image_t *image);

image_t *image_create(unsigned int width, unsigned int height);

unsigned int image_get_width(image_t *image);

unsigned int image_get_height(image_t *image);

int image_write_file(image_t *image, const char *file_name);

#endif /* __IMAGE_H__ */

