#ifndef _SORTING_CONTEXT_H
#define _SORTING_CONTEXT_H

enum orientation_e { COLUMN, ROW, BOTH };
enum run_type_e { ALL, DARK, LIGHT };
enum comparison_e { AVG, MUL, MAX, MIN, XOR };

typedef struct pixel_sorting_context {
    enum orientation_e orientation;
    enum comparison_e comparison;
    enum run_type_e run_type;
    long threshold;
} pixel_sorting_context_t;

#endif
