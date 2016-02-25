#ifndef _GUSMANDEL_H_
#define _GUSMANDEL_H_

#include "image.h"
#include "mandel.h"
#include "palette.h"

typedef struct {
    image_t *image;
    mandel_t *mandel;
    palette_t *pal;
    int supersample_mult;
} gusmandel_t;

gusmandel_t *gusmandel_init(double real, double imag, double radius,
                            int max_iter, const char *pallete_file,
                            int image_size, int supersample_mult);

int gusmandel_run(gusmandel_t *gm, int num_threads);

void gusmandel_destroy(gusmandel_t *gm);

#endif /* _GUSMANDEL_H_ */
