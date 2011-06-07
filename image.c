#include <stdlib.h>

#include "image.h"

void image_destroy_rgb_matrix(rgb_t **rgb_matrix, unsigned int size_x, unsigned int size_y)
{
	int i;

	for (i = 0; i < size_y; i++) {
		if (rgb_matrix[i])
			free(rgb_matrix[i]);
	}
	free(rgb_matrix);
}

rgb_t **image_create_rgb_matrix(unsigned int size_x, unsigned int size_y)
{
	int i;
	rgb_t **rgb_matrix;

	rgb_matrix = (rgb_t **)malloc(sizeof(rgb_t *) * size_y);
	if (!rgb_matrix)
		return NULL;
	for (i = 0; i < size_y; i++) {
		rgb_matrix[i] = (rgb_t *)malloc(sizeof(rgb_t) * size_x);
		if (!rgb_matrix[i]) {
			image_destroy_rgb_matrix(rgb_matrix, size_x, size_y);
			return NULL;
		}
	}
	return rgb_matrix;
}

void image_destroy(image_t *image)
{
	if (image->rgb_matrix)
		image_destroy_rgb_matrix(image->rgb_matrix, image->size_x, image->size_y);
	free(image);
}

image_t *image_create(unsigned int size_x, unsigned int size_y)
{
	image_t *image;

	image = (image_t*)malloc(sizeof(image_t));
	if (!image)
		return NULL;

	image->size_x = size_x;
	image->size_y = size_y;
	image->rgb_matrix = image_create_rgb_matrix(size_x, size_y);
	if (!image->rgb_matrix) {
		free(image);
		return NULL;
	}
	return image;
}

