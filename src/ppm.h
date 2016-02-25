#ifndef __PPM_H__
#define __PPM_H__

#include "rgb.h"

int ppm_write(const char *file_name, rgb_t **image, int width, int height);

rgb_t **ppm_read(const char *file_name, unsigned short int *width,
                 unsigned short int *height);

#endif /* __PPM_H__ */

