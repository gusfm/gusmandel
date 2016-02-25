#include <stdlib.h>
#include "image.h"
#include "ppm.h"

void image_destroy(image_t *image)
{
    for (int i = 0; i < image->width; ++i)
        free(image->rgb_matrix[i]);
    free(image->rgb_matrix);
    free(image);
}

image_t *image_create(unsigned int width, unsigned int height)
{
    image_t *image;
    image = (image_t *)malloc(sizeof(image_t));
    if (!image)
        return NULL;
    image->width = width;
    image->height = height;
    image->rgb_matrix = (rgb_t **)malloc(sizeof(rgb_t *) * width);
    for (int i = 0; i < width; ++i) {
        image->rgb_matrix[i] = (rgb_t *)malloc(sizeof(rgb_t) * height);
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
    return ppm_write(file_name, image->rgb_matrix, image->width, image->height);
}

static void pixel_average(rgb_t **rgb_matrix, rgb_t *rgb_average,
                          unsigned int x, unsigned int y, unsigned int xy_cnt)
{
    unsigned int r = 0, g = 0, b = 0, x_max, y_max, total_pixels;
    x_max = x + xy_cnt;
    y_max = y + xy_cnt;
    total_pixels = xy_cnt * xy_cnt;
    for (int i = x; i < x_max; i++) {
        for (int j = y; j < y_max; j++) {
            r += rgb_matrix[i][j].red;
            g += rgb_matrix[i][j].green;
            b += rgb_matrix[i][j].blue;
        }
    }
    rgb_average->red = r / total_pixels;
    rgb_average->green = g / total_pixels;
    rgb_average->blue = b / total_pixels;
}

image_t *image_downsample(image_t *img, unsigned int xy_cnt)
{
    int i, i_out, j, j_out;
    image_t *out_img = NULL;

    if (img->width % xy_cnt == 0 && img->height % xy_cnt == 0) {
        out_img = image_create(img->width / xy_cnt, img->height / xy_cnt);
        if (out_img) {
            for (i = 0, i_out = 0; i < img->width; i += xy_cnt, i_out++) {
                for (j = 0, j_out = 0; j < img->height; j += xy_cnt, j_out++) {
                    pixel_average(img->rgb_matrix,
                                  &out_img->rgb_matrix[i_out][j_out], i, j,
                                  xy_cnt);
                }
            }
        }
    }
    return out_img;
}
