#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <stdio.h>

#include "mandel.h"

/* Threads shared memory */
static int work_cnt;
static pthread_mutex_t work_mutex;

static int mandel_iter(double xc, double yc, int max_iter)
{
	double x, y, x0, y0, tmp_x;
	int iter = 0;

	x = x0 = xc;
	y = y0 = yc;

	while (x * x + y * y < (2 * 2) && iter < max_iter) {
		tmp_x = x * x - y * y + x0;
		y = 2 * x * y + y0;
		x = tmp_x;
		iter++;
	}

	return iter;
}

#if 0
double logZn(double x, double y)
{
	return log(sqrt(x * x + y * y));
}

static double mandel_iter_smooth(double xc, double yc, int max_iter)
{
	double x, y, x0, y0, tmp_x;
	int iter = 0;

	x = x0 = xc;
	y = y0 = yc;

	while (x * x + y * y < (2 * 2) && iter < max_iter) {
		tmp_x = x * x - y * y + x0;
		y = 2 * x * y + y0;
		x = tmp_x;
		iter++;
	}

	//return iter + (log(2 * log(2)) - log(logZn(x, y))) / log(2); // wikki formula
	//return iter + (log(log(2)) - log(logZn(x, y))) / log(2); // article formula
	return iter - (log(logZn(x, y)) / log(2.0)); // site
}
#else
static double mandel_iter_smooth(double xc, double yc, int max_iter)
{
	double x, y, x0, y0, x2, y2;
	int iter = 0;

	x = x0 = xc;
	y = y0 = yc;

loop:
	x2 = x * x;
	y2 = y * y;
	if (x2 + y2 < 4 && iter < max_iter) {
		y = 2 * x * y + y0;
		x = x2 - y2 + x0;
		iter++;
		goto loop;
	}

	return iter - (logf(logf(sqrtf(x2 + y2))) / logf(2.0)); // site
}
#endif
void mandel_calculate(mandel_t *mandel, palette_t *pal)
{
	double delta, rmin, imin, x, y;
	int i, j, width, height;

	width = image_get_width(mandel->img);
	height = image_get_height(mandel->img);
	delta = (2 * mandel->radius) / (width - 1);
	//delta_y = (2 * mandel->radius) / (height - 1);
	rmin = mandel->real - mandel->radius;
	imin = mandel->imag + mandel->radius;

	for (x = rmin, j = 0; j < width; x += delta, j++) {
		for (y = imin, i = 0; i < height; y -= delta, i++) {
			mandel->img->rgb_matrix[j][i] = palette_get_color(pal, mandel_iter(x, y, pal->max_iter));
		}
	}
}

void mandel_calculate_smooth(mandel_t *mandel, palette_t *pal)
{
	double delta, rmin, imin, x, y;
	int i, j, width, height;

	width = image_get_width(mandel->img);
	height = image_get_height(mandel->img);
	delta = (2 * mandel->radius) / (width - 1);
	//delta_y = (2 * mandel->radius) / (height - 1);
	rmin = mandel->real - mandel->radius;
	imin = mandel->imag + mandel->radius;

	for (x = rmin, j = 0; j < width; x += delta, j++) {
		for (y = imin, i = 0; i < height; y -= delta, i++) {
			palette_get_color_smooth(pal, &mandel->img->rgb_matrix[j][i], mandel_iter_smooth(x, y, pal->max_iter));
		}
	}
}

int mandel_get_work(void)
{
	int work_id;

	pthread_mutex_lock(&work_mutex);
	work_id = work_cnt;
	work_cnt++;
	pthread_mutex_unlock(&work_mutex);

	//printf("# %s: work_id = %d\n\r", __func__, work_id);
	return work_id;
}

#define NUM_THREADS 8
#define NUM_JOBS_PER_THREADS 40

void mandel_calc_range(mandel_t *mandel, double delta, int j, int width)
{
	int i;
	double x, y;

	x = mandel->rmin + delta * j;
	while (j < width) {
		for (y = mandel->imin, i = 0; i < mandel->height; y -= delta, i++) {
			palette_get_color_smooth(mandel->pal, &mandel->img->rgb_matrix[j][i], mandel_iter_smooth(x, y, mandel->pal->max_iter));
		}
		x += delta;
		j++;
	}
}

void *mandel_thread_run(void *arg)
{
	mandel_t *mandel;
	int work_id, work_width_tmp, max_jobs;
	int x_work_begin, x_work_end, work_size;

	mandel = (mandel_t *)arg;

	work_size = mandel->width / (NUM_THREADS * NUM_JOBS_PER_THREADS);
	max_jobs = mandel->width / work_size;
	/* Process work until no work left */
	while ((work_id = mandel_get_work()) <= max_jobs) {
		x_work_begin = work_id * work_size;
		work_width_tmp = x_work_begin + work_size;
		x_work_end = work_width_tmp < mandel->width ? work_width_tmp : mandel->width;
		mandel_calc_range(mandel, mandel->delta, x_work_begin, x_work_end);
	}

	pthread_exit(0);
}

void mandel_calc_smooth_threaded(mandel_t *mandel)
{
	int i;
	pthread_t threads[NUM_THREADS];

	work_cnt = 0;
	pthread_mutex_init(&work_mutex, NULL);

	mandel->delta = (2 * mandel->radius) / (mandel->width - 1);
	mandel->rmin = mandel->real - mandel->radius;
	mandel->imin = mandel->imag + mandel->radius;


	/* Create threads */
	for (i = 0; i < NUM_THREADS; i++) {
		pthread_create(&threads[i], NULL, mandel_thread_run, (void *)mandel);
		//printf("# %s: thread %d created\n\r", __func__, i);
	}
	
	/* Wait threads to finish all work */
	for (i = 0; i < NUM_THREADS; i++) {
		pthread_join(threads[i], NULL);
		//printf("# %s: thread %d finished\n\r", __func__, i);
	}
}

mandel_t *mandel_create(image_t *image, palette_t *pal, double real, double imag, double radius)
{
	mandel_t *mandel;

	mandel = (mandel_t*)malloc(sizeof(mandel_t));
	if (!mandel)
		return NULL;

	mandel->real = real;
	mandel->imag = imag;
	mandel->radius = radius;
	mandel->img = image;
	mandel->pal = pal;
	mandel->width = image_get_width(mandel->img);
	mandel->height = image_get_height(mandel->img);

	return mandel;
}
