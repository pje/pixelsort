#ifndef _SORTING_H
#define _SORTING_H

#include "read_write.h"
#include "sorting_context.h"

struct sort_plan;
typedef struct pixel_sorting_context context_t;

typedef struct pixel {
    const unsigned char r;
    const unsigned char g;
    const unsigned char b;
    const unsigned char a;
} pixel_t;

typedef int(*sort_val_fn_t)(const pixel_t *);
typedef int(*compare_fn_t)(const pixel_t *, const pixel_t *);
typedef void(*run_processor_fn_t)(pixel_t *, const struct sort_plan *);

typedef struct sort_plan {
    int run_count;
    int run_length;

    enum orientation_e orientation;

    run_processor_fn_t run_processor_fn;
    sort_val_fn_t sort_val_fn;
    compare_fn_t compare_fn;

    const context_t * context_ptr;
    struct sort_plan * next_step_ptr;
} sort_plan_t;

void sort(struct image *, const struct pixel_sorting_context *);

#endif
