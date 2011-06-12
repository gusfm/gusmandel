#include <stdlib.h>
#include "stdio.h"

#include "rgb.h"
#include "image.h"
#include "palette.h"
#include "mandel.h"

#define SIZE_XY 512
#define MAX_ITER 1024

int main (int argc, char const* argv[])
{
	int ret;
	image_t *image;
	mandel_t *mandel;
	palette_t *pal;
	rgb_t rgb_max_iter = {0,0,0,0};

	image = image_create(SIZE_XY, SIZE_XY);
	//image = image_create(1920, 1080);
	if (!image) {
		printf("image NULL\n");
		return EXIT_FAILURE;
	}

	pal = palette_load_ppm("pal.ppm", MAX_ITER, &rgb_max_iter);
	if (!pal) {
		printf("palette NULL\n");
		return EXIT_FAILURE;
	}

	//mandel = mandel_create(image, -0.7435669, 0.1314023, .0022878/3.0);
	//mandel = mandel_create(image, -1, 0, 2);
	//mandel = mandel_create(image, -0.7436447860, 0.1318252536, .00029336);
	mandel = mandel_create(image, -1, 0.3, 0.007);
	if (!mandel) {
		printf("mandel NULL\n");
		return EXIT_FAILURE;
	}

	mandel_calculate(mandel, pal);

	ret = image_write_file(image, "image.ppm");
	if (ret < 0) {
		printf("could not write image\n");
		return EXIT_FAILURE;
	}

	return 0;
}

