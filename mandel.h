#ifndef __MANDEL_H__
#define __MANDEL_H__

#include "image.h"

typedef struct {
	double real;
	double imag;
	double radius;
	int max_iter;
	image_t *img;
} mandel_t;

#endif /* __MANDEL_H__ */

