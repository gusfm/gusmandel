#include <stdlib.h>

#include "palette.h"

#define MAX_COLORS 255

static palette_t *palette_allocate(int max_iter, rgb_t *rgb_max_iter)
{
	palette_t *pal;

	pal = (palette_t *)malloc(sizeof(palette_t));
	if (!pal)
		return NULL;

    pal->max_iter = max_iter;
	pal->rgb_max_iter = rgb_max_iter;

	return pal;
}

palette_t *palette_generate(int seed, int size, int max_iter, rgb_t *rgb_max_iter)
{
	int i;
	palette_t *pal;
	
	pal = palette_allocate(max_iter, rgb_max_iter);
	if (!pal)
		return NULL;

    pal->size = size;
    pal->color_array = (rgb_t *)malloc(sizeof(rgb_t) * size);
	if (!pal->color_array) {
		free(pal);
		return NULL;
	}

	srand(seed);
	for (i = 0; i < size; i++) {
		pal->color_array[i].red = rand() % MAX_COLORS;
		pal->color_array[i].green = rand() % MAX_COLORS;
		pal->color_array[i].blue = rand() % MAX_COLORS;
	}

	return pal;
}

palette_t *palette_load_ppm(const char *file_name, int max_iter, rgb_t *rgb_max_iter)
{
	int height, i;
	rgb_t **img;
	palette_t *pal;
	
	pal = palette_allocate(max_iter, rgb_max_iter);
	if (!pal)
		return NULL;

	// load image rgb matrix
	img = loadPPM(file_name, &pal->size, &height);
	if (!pal->color_array) {
		free(pal);
		return NULL;
	}

	// copy the first matrix pointer to be the color_array
	pal->color_array = img[0];

	// free the rest
	for (i = 1; i < height; i++)
		free(img[i]);

	free(img);

	return pal;
}

void palette_destroy(palette_t *palette)
{
	free(palette->color_array);
	free(palette);
}

void inline get_color(palette_t *palette, int iter)
{
    if (iter >= palette->max_iter)
        return palette->rgb_max_iter;
    return palette->color_array[iter % palette->size];
}

