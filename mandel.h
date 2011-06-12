#ifndef __MANDEL_H__
#define __MANDEL_H__

#include "image.h"
#include "palette.h"

typedef struct {
	double real;
	double imag;
	double radius;
	int max_iter;
	image_t *img;
} mandel_t;

mandel_t *mandel_create(image_t *image, double real, double imag, double radius);

void mandel_calculate(mandel_t *mandel, palette_t *pal);

#endif /* __MANDEL_H__ */

