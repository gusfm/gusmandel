#ifndef __MANDEL_H__
#define __MANDEL_H__

#include "image.h"
#include "palette.h"

typedef struct {
	double real;
	double imag;
	double radius;
	double delta;
	double rmin;
	double imin;
	int max_iter;
	int width;
	int height;
	image_t *img;
	palette_t *pal;
} mandel_t;

mandel_t *mandel_create(image_t *image, palette_t *pal, double real, double imag, double radius);

void mandel_calculate(mandel_t *mandel, palette_t *pal);

void mandel_calculate_smooth(mandel_t *mandel, palette_t *pal);

void mandel_calc_smooth_threaded(mandel_t *mandel);

#endif /* __MANDEL_H__ */

