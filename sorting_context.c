#include <stdlib.h>
#include "sorting_context.h"

typedef struct PixelSortingContext {
    enum Orientation_e orientation;
    enum Comparison_e comparison;
    enum SortDirection_e sort_direction;
    enum RunType_e run_type;
    void * addendum_one;
    void * addendum_two;
} PixelSortingContext_t;

struct PixelSortingContext * create_context()
{
    PixelSortingContext_t * ctx = (PixelSortingContext_t*)malloc(sizeof(PixelSortingContext_t));
    ctx->orientation = ROW;
    ctx->comparison = XOR;
    ctx->sort_direction = ASC;
    ctx->run_type = ALL;
    ctx->addendum_one = NULL;
    ctx->addendum_two = NULL;
    return ctx;
}

void destroy_context(struct PixelSortingContext * ctx)
{
    free(ctx);
}

void set_orientation(struct PixelSortingContext * ctx, enum Orientation_e o)
{
    ctx->orientation = o;
}

enum Orientation_e get_orientation(const struct PixelSortingContext * ctx)
{
    return ctx->orientation;
}

void set_run_type(struct PixelSortingContext * ctx, enum RunType_e r)
{
    ctx->run_type = r;
}

enum RunType_e get_run_type(const struct PixelSortingContext * ctx)
{
    return ctx->run_type;
}

void set_comparison(struct PixelSortingContext * ctx, enum Comparison_e c)
{
    ctx->comparison = c;
}

enum Comparison_e get_comparison(const struct PixelSortingContext * ctx)
{
    return ctx->comparison;
}

void set_sort_direction(struct PixelSortingContext * ctx, enum SortDirection_e s)
{
    ctx->sort_direction = s;
}

enum SortDirection_e get_sort_direction(const struct PixelSortingContext * ctx)
{
    return ctx->sort_direction;
}

void set_threshold(struct PixelSortingContext * ctx, /*RunType_e t, */long threshold)
{
    ctx->addendum_one = (void*)threshold;
}

long get_threshold(const struct PixelSortingContext * ctx/*, enum RunType_e t*/)
{
    return (long)ctx->addendum_one;
}
