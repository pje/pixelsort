#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "read_write.h"
#include "lodepng.h"

struct image * read_image(const char * const file)
{
    unsigned error;
    FILE * src;
    if(NULL == (src = fopen(file, "rb"))) {
        printf("unable to open source file: %s\n", file);
    }

    image_t * img = (image_t*)malloc(sizeof(image_t));
    img->height = 0;
    img->width = 0;
    img->channels = 4;

    error = lodepng_decode32_file(&(img->buffer), &(img->width), &(img->height), file);
    if(error) printf("error %u: %s\n", error, lodepng_error_text(error));

    return img;
}

void write_image(image_t * img, const char * const file)
{
    FILE * dest;
    if(NULL == (dest = fopen(file, "wb"))) {
        printf("unable to open destination file: %s\n", file);
    }

    unsigned error = lodepng_encode32_file(file, img->buffer, img->width, img->height);
    if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
}
