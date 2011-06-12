#include <stdlib.h>

#include "image.h"
#include "ppm.h"

void image_destroy_rgb_matrix(rgb_t **rgb_matrix, unsigned int width, unsigned int height)
{
	int i;

	for (i = 0; i < width; i++) {
		if (rgb_matrix[i])
			free(rgb_matrix[i]);
	}
	free(rgb_matrix);
}

rgb_t **image_create_rgb_matrix(unsigned int width, unsigned int height)
{
	int i;
	rgb_t **rgb_matrix;

	rgb_matrix = (rgb_t **)malloc(sizeof(rgb_t *) * width);
	if (!rgb_matrix)
		return NULL;
	for (i = 0; i < width; i++) {
		rgb_matrix[i] = (rgb_t *)malloc(sizeof(rgb_t) * height);
		if (!rgb_matrix[i]) {
			image_destroy_rgb_matrix(rgb_matrix, width, height);
			return NULL;
		}
	}
	return rgb_matrix;
}

void image_destroy(image_t *image)
{
	if (image->rgb_matrix)
		image_destroy_rgb_matrix(image->rgb_matrix, image->width, image->height);
	free(image);
}

image_t *image_create(unsigned int width, unsigned int height)
{
	image_t *image;

	image = (image_t*)malloc(sizeof(image_t));
	if (!image)
		return NULL;

	image->width = width;
	image->height = height;
	image->rgb_matrix = image_create_rgb_matrix(width, height);
	if (!image->rgb_matrix) {
		free(image);
		return NULL;
	}
	return image;
}

unsigned int image_get_width(image_t *image)
{
	return image->width;
}

unsigned int image_get_height(image_t *image)
{
	return image->height;
}

int image_write_file(image_t *image, const char *file_name)
{
	return write_ppm(file_name, image->rgb_matrix, image->width, image->height);
}

