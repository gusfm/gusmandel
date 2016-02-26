#ifndef __PPM_H__
#define __PPM_H__

#include "rgb.h"

int ppm_p3_write(const char *file_name, rgb_t **image, int width, int height);

int ppm_p6_write(const char *file_name, rgb_t **image, int width, int height);

rgb_t **ppm_read(const char *file_name, unsigned int *width,
                 unsigned int *height);

#endif /* __PPM_H__ */

