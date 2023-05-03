C?=gcc
CXX?=g++
DEBUG=-DDEBUG -g
CFLAGS=-Wall -O3 -std=c++11 $(DEBUG)

default:
	@+make -C build

tools:
	@+make -C build tools

#.PHONY: samples
samples: default
	@+make -C samples

clean:
	cd python && make clean
	@rm -f build/*.o libkohn3d.so
	@rm -f parse_bmp parse_gif
	@rm -f draw_bars draw_cube draw_lines draw_texture draw_triangles
	@rm -f draw_bmp8 draw_bmp24 draw_projection draw_scaled
	@rm -f draw_avi8 draw_avi24
	@echo "Clean!"

