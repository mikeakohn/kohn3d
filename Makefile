C?=gcc
CXX?=g++
DEBUG=-DDEBUG -g
CFLAGS=-Wall -O3 -std=c++11 $(DEBUG)

default:
	@+make -C build

sample:
	$(CXX) -o draw_bars samples/draw_bars.cxx \
	  -L. -lkohn3d -Isrc \
	  $(CFLAGS)
	$(CXX) -o draw_cube samples/draw_cube.cxx \
	  -L. -lkohn3d -Isrc \
	  $(CFLAGS)
	$(CXX) -o draw_lines samples/draw_lines.cxx \
	  -L. -lkohn3d -Isrc \
	  $(CFLAGS)
	$(CXX) -o draw_triangles samples/draw_triangles.cxx \
	  -L. -lkohn3d -Isrc \
	  $(CFLAGS)
	$(CXX) -o draw_bmp8 samples/draw_bmp8.cxx \
	  -L. -lkohn3d -Isrc \
	  $(CFLAGS)
	$(CXX) -o draw_bmp24 samples/draw_bmp24.cxx \
	  -L. -lkohn3d -Isrc \
	  $(CFLAGS)

clean:
	cd python && make clean
	@rm -f build/*.o libgif_factory.so
	@rm -f parse_bmp parse_gif
	@rm -f draw_bars draw_cube draw_lines draw_triangles
	@rm -f draw_bmp8 draw_bmp24
	@echo "Clean!"

