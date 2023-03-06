/*

  GIF Factory - GIF drawing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#ifndef KOHN3D_H
#define KOHN3D_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "GifCompressor.h"

class Kohn3D
{
public:
  Kohn3D(int width, int height);
  ~Kohn3D();

  int create(const char *filename);
  void finish();

  // The following methods must be called before init_end();
  int add_color(int value);
  void set_color(int index, int value);
  void set_bg_color_index(uint8_t value) { gif.set_bg_color_index(value); }

  void set_transparent_color_index(uint8_t value)
  {
    gif.set_transparent_color_index(value);
  }

  // Delay value is 100ths of a second.
  void set_delay(int value) { gif.set_delay(value); }

  static const int LOOP_INFINITE = 0;
  void set_loop_count(int value) { gif.set_loop_count(value); }

  void init_end();
  uint8_t *get_picture() { return picture; }
  void clear();

  void draw_pixel(int x, int y, int color_index)
  {
    if (x < 0 || x >= width) { return; }
    if (y < 0 || y >= height) { return; }

    picture[(y * width) + x] = color_index;
  }

  void draw_pixel(int x, int y, int color_index, int z)
  {
    if (x < 0 || x >= width) { return; }
    if (y < 0 || y >= height) { return; }

    int pixel = (y * width) + x;

    if (z < z_buffer[pixel]) { return; }

    picture[pixel] = color_index;
    z_buffer[pixel] = z;
  }

  void draw_line(int x0, int y0, int x1, int y1, int color_index);
  void draw_line(int x0, int y0, int z0, int x1, int y1, int z1, int color_index);
  void draw_rect(int x0, int y0, int x1, int y1, int color_index);
  void draw_rect(int x0, int y0, int x1, int y1, int color_index, int z);

  struct Triangle
  {
    int x0, y0, z0;
    int x1, y1, z1;
    int x2, y2, z2;
  };

  struct Rotation
  {
    Rotation() : rx { 0 }, ry { 0 }, rz { 0 } { }
    float rx, ry, rz;
  };

  void load_triangle(Triangle &triangle, const int coords[]);
  void load_rotation(Rotation &rotation, const float values[]);

  void draw_triangle(const Triangle &triangle, int x, int y, int color_index);
  void draw_triangle(const Triangle &triangle, int x, int y, int z, int color_index);

  void draw_triangle(
    const Triangle &triangle,
    const Rotation &rotation,
    int x, int y, int z,
    int color_index);

  void write_frame();

private:
  void exchange(int &a, int &b)
  {
    int temp;
    temp = a;
    a = b;
    b = temp;
  }

  void sort_vertexes(Triangle &triangle);
  void rotate(Triangle &triangle, const Rotation &rotation);
  void rotate(int &x, int &y, int &z, const Rotation &rotation);

  int width, height;
  int color_count;
  uint8_t *picture;
  int16_t *z_buffer;
  uint32_t palette[256];
  GifCompressor gif;
};

#endif

