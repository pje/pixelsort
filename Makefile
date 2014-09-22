CC ?= clang
CFLAGS := -std=c11 -Werror -Wall -Wpedantic -O3

object_files := $(patsubst src/%.c, src/%.o, $(wildcard src/*.c))

bin/pixelsort: $(object_files)
	$(CC) -o $(@) $(CFLAGS) $(^)

clean:
	rm -rf $(object_files)
	rm -rf bin/*

.PHONY: clean
.DEFAULT: bin/pixelsort
