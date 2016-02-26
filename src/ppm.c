#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ppm.h"

#define MAX_PPM_COLORS 255

static void ppm_p3_write_subpixel(FILE *pfile, byte_t byte, int *char_count)
{
    *char_count += fprintf(pfile, "%hhu ", byte);
    if (*char_count > 67) {
        fseek(pfile, -1, SEEK_CUR);
        fprintf(pfile, "\n");
        *char_count = 0;
    }
}

int ppm_p3_write(const char *file_name, rgb_t **image, int width, int height)
{
    int char_count = 0;
    FILE *pfile = fopen(file_name, "w");
    if (pfile == NULL)
        return -1;

    fprintf(pfile, "P3\n# gus-mandel-ppm\n%d %d\n%d\n", width, height,
            MAX_PPM_COLORS);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            ppm_p3_write_subpixel(pfile, image[x][y].red, &char_count);
            ppm_p3_write_subpixel(pfile, image[x][y].green, &char_count);
            ppm_p3_write_subpixel(pfile, image[x][y].blue, &char_count);
        }
    }
    fclose(pfile);
    return 0;
}

int ppm_p6_write(const char *file_name, rgb_t **image, int width, int height)
{
    FILE *pfile = fopen(file_name, "wb");
    if (pfile == NULL)
        return -1;

    fprintf(pfile, "P6\n# gus-mandel-ppm\n%d %d\n%d\n", width, height,
            MAX_PPM_COLORS);
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            fwrite(&image[x][y].red, 1, 1, pfile);
            fwrite(&image[x][y].green, 1, 1, pfile);
            fwrite(&image[x][y].blue, 1, 1, pfile);
        }
    }
    fclose(pfile);
    return 0;
}

rgb_t **ppm_read(const char *file_name, unsigned short int *width,
                 unsigned short int *height)
{
    int i, j;
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
    fclose(pfile);

    return image;
}

