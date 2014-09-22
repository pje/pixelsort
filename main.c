#include <getopt.h>
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

void print_usage(int argc, char **argv)
{
    printf("usage: %s [options] <in.png> <out.png>\n", argv[0]);
    printf("\n");
    printf("options:\n");
    printf("  -o, --orientation     [row|column|both] (row)\n");
    printf("  -m, --method          [light|dark|all] (light)\n");
    printf("  -c, --comparator      [avg|mul|max|min|xor] (min)\n");
    printf("  -t, --threshold       [0..255] (210)\n");
    printf("\n");
}

int main (int argc, char **argv)
{
    char* orientation_arg_value = ARG_ROW;
    char* method_arg_value = ARG_LIGHT;
    char* comparator_arg_value = ARG_MIN;
    long threshold_arg_value = -1;

    if (argc < 3) {
        print_usage(argc, argv);
        return 1;
    }

    int c;
    while (1)
    {
        static struct option long_options[] =
        {
          { "orientation",  required_argument, 0, 'o'},
          { "method",       required_argument, 0, 'm'},
          { "comparator",   required_argument, 0, 'c'},
          { "threshold",    required_argument, 0, 't'},
          { 0, 0, 0, 0 }
        };

        int option_index = 0;

        c = getopt_long(argc, argv, "a:f:", long_options, &option_index);

        if (c == -1) break;

        switch (c)
        {
            case 'o':
                orientation_arg_value = optarg;
                break;
            case 'm':
                method_arg_value = optarg;
                break;
            case 'c':
                comparator_arg_value = optarg;
                break;
            case 't':
                threshold_arg_value = strtol(optarg, NULL, 10);
                break;
            case '?':
                break;
            default:
                abort();
        }
    }

    char *source = argv[argc - 2];
    char *destination = argv[argc - 1];

    struct image *img = read_image(source);

    pixel_sorting_context_t *ctx = (pixel_sorting_context_t*)malloc(sizeof(pixel_sorting_context_t));

    if (0 == strcmp(ARG_ROW, orientation_arg_value)) {
        ctx->orientation = ROW;
    } else if (0 == strcmp(ARG_COLUMN, orientation_arg_value)) {
        ctx->orientation = COLUMN;
    } else if (0 == strcmp(ARG_BOTH, orientation_arg_value)) {
        ctx->orientation = BOTH;
    } else {
        printf("invalid value for orientation argument; must be one of [row|column|both]");
        return 1;
    }

    if (0 == strcmp(ARG_DARK, method_arg_value)) {
        ctx->run_type = DARK;
        if (threshold_arg_value == -1) {
           ctx->threshold = 45;
        } else {
            ctx->threshold = threshold_arg_value;
        }
    } else if (0 == strcmp(ARG_LIGHT, method_arg_value)) {
        ctx->run_type = LIGHT;
        if (threshold_arg_value == -1) {
           ctx->threshold = 210;
        } else {
            ctx->threshold = threshold_arg_value;
        }
    } else if (0 == strcmp(ARG_ALL, method_arg_value)) {
        ctx->run_type = ALL;
    } else {
        printf("invalid value for method argument; must be one of [all|dark|light]");
        return 1;
    }

    if (0 == strcmp(ARG_AVG, comparator_arg_value)) {
        ctx->comparison = AVG;
    } else if (0 == strcmp(ARG_MUL, comparator_arg_value)) {
        ctx->comparison = MUL;
    } else if (0 == strcmp(ARG_MAX, comparator_arg_value)) {
        ctx->comparison = MAX;
    } else if (0 == strcmp(ARG_MIN, comparator_arg_value)) {
        ctx->comparison = MIN;
    } else if (0 == strcmp(ARG_XOR, comparator_arg_value)) {
        ctx->comparison = XOR;
    } else {
        printf("invalid value for comparator argument; must be one of [avg|mul|max|min|xor]");
        return 1;
    }

    sort(img, ctx);
    write_image(img, destination);
}
