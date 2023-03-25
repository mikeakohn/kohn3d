/*

  Kohn3D - GIF drawing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#ifndef KOHN3D_H
#define KOHN3D_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "ImageWriterBmp.h"
#include "ImageWriterGif.h"
#include "ImageWriterAvi.h"
#include "Picture.h"

class Kohn3D
{
public:
  enum Format
  {
    FORMAT_GIF = 0,
    FORMAT_AVI8,
    FORMAT_AVI24,
    FORMAT_BMP8,
    FORMAT_BMP24
  };

  Kohn3D(int width, int height, Format format);
  ~Kohn3D();

  int create(const char *filename);
  void finish();

  // The following methods must be called before init_end();
  int add_color(int value);
  void set_color(int index, int value);
  void set_bg_color_index(uint8_t value) { image_writer->set_bg_color_index(value); }

  void set_transparent_color_index(uint8_t value)
  {
    image_writer->set_transparent_color_index(value);
  }

  // Delay value is 100ths of a second.
  void set_delay(int value) { image_writer->set_delay(value); }
  void set_fps(int value) { image_writer->set_fps(value); }

  static const int LOOP_INFINITE = 0;
  void set_loop_count(int value) { image_writer->set_loop_count(value); }
  void set_32bit() { is_32bit = true; }

  void init_end();
  uint8_t *get_picture() { return picture; }
  uint32_t *get_picture_32bit() { return picture_32bit; }
  void clear();

  void enable_alpha_blending(bool value) { do_alpha_blending = value; }

  void draw_pixel(int x, int y, uint32_t color)
  {
    if (x < 0 || x >= width) { return; }
    if (y < 0 || y >= height) { return; }

    int pixel = (y * width) + x;

    if (is_32bit)
    {
      if (do_alpha_blending) { color = calculate_alpha(color, pixel); }

      picture_32bit[pixel] = color;
    }
      else
    {
      picture[pixel] = color;
    }
  }

  void draw_pixel(int x, int y, uint32_t color, int z)
  {
    if (x < 0 || x >= width) { return; }
    if (y < 0 || y >= height) { return; }

    int pixel = (y * width) + x;

    if (z < z_buffer[pixel]) { return; }

    if (is_32bit)
    {
      picture_32bit[pixel] = color;
    }
      else
    {
      picture[pixel] = color;
    }

    z_buffer[pixel] = z;
  }

  void draw_line(int x0, int y0, int x1, int y1, uint32_t color);
  void draw_line(int x0, int y0, int z0, int x1, int y1, int z1, uint32_t color);
  void draw_line(int x0, int y0, int z0, int x1, int y1, int z1, int r0, int g0, int b0, int r1, int b1, int g1);

  void draw_rect(int x0, int y0, int x1, int y1, uint32_t color);
  void draw_rect(int x0, int y0, int x1, int y1, uint32_t color, int z);

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

  void draw_triangle(const Triangle &triangle, int x, int y, uint32_t color);
  void draw_triangle(const Triangle &triangle, int x, int y, int z, uint32_t color);
  void draw_triangle(const Triangle &triangle, int x, int y, int z, uint32_t *colors);

  void draw_triangle(
    const Triangle &triangle,
    const Rotation &rotation,
    int x, int y, int z,
    uint32_t color);

  void draw_triangle(
    const Triangle &triangle,
    const Rotation &rotation,
    int x, int y, int z,
    uint32_t *colors);

  void draw_picture(Picture &picture, int x, int y, int z = INT32_MIN);
  void draw_picture(Picture &picture, int x, int y, int width, int height, int z = INT32_MIN);

  void write_frame();

private:
  template<typename T> void exchange(T &a, T &b)
  {
    T temp;
    temp = a;
    a = b;
    b = temp;
  }

  void sort_vertexes(Triangle &triangle);
  void sort_vertexes(Triangle &triangle, uint32_t *colors);
  void rotate(Triangle &triangle, const Rotation &rotation);
  void rotate(int &x, int &y, int &z, const Rotation &rotation);
  uint32_t calculate_alpha(uint32_t color, int pixel);

  bool do_alpha_blending;
  bool is_32bit;
  int width, height;
  int color_count;
  int format;
  uint8_t *picture;
  uint32_t *picture_32bit;
  int16_t *z_buffer;
  uint32_t palette[256];
  ImageWriter *image_writer;
};

#endif

