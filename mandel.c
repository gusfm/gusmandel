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

