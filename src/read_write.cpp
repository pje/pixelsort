#include "../include/read_write.h"

#include <iostream>

#include <cstdlib>
#include <cassert>
#include <cstdio>
#include <cstring>

#include "../include/lodepng.h"

using namespace std;

typedef struct Pixel Pixel_t;

typedef struct Image {
	unsigned char *buffer;
	unsigned int width;
	unsigned int height;
	unsigned int components;
} Image_t;

struct Image * read_image(const char * const file) {
	unsigned error;
	FILE * src;
	if(NULL == (src = fopen(file, "rb"))) {
		cout << "unable to open source file: " << file << endl;
	}

	Image_t * img = (Image_t*)malloc(sizeof(Image_t));
	img->height = 0;
	img->width = 0;
	img->components = 4;

  error = lodepng_decode32_file(&(img->buffer), &(img->width), &(img->height), file);
  if(error) printf("error %u: %s\n", error, lodepng_error_text(error));

	// printf("img\n");
	// printf("  width: %d\n", img->width);
	// printf("  height: %d\n", img->height);
	// printf("  components: %d\n", img->components);

	return img;
}

void write_image(Image_t * img, const char * const file) {
	FILE * dest;
	if(NULL == (dest = fopen(file, "wb"))) {
		cout << "unable to open destination file: " << file << endl;
	}

  unsigned error = lodepng_encode32_file(file, img->buffer, img->width, img->height);
  if(error) printf("error %u: %s\n", error, lodepng_error_text(error));
}


int get_width(const struct Image * const img) {
	return img->width;
}

int get_height(const struct Image * const img) {
	return img->height;
}

int get_components(const struct Image * const img) {
	return img->components;
}

const unsigned char * const get_buffer(const struct Image * const img) {
	return img->buffer;
}

void set_buffer(struct Image * img, unsigned char * buffer) {
	free(img->buffer);
	img->buffer = buffer;
}
