#ifndef _READ_WRITE_H
#define _READ_WRITE_H

typedef struct pixel pixel_t;

typedef struct image {
    unsigned char *buffer;
    unsigned int width;
    unsigned int height;
    unsigned int channels;
} image_t;

struct image * read_image(const char * const);
void write_image(struct image *, const char * const);

#endif
