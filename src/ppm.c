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

static void ppm_rgb_destroy(rgb_t **image, int width, int height)
{
    for (int i = 0; i < height; i++) {
        free(image[i]);
    }
    free(image);
}

static rgb_t **ppm_p3_read(FILE *pfile, int width, int height)
{
    int x, y, ret;
    /* Allocate memory for the matrix of Rgb. */
    rgb_t **image = (rgb_t **)malloc(sizeof(rgb_t *) * (height));
    for (y = 0; y < height; y++) {
        image[y] = (rgb_t *)malloc(sizeof(rgb_t) * (width));
    }
    for (y = 0, x = 0; y < height; ++x) {
        ret = fscanf(pfile, "%hhu %hhu %hhu", &image[y][x].red,
                     &image[y][x].green, &image[y][x].blue);
        if (ret != 3)
            break;
        if (x == width - 1) {
            ++y;
            x = -1;
        }
    }
    if (y != height || x != 0) {
        ppm_rgb_destroy(image, width, height);
        image = NULL;
    }
    return image;
}

static rgb_t **ppm_p6_read(FILE *pfile, int width, int height)
{
    int x, y, ret;
    /* Allocate memory for the matrix of Rgb. */
    rgb_t **image = (rgb_t **)malloc(sizeof(rgb_t *) * (height));
    for (y = 0; y < height; y++) {
        image[y] = (rgb_t *)malloc(sizeof(rgb_t) * (width));
    }
    for (y = 0, x = 0; y < height; ++x) {
        ret = fread(&image[y][x].red, 1, 1, pfile);
        if (ret != 1)
            break;
        ret = fread(&image[y][x].green, 1, 1, pfile);
        if (ret != 1)
            break;
        ret = fread(&image[y][x].blue, 1, 1, pfile);
        if (ret != 1)
            break;
        if (x == width - 1) {
            ++y;
            x = -1;
        }
    }
    if (y != height || x != 0) {
        ppm_rgb_destroy(image, width, height);
        image = NULL;
    }
    return image;
}

static int ppm_read_header(FILE *pfile, unsigned int *width,
                           unsigned int *height, char *type)
{
    char str[70], *s;
    int maxcolor, ret;
    /* Read type. */
    ret = fscanf(pfile, "P%c\n", type);
    if (ret != 1) {
        return -1;
    }
    /* Read comment line and ignore. */
    s = fgets(str, 70, pfile);
    if (s == NULL) {
        return -1;
    }
    /* Read width and height. */
    ret = fscanf(pfile, "%u %u", width, height);
    if (ret != 2) {
        return -1;
    }
    /* Read maxcolor. */
    ret = fscanf(pfile, "%u\n", &maxcolor);
    if (ret != 1) {
        return -1;
    }
    return 0;
}

rgb_t **ppm_read(const char *file_name, unsigned int *width,
                 unsigned int *height)
{
    rgb_t **image;
    char type;
    FILE *pfile;
    int ret;

    pfile = fopen(file_name, "rb");
    if (pfile == NULL)
        return NULL;

    ret = ppm_read_header(pfile, width, height, &type);
    if (ret < 0) {
        fclose(pfile);
        return NULL;
    }
    if (type == '3') {
        image = ppm_p3_read(pfile, *width, *height);
    } else if (type == '6') {
        image = ppm_p6_read(pfile, *width, *height);
    } else {
        image = NULL;
    }
    fclose(pfile);
    return image;
}
