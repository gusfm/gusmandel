#include <stdlib.h>
#include <stdio.h>
#include "gusmandel.h"

rgb_t rgb_max_iter = { 0, 0, 0, 0 };

gusmandel_t *gusmandel_init(double real, double imag, double radius,
                            int max_iter, const char *pallete_file,
                            int image_size, int supersample_mult)
{
    gusmandel_t *gm = calloc(1, sizeof(gusmandel_t));
    gm->supersample_mult = supersample_mult;
    gm->image = image_create(image_size, image_size);
    if (gm->image == NULL) {
        fprintf(stderr, "image is null\n");
        gusmandel_destroy(gm);
        return NULL;
    }
    gm->pal = palette_load_ppm(pallete_file, max_iter, &rgb_max_iter);
    if (gm->pal == NULL) {
        fprintf(stderr, "palette is null\n");
        gusmandel_destroy(gm);
        return NULL;
    }
    gm->mandel = mandel_create(gm->image, gm->pal, real, imag, radius);
    if (gm->mandel == NULL) {
        fprintf(stderr, "mandel NULL\n");
        gusmandel_destroy(gm);
        return NULL;
    }
    return gm;
}

static int gusmandel_write_image(gusmandel_t *gm)
{
    int ret = 0;
    image_t *image, *image_downsampled = NULL;
    if (gm->supersample_mult > 1) {
        image_downsampled = image_downsample(gm->image, gm->supersample_mult);
        if (image_downsampled == NULL) {
            printf("could not generate downsampled image\n");
            return -1;
        }
        image = image_downsampled;
    } else {
        image = gm->image;
    }
    ret = image_write_file(image, "out.ppm");
    if (ret < 0) {
        printf("could not write image\n");
        ret = -1;
    }
    if (image_downsampled != NULL)
        image_destroy(image_downsampled);
    return ret;
}

int gusmandel_run(gusmandel_t *gm, int num_threads)
{
    // mandel_calculate(mandel, pal);
    // mandel_calculate_smooth(mandel, pal);
    mandel_calc_smooth_threaded(gm->mandel);
    return gusmandel_write_image(gm);
}

void gusmandel_destroy(gusmandel_t *gm)
{
    mandel_destroy(gm->mandel);
    palette_destroy(gm->pal);
    image_destroy(gm->image);
    free(gm);
}
