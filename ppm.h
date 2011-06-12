#ifndef __PPM_H__
#define __PPM_H__

#include "rgb.h"

int write_ppm(const char *file_name, rgb_t **image, int width, int height);

rgb_t **load_ppm(const char *file_name, unsigned short int *width, unsigned short int *height);

#endif /* __PPM_H__ */

