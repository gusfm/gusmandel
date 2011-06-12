#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ppm.h"

#define MAX_PPM_COLORS 255

int write_ppm(const char *file_name, rgb_t **image, int width, int height)
{
	int x, y;
	FILE *pfile;

	pfile = fopen(file_name, "w");
	if (pfile == NULL)
		return -1;

	fprintf(pfile, "P3\n# gus-mandel-ppm\n%d %d\n%d\n", width, height, MAX_PPM_COLORS);
	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			fprintf(pfile, "%u %u %u ", (unsigned int)image[x][y].red, (unsigned int)image[x][y].green, (unsigned int)image[x][y].blue);
		}
		fprintf(pfile, "\n");
	}

	return 0;
}

rgb_t **load_ppm(const char *file_name, unsigned short int *width, unsigned short int *height)
{
	int i,j;
	char str[70];
	rgb_t **image;
	FILE *pfile;
	unsigned short int r, g, b, maxcolor;

	pfile = fopen(file_name, "r");
	if (pfile == NULL)
		return NULL;

	fgets(str, 70, pfile);
	if (strcmp(str, "P3\n") != 0)
		return NULL;

	fgets(str, 70, pfile);
	fscanf(pfile, "%hu %hu %hu", width, height, &maxcolor);

	// allocate memory for the matrix of Rgb
	image = (rgb_t **)malloc(sizeof(rgb_t *) * (*height));
	if (!image)
		return NULL;

	for (i = 0; i < *height; i++) {
		image[i] = (rgb_t *)malloc(sizeof(rgb_t) * (*width));
	}

	for (i = 0, j = 0; i < *height; j++) {
		fscanf(pfile, "%hu %hu %hu", &r, &g, &b);
		image[i][j].red = (unsigned char)r;
		image[i][j].green = (unsigned char)g;
		image[i][j].blue = (unsigned char)b;
		if (j == *width - 1) {
			i++;
			j = -1;
		}
	}

	return image;
}

