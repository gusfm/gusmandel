#ifndef __IMAGE_H__
#define __IMAGE_H__

typedef unsigned char byte_t;

typedef struct {
	byte_t red;
	byte_t green;
	byte_t blue;
	byte_t alpha;
} rgb_t;

typedef struct {
	unsigned int size_x;
	unsigned int size_y;
	rgb_t **rgb_matrix;
} image_t;


void image_destroy(image_t *image);

image_t *image_create(unsigned int size_x, unsigned int size_y);

#endif /* __IMAGE_H__ */

