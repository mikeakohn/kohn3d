/*
  Kohn3D - GIF drawing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ImageReaderBmp.h"
#include "ImageReaderGif.h"
#include "Picture.h"

Picture::Picture() :
  data { nullptr },
  width { 0 },
  height { 0 }
{
}

Picture::~Picture()
{
  if (data != nullptr) { free(data); }
  data = nullptr;
}

int Picture::create(int width, int height)
{
  this->width = width;
  this->height = height;

  data = (uint32_t *)malloc(width * height * sizeof(uint32_t));

  return 0;
}

int Picture::load_bmp(const char *filename)
{
  ImageReaderBmp image_reader;

  int result = image_reader.load(filename);
  set_data(image_reader.get_image());
  width = image_reader.get_width();
  height = image_reader.get_height();

  return result;
}

int Picture::load_gif(const char *filename)
{
  ImageReaderGif image_reader;

  int result = image_reader.load(filename);
  set_data(image_reader.get_image());
  width = image_reader.get_width();
  height = image_reader.get_height();

  return result;
}

void Picture::set_color_transparent(uint32_t value)
{
  int pixel_count = get_pixel_count();

  for (int n = 0; n < pixel_count; n++)
  {
    uint32_t color = get_pixel(n);

    if (color != value) { continue; }

    color = color & 0xffffff;
    set_pixel(n, color);
  }
}

void Picture::update_alpha(uint8_t value)
{
  int pixel_count = get_pixel_count();
  uint32_t alpha_mask = value << 24;

  for (int n = 0; n < pixel_count; n++)
  {
    uint32_t color = get_pixel(n);

    color = color & 0xffffff;
    color |= alpha_mask;
    set_pixel(n, color);
  }
}

void Picture::update_alpha(uint8_t value, uint32_t ignore_color)
{
  int pixel_count = get_pixel_count();
  uint32_t alpha_mask = value << 24;

  for (int n = 0; n < pixel_count; n++)
  {
    uint32_t color = get_pixel(n);

    if (color != ignore_color)
    {
      color = color & 0xffffff;
      color |= alpha_mask;
      set_pixel(n, color);
    }
  }
}

uint32_t Picture::get_scaled_pixel(double u, double v, double w, double h)
{
  // If this maps into a single pixel, skip the code below and just
  // return the value at (u , v).
  if ((int)u == (int)(u + w) &&
      (int)v == (int)(v + h))
  {
    return get_pixel((int)u, (int)v);
  }

  double a_sum = 0;
  double r_sum = 0;
  double g_sum = 0;
  double b_sum = 0;
  double total = 0;

  for (double y = v; y < v + h; y += 1.0)
  {
    double fy1 = (y + 1.0) - (int)(y + 1.0);
    double fy0 = 1.0 - fy1;

    for (double x = u; x < u + w; x += 1.0)
    {
      // Calculate and average up to 4 pixels.
      // f0 | f1
      // -------
      // f2 | f3

      double fx1 = (x + 1.0) - (int)(x + 1.0);
      double fx0 = 1.0 - fx1;

      double f0 = fx0 * fy0;
      double f1 = fx1 * fy0;
      double f2 = fx0 * fy1;
      double f3 = fx1 * fy1;

      int x0 = (int)x;
      int y0 = (int)y;
      int x1 = (int)(x + 1.0);
      int y1 = (int)(y + 1.0);

      double a;
      double r;
      double g;
      double b;

      if (x0 >= 0 && y0 >= 0 && x0 < width && y0 < height)
      {
        split_rgb(get_pixel(x0, y0), a, r, g, b);
        a_sum += a * f0;
        r_sum += r * f0;
        g_sum += g * f0;
        b_sum += b * f0;
        total += f0;
      }

      if (x1 >= 0 && y0 >= 0 && x1 < width && y0 < height)
      {
        split_rgb(get_pixel(x1, y0), a, r, g, b);
        a_sum += a * f1;
        r_sum += r * f1;
        g_sum += g * f1;
        b_sum += b * f1;
        total += f1;
      }

      if (x0 >= 0 && y1 >= 0 && x0 < width && y1 < height)
      {
        split_rgb(get_pixel(x0, y1), a, r, g, b);
        a_sum += a * f2;
        r_sum += r * f2;
        g_sum += g * f2;
        b_sum += b * f2;
        total += f2;
      }

      if (x1 >= 0 && y1 >= 0 && x1 < width && y1 < height)
      {
        split_rgb(get_pixel(x1, y1), a, r, g, b);
        a_sum += a * f3;
        r_sum += r * f3;
        g_sum += g * f3;
        b_sum += b * f3;
        total += f3;
      }
    }
  }

  if (total == 0) { return 0; }

  int a = a_sum / total;
  int r = r_sum / total;
  int g = g_sum / total;
  int b = b_sum / total;

  if (a > 255) { a = 255; }
  if (r > 255) { r = 255; }
  if (g > 255) { g = 255; }
  if (b > 255) { b = 255; }

  return (a << 24) | (r << 16) | (g << 8) | b;
}

