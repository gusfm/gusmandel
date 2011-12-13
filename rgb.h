#ifndef __RGB_H__
#define __RGB_H__

#include <stdbool.h>

typedef unsigned char byte_t;

typedef struct {
	byte_t red;
	byte_t green;
	byte_t blue;
	byte_t alpha;
} rgb_t;

bool rgb_equals(rgb_t *rgb1, rgb_t *rgb_2);

#endif /* __RGB_H__ */

