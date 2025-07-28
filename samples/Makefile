C?=gcc
CXX?=g++
DEBUG=-DDEBUG -g
CFLAGS=-Wall -O3 -std=c++11 $(DEBUG)

OBJECTS= \
  ../draw_avi24 \
  ../draw_avi8 \
  ../draw_bars \
  ../draw_bmp24 \
  ../draw_bmp8 \
  ../draw_cube \
  ../draw_lines \
  ../draw_projection \
  ../draw_scaled \
  ../draw_texture \
  ../draw_triangles \
  ../simple_texture

default: $(OBJECTS)

../%: %.cxx
	$(CXX) -o $@ $< \
	  -L.. -lkohn3d -Isrc -I../src \
	  $(CFLAGS)

