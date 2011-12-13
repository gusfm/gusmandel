#include <stdlib.h>
#include <math.h>

#include "mandel.h"

static int mandel_iter(double xc, double yc, int max_iter)
{
	double x, y, x0, y0, tmp_x;
	int iter = 0;

	x = x0 = xc;
	y = y0 = yc;

	while (x * x + y * y < (2 * 2) && iter < max_iter) {
		tmp_x = x * x - y * y + x0;
		y = 2 * x * y + y0;
		x = tmp_x;
		iter++;
	}

	return iter;
}

double logZn(double x, double y)
{
	return log(sqrt(x * x + y * y));
}

static double mandel_iter_smooth(double xc, double yc, int max_iter)
{
	double x, y, x0, y0, tmp_x;
	int iter = 0;

	x = x0 = xc;
	y = y0 = yc;

	while (x * x + y * y < (2 * 2) && iter < max_iter) {
		tmp_x = x * x - y * y + x0;
		y = 2 * x * y + y0;
		x = tmp_x;
		iter++;
	}

	//return iter + (log(2 * log(2)) - log(logZn(x, y))) / log(2); // wikki formula
	//return iter + (log(log(2)) - log(logZn(x, y))) / log(2); // article formula
	return iter - (log(logZn(x, y)) / log(2.0)); // site
}

void mandel_calculate(mandel_t *mandel, palette_t *pal)
{
	double delta, rmin, imin, x, y;
	int i, j, width, height;

	width = image_get_width(mandel->img);
	height = image_get_height(mandel->img);
	delta = (2 * mandel->radius) / (width - 1);
	//delta_y = (2 * mandel->radius) / (height - 1);
	rmin = mandel->real - mandel->radius;
	imin = mandel->imag + mandel->radius;

	for (x = rmin, j = 0; j < width; x += delta, j++) {
		for (y = imin, i = 0; i < height; y -= delta, i++) {
			mandel->img->rgb_matrix[j][i] = palette_get_color(pal, mandel_iter(x, y, pal->max_iter));
		}
	}
}

void mandel_calculate_smooth(mandel_t *mandel, palette_t *pal)
{
	double delta, rmin, imin, x, y;
	int i, j, width, height;

	width = image_get_width(mandel->img);
	height = image_get_height(mandel->img);
	delta = (2 * mandel->radius) / (width - 1);
	//delta_y = (2 * mandel->radius) / (height - 1);
	rmin = mandel->real - mandel->radius;
	imin = mandel->imag + mandel->radius;

	for (x = rmin, j = 0; j < width; x += delta, j++) {
		for (y = imin, i = 0; i < height; y -= delta, i++) {
			palette_get_color_smooth(pal, &mandel->img->rgb_matrix[j][i], mandel_iter_smooth(x, y, pal->max_iter));
		}
	}
}

mandel_t *mandel_create(image_t *image, double real, double imag, double radius)
{
	mandel_t *mandel;

	mandel = (mandel_t*)malloc(sizeof(mandel_t));
	if (!mandel)
		return NULL;

	mandel->real = real;
	mandel->imag = imag;
	mandel->radius = radius;
	mandel->img = image;

	return mandel;
}
