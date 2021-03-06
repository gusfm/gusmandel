#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include "gusmandel.h"

double data[] = { -0.7435669, 0.1314023, 0.0022878 / 2.0 };
// double data[] = { -1.0, 0.0, 1.0 };
// double data[] = { -0.7436447860, 0.1318252536, 0.00029336 / 3 };
// double data[] = { -1.0, 0.3, 0.007 };

int main(int argc, char const *argv[])
{
    gusmandel_t *gm;
    int ret;
    double real = data[0];
    double imag = data[1];
    double radius = data[2];

    if (argc < 5) {
        fprintf(stderr,
                "ERROR: Syntax: %s <image size> <max iter> <anti-aliasing> "
                "<pallete ppm file>\n",
                argv[0]);
        exit(EXIT_FAILURE);
    }

    int image_size = strtol(argv[1], NULL, 10);
    if (image_size <= 0) {
        fprintf(stderr, "ERROR: Invalid image size (one dimension only): %d\n",
                image_size);
        exit(EXIT_FAILURE);
    }
    int max_iter = strtol(argv[2], NULL, 10);
    if (max_iter <= 0) {
        fprintf(stderr, "ERROR: Invalid max iterations value: %d\n", max_iter);
        exit(EXIT_FAILURE);
    }
    int aa = strtol(argv[3], NULL, 10);
    if (aa <= 0) {
        fprintf(stderr, "ERROR: Invalid anti-aliasing value: %d\n", aa);
        exit(EXIT_FAILURE);
    }
    const char *pal = argv[4];

    printf(
        "Starting gusmandel...\n"
        "Image size: %d\n"
        "Max iterations: %d\n"
        "Anti-aliasing: x%d\n"
        "Palette: %s\n"
        "Output file: out.txt\n",
        image_size, max_iter, aa, pal);
    gm = gusmandel_init(real, imag, radius, max_iter, pal, image_size * aa, aa);
    if (gm == NULL)
        exit(EXIT_FAILURE);

    ret = gusmandel_run(gm, 8);
    gusmandel_destroy(gm);
    if (ret < 0)
        exit(EXIT_FAILURE);
    printf("done!\n");

    return 0;
}
