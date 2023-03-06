C?=gcc
CXX?=g++
DEBUG=-DDEBUG -g
CFLAGS=-Wall -O3 -std=c++11 $(DEBUG)

default:
	@+make -C build

sample:
	$(CXX) -o draw_bars samples/draw_bars.cxx \
	  -L. -lgif_factory -Isrc \
	  $(CFLAGS)
	$(CXX) -o draw_cube samples/draw_cube.cxx \
	  -L. -lgif_factory -Isrc \
	  $(CFLAGS)
	$(CXX) -o draw_lines samples/draw_lines.cxx \
	  -L. -lgif_factory -Isrc \
	  $(CFLAGS)
	$(CXX) -o draw_triangles samples/draw_triangles.cxx \
	  -L. -lgif_factory -Isrc \
	  $(CFLAGS)

clean:
	cd python && make clean
	@rm -f build/*.o libgif_factory.so
	@rm -f draw_bars draw_cube draw_lines draw_triangles parse_gif
	@echo "Clean!"

