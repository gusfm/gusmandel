#include <stdlib.h>
#include <stdio.h>

#include "rgb.h"
#include "image.h"
#include "palette.h"
#include "mandel.h"

#define OUT_IMG_XY_SIZE 1024
#define SUPERSAMPLE_XY_MULT 8 /* set to 1 if no supersample */
#define SUPERSAMPLE_IMG_XY_SIZE OUT_IMG_XY_SIZE * SUPERSAMPLE_XY_MULT
#define MAX_ITER 1024

int main (int argc, char const* argv[])
{
	int ret;
	image_t *image;
	image_t *image_downsampled;
	mandel_t *mandel;
	palette_t *pal;
	rgb_t rgb_max_iter = {0,0,0,0};

	image = image_create(SUPERSAMPLE_IMG_XY_SIZE, SUPERSAMPLE_IMG_XY_SIZE);
	if (!image) {
		printf("image NULL\n");
		return EXIT_FAILURE;
	}

	pal = palette_load_ppm("pal.ppm", MAX_ITER, &rgb_max_iter);
	if (!pal) {
		printf("palette NULL\n");
		return EXIT_FAILURE;
	}

	mandel = mandel_create(image, -0.7435669, 0.1314023, .0022878/2.0);
	//mandel = mandel_create(image, -1, 0, 2);
	//mandel = mandel_create(image, -0.7436447860, 0.1318252536, .00029336);
	//mandel = mandel_create(image, -1, 0.3, 0.007);
	if (!mandel) {
		printf("mandel NULL\n");
		return EXIT_FAILURE;
	}

	//mandel_calculate(mandel, pal);
	mandel_calculate_smooth(mandel, pal);

	if (SUPERSAMPLE_XY_MULT > 1) {
		image_downsampled = image_downsample(image, SUPERSAMPLE_XY_MULT);
		if (image_downsampled) {
			ret = image_write_file(image_downsampled, "image_downsample.ppm");
			if (ret < 0) {
				printf("could not write downsampled image\n");
				return EXIT_FAILURE;
			}
		} else {
			printf("could not generate downsampled image\n");
		}
	} else {
		ret = image_write_file(image, "image.ppm");
		if (ret < 0) {
			printf("could not write image\n");
			return EXIT_FAILURE;
		}
	}

	return 0;
}

