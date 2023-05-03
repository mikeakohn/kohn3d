/*
  Kohn3D - GIF drawing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Texture.h"

Texture::Texture()
{
}

Texture::~Texture()
{
}

int Texture::load(const char *filename)
{
  return picture.load(filename);
}

uint32_t Texture::get_pixel(double u, double v)
{
  double u0 = (double)picture.get_width() * u;
  double v0 = (double)picture.get_height() * v;

  return picture.get_pixel(u0, v0);
}

void Texture::set_scale(int x0, int y0, int x1, int y1, int x2, int y2)
{
  float a = compute_angle(x0, y0, x1, y1, x2, y2);

  printf("%f %f\n", a, 360 * (a / 6.28));
}

float Texture::compute_angle(int x0, int y0, int x1, int y1, int x2, int y2)
{
  // Angle @ vertex (x1, y1) is
  //
  //         a * b
  // cos t = ------
  //         |a||b|
  //
  // a = vector from (x0, y0), (x1, y1)
  // b = vector from (x2, y2), (x1, y1)
  // a * b = (ax * bx) + (ay * by)

  const int ax = x1 - x0;
  const int ay = y1 - y0;
  const int bx = x1 - x2;
  const int by = y1 - y2;

  const int a_length = compute_length(ax, ay);
  const int b_length = compute_length(bx, by);
  const int ab = (ax * bx) + (ay * by);

  return acos((float)ab / (float)(a_length * b_length));
}

