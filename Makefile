CC ?= clang
CFLAGS := -std=c11 -Werror -Wall -Wpedantic -O3

object_files := $(patsubst %.c, %.o, $(wildcard *.c))
target := pixelsort

$(target): $(object_files)
	$(CC) -o $(@) $(CFLAGS) $(^)

clean:
	rm -rf $(object_files) $(target)

.PHONY: clean
.DEFAULT: pixelsort
