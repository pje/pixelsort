#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "read_write.h"
#include "sorting_context.h"
#include "sorting.h"

#define ARG_ROW "row"
#define ARG_COLUMN "col"
#define ARG_BOTH "both"

#define ARG_DARK "dark"
#define ARG_LIGHT "light"
#define ARG_ALL "all"

#define ARG_AVG "avg"
#define ARG_MUL "mul"
#define ARG_MAX "max"
#define ARG_MIN "min"
#define ARG_XOR "xor"

void print_usage()
{
    printf("usage: pixelsort [orientation] [method] [comparator] <in.png> <out.png>\n");
    printf("\n");
    printf("orientation:\n");
    printf("  row\n");
    printf("  col\n");
    printf("  both\n");
    printf("\n");
    printf("method:\n");
    printf("  all\n");
    printf("  dark\n");
    printf("  threshold\n");
    printf("\n");
    printf("comparator:\n");
    printf("  avg\n");
    printf("  mul\n");
    printf("  max\n");
    printf("  min\n");
    printf("  xor\n");
    printf("\n");
}

int main(const int argc, const char* argv[])
{
    if(argc < 6) {
        print_usage();
        return 1;
    }

    const char *orientation = argv[1];
    const char *method = argv[2];
    const char *comparator = argv[3];
    const char *source = argv[4];
    const char *destination = argv[5];

    struct image *img = read_image(source);

    pixel_sorting_context_t *ctx = (pixel_sorting_context_t*)malloc(sizeof(pixel_sorting_context_t));
    ctx->orientation = ROW;
    ctx->comparison = XOR;
    ctx->sort_direction = ASC;
    ctx->run_type = ALL;
    ctx->addendum_one = NULL;
    ctx->addendum_two = NULL;

    ctx->sort_direction = ASC;

    if(0 == strcmp(ARG_ROW, orientation)) {
        ctx->orientation = ROW;
    } else if(0 == strcmp(ARG_COLUMN, orientation)) {
        ctx->orientation = COLUMN;
    } else {
        ctx->orientation = BOTH;
    }

    if(0 == strcmp(ARG_DARK, method)) {
        ctx->run_type = DARK;
        ctx->threshold = 45;
    } else if(0 == strcmp(ARG_LIGHT, method)) {
        ctx->run_type = LIGHT;
        ctx->threshold = 210;
    } else {
        ctx->run_type = ALL;
    }

    if(0 == strcmp(ARG_AVG, comparator)) {
        ctx->comparison = AVG;
    } else if(0 == strcmp(ARG_MUL, comparator)) {
        ctx->comparison = MUL;
    } else if(0 == strcmp(ARG_MAX, comparator)) {
        ctx->comparison = MAX;
    } else if(0 == strcmp(ARG_MIN, comparator)) {
        ctx->comparison = MIN;
    } else {
        ctx->comparison = XOR;
    }

    sort(img, ctx);

    write_image(img, destination);
}
