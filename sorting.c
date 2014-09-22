#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "read_write.h"
#include "sorting.h"
#include "sorting_context.h"

#define CHANNELS 3 // we just ignore the alpha channel

#define CMP_FN inline int
#define VAL_FN inline int

#define AVG_VAL avg_val
#define MUL_VAL mul_val
#define MAX_VAL max_val
#define MIN_VAL min_val
#define XOR_VAL orx_val

#define AVG_CMP avg_cmp
#define MUL_CMP mul_cmp
#define MAX_CMP max_cmp
#define MIN_CMP min_cmp
#define XOR_CMP orx_cmp

void sort_run(pixel_t *, const int, compare_fn_t);

void dark_run_processor(pixel_t *, const sort_plan_t *);
void light_run_processor(pixel_t *, const sort_plan_t *);
void default_run_processor(pixel_t *, const sort_plan_t *);

int get_first_dark(const pixel_t *, sort_val_fn_t, const int, const long);
int get_first_non_dark(const pixel_t *, sort_val_fn_t, const int, const long);
int get_first_light(const pixel_t *, sort_val_fn_t, const int, const long);
int get_first_non_light(const pixel_t *, sort_val_fn_t, const int, const long);

static int AVG_VAL(const pixel_t *);
static int MUL_VAL(const pixel_t *);
static int MAX_VAL(const pixel_t *);
static int MIN_VAL(const pixel_t *);
static int XOR_VAL(const pixel_t *);

static int AVG_CMP(const pixel_t *, const pixel_t *);
static int MUL_CMP(const pixel_t *, const pixel_t *);
static int MAX_CMP(const pixel_t *, const pixel_t *);
static int MIN_CMP(const pixel_t *, const pixel_t *);
static int XOR_CMP(const pixel_t *, const pixel_t *);

static pixel_t * create_pixel_list(const struct image * const, const sort_plan_t *);
static void sync_pixels(struct image *, const sort_plan_t *, const pixel_t *);
static sort_plan_t * create_sort_plan(const struct image *, const context_t *, enum orientation_e);
static void destroy_sort_plan(sort_plan_t *);
static void do_sort(pixel_t *, const sort_plan_t *);

void sort(struct image * img, const context_t * ctx)
{
    sort_plan_t *plan_steps;

    if(ROW == ctx->orientation) {
        plan_steps = create_sort_plan(img, ctx, ROW);
    } else if(COLUMN == ctx->orientation) {
        plan_steps = create_sort_plan(img, ctx, COLUMN);
    } else {
        plan_steps = create_sort_plan(img, ctx, ROW);
        plan_steps->next_step_ptr = create_sort_plan(img, ctx, COLUMN);
    }

    for(const sort_plan_t * step = plan_steps; NULL != step; step = step->next_step_ptr) {
        pixel_t * pixels = create_pixel_list(img, step);
        do_sort(pixels, step);
        sync_pixels(img, step, pixels);
    }

    destroy_sort_plan(plan_steps);
}

pixel_t * create_pixel_list(const struct image * const img, const sort_plan_t * plan_ptr)
{
    pixel_t *new_pixels = (pixel_t*)malloc(sizeof(pixel_t) * img->width * img->height);

    if (ROW != plan_ptr->orientation) {
        for (size_t column_index = 0; column_index < img->width; column_index++) {
            for (size_t row_index = 0; row_index < img->height; row_index++) {
                size_t destination_index = (img->height * column_index) + row_index;
                size_t source_index = (img->width * row_index) + column_index;
                pixel_t p = {
                    .r = img->buffer[(source_index * sizeof(pixel_t)) + 0],
                    .g = img->buffer[(source_index * sizeof(pixel_t)) + 1],
                    .b = img->buffer[(source_index * sizeof(pixel_t)) + 2],
                    .a = img->buffer[(source_index * sizeof(pixel_t)) + 3]
                };
                new_pixels[destination_index] = p;
            }
        }
    }

    return new_pixels;
}

void sync_pixels(struct image * img, const sort_plan_t * plan_ptr, const pixel_t * pixels)
{
    if (ROW != plan_ptr->orientation) {
        for (size_t column_index = 0; column_index < img->width; column_index++) {
            for (size_t row_index = 0; row_index < img->height; row_index++) {
                size_t destination_index = (img->width * row_index) + column_index;
                size_t source_index = (img->height * column_index) + row_index;
                img->buffer[(destination_index * sizeof(pixel_t)) + 0] = pixels[source_index].r;
                img->buffer[(destination_index * sizeof(pixel_t)) + 1] = pixels[source_index].g;
                img->buffer[(destination_index * sizeof(pixel_t)) + 2] = pixels[source_index].b;
                img->buffer[(destination_index * sizeof(pixel_t)) + 3] = pixels[source_index].a;
            }
        }
    }
}

void destroy_sort_plan(sort_plan_t * plan_list_ptr)
{
    while(NULL != plan_list_ptr) {
        sort_plan_t * next = plan_list_ptr->next_step_ptr;
        free(plan_list_ptr);
        plan_list_ptr = next;
    }
}

