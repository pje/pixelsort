CC       := g++
LD       := g++
CXXFLAGS := -std=c++11 -Werror -Wall -O3

SRC_DIR := src
INCLUDE := include
BIN     := bin

OBJECTS := $(SRC_DIR)/sorting.o $(SRC_DIR)/sorting_context.o $(SRC_DIR)/lodepng.o $(SRC_DIR)/read_write.o $(SRC_DIR)/main.o

all: bin/pixelsort

bin/pixelsort: $(OBJECTS)
	$(LD) -o $(@) $(CXXFLAGS) $(^)

clean:
	rm -rf src/*.o
	rm -rf bin/*

bin/example_png_info: $(SRC_DIR)/lodepng.o $(SRC_DIR)/example_png_info.o
	$(LD) -o $(@) $(CXXFLAGS) $(^)