sort_plan_t * create_sort_plan(const struct image * img, const context_t * ctx, enum orientation_e o)
{
    sort_plan_t * plan = (sort_plan_t*)malloc(sizeof(sort_plan_t));
    plan->next_step_ptr = NULL;
    plan->orientation = o;
    plan->context_ptr = ctx;
    plan->run_length = (ROW == o) ? img->width  : img->height;
    plan->run_count     = (ROW == o) ? img->height : img->width;
    switch(ctx->run_type) {
    case DARK:
        plan->run_processor_fn = dark_run_processor;
        break;
    case LIGHT:
        plan->run_processor_fn = light_run_processor;
        break;
    case ALL:
    default:
        plan->run_processor_fn = default_run_processor;
        break;
    }
    switch(ctx->comparison) {
    case AVG:
        plan->compare_fn = AVG_CMP;
        plan->sort_val_fn = AVG_VAL;
        break;
    case MUL:
        plan->compare_fn = MUL_CMP;
        plan->sort_val_fn = MUL_VAL;
        break;
    case MAX:
        plan->compare_fn = MAX_CMP;
        plan->sort_val_fn = MAX_VAL;
        break;
    case MIN:
        plan->compare_fn = MIN_CMP;
        plan->sort_val_fn = MIN_VAL;
        break;
    case XOR:
    default:
        plan->compare_fn = XOR_CMP;
        plan->sort_val_fn = XOR_VAL;
        break;
    }
    return plan;
}

void do_sort(pixel_t * pixels, const sort_plan_t * plan)
{
    for(int run = 0; run < plan->run_count; ++run) {
        (*plan->run_processor_fn)(pixels + (run * plan->run_length), plan);
    }
}

void dark_run_processor(pixel_t * pixels, const sort_plan_t * plan_ptr)
{
    const int length = plan_ptr->run_length, threshold = plan_ptr->context_ptr->threshold;
    pixel_t * cursor = pixels;
    while(length > (cursor - pixels)) {
        pixel_t * start = cursor + get_first_non_dark(cursor, plan_ptr->sort_val_fn, length - (cursor - pixels), threshold);
        pixel_t * end = start + get_first_dark(start, plan_ptr->sort_val_fn, length - (start - pixels), threshold);
        sort_run(start, end - start, plan_ptr->compare_fn);
        cursor = end;
    }
}

void light_run_processor(pixel_t * pixels, const sort_plan_t * plan)
{
    const int length = plan->run_length, threshold = plan->context_ptr->threshold;
    pixel_t * cursor = pixels;
    while(length > (cursor - pixels)) {
        pixel_t * start = cursor + get_first_non_light(cursor, plan->sort_val_fn, length - (cursor - pixels), threshold);
        pixel_t * end = start + get_first_light(start, plan->sort_val_fn, length - (start - pixels), threshold);
        sort_run(start, end - start, plan->compare_fn);
        cursor = end;
    }
}

void default_run_processor(pixel_t * pixels, const sort_plan_t * plan)
{
    sort_run(pixels, plan->run_length, plan->compare_fn);
}

void sort_run(pixel_t * start, const int length, compare_fn_t cmp)
{
    qsort(start, length, sizeof(pixel_t), (int(*)(const void*,const void*))cmp);
}

int get_first_dark(const pixel_t * pixels, sort_val_fn_t v, const int length, const long threshold)
{
    for(int idx = 0; idx < length; ++idx) {
        if(threshold >= (*v)(pixels + idx)) return idx;
    }
    return length;
}

int get_first_non_dark(const pixel_t * pixels, sort_val_fn_t v, const int length, const long threshold)
{
    for(int idx = 0; idx < length; ++idx) {
        if(threshold < (*v)(pixels + idx)) return idx;
    }
    return length;
}

int get_first_light(const pixel_t * pixels, sort_val_fn_t v, const int length, const long threshold)
{
    for(int idx = 0; idx < length; ++idx) {
        if(threshold <= (*v)(pixels + idx)) return idx;
    }
    return length;
}

int get_first_non_light(const pixel_t * pixels, sort_val_fn_t v, const int length, const long threshold)
{
    for(int idx = 0; idx < length; ++idx) {
        if(threshold > (*v)(pixels + idx)) return idx;
    }
    return length;
}

CMP_FN AVG_CMP(const pixel_t * a, const pixel_t * b)
{
    return AVG_VAL(a) - AVG_VAL(b);
}

CMP_FN MUL_CMP(const pixel_t * a, const pixel_t * b)
{
    return MUL_VAL(a) - MUL_VAL(b);
}

CMP_FN MAX_CMP(const pixel_t * a, const pixel_t * b)
{
    return MAX_VAL(a) - MAX_VAL(b);
}

CMP_FN MIN_CMP(const pixel_t * a, const pixel_t * b)
{
    return MIN_VAL(a) - MIN_VAL(b);
}

CMP_FN XOR_CMP(const pixel_t * a, const pixel_t * b)
{
    return XOR_VAL(a) - XOR_VAL(b);
}

VAL_FN AVG_VAL(const pixel_t * a)
{
    int avg = 0;
    for(int c = 0, len = CHANNELS; c < len; ++c) avg += ((unsigned char *)a)[c];
    return avg / CHANNELS;
}

VAL_FN MUL_VAL(const pixel_t * a)
{
    int mul = 1;
    for(int c = 0, len = CHANNELS; c < len; ++c) mul *= ((unsigned char *)a)[c];
    return mul;
}

VAL_FN MAX_VAL(const pixel_t * a)
{
    int max = -1;
    for(int c = 0, len = CHANNELS; c < len; ++c) {
        if(((unsigned char *)a)[c] > max) max = ((unsigned char *)a)[c];
    }
    return max;
}

VAL_FN MIN_VAL(const pixel_t * a)
{
    int min = 256;
    for(int c = 0, len = CHANNELS; c < len; ++c) {
        if(((unsigned char *)a)[c] < min) min = ((unsigned char *)a)[c];
    }
    return min;
}

VAL_FN XOR_VAL(const pixel_t * a)
{
    int orx = ((unsigned char *)a)[0];
    for(int c = 1, len = CHANNELS; c < len; ++c) orx ^= ((unsigned char *)a)[c];
    return orx;
}
