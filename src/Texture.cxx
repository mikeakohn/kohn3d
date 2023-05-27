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
  sorted.x0 = x0;
  sorted.y0 = y0;
  sorted.x1 = x1;
  sorted.y1 = y1;
  sorted.x2 = x2;
  sorted.y2 = y2;

  // FIXME: It's possible this really isn't needed since it's the same
  // as angle_xy_0 - angle_xy_1.
  angle_xy = compute_angle(x0, y0, x1, y1, x2, y2);

  int r;
  cartesian_to_polar(angle_xy_0, r, x1 - x0, y1 - y0);
  cartesian_to_polar(angle_xy_1, r, x2 - x0, y2 - y0);

  int u0 = sorted.u[0] * picture.get_width();
  int v0 = sorted.v[0] * picture.get_height();
  int u1 = sorted.u[1] * picture.get_width();
  int v1 = sorted.v[1] * picture.get_height();
  int u2 = sorted.u[2] * picture.get_width();
  int v2 = sorted.v[2] * picture.get_height();

  angle_uv = compute_angle(u0, v0, u1, v1, u2, v2);

  cartesian_to_polar(angle_uv_0, r, u1 - u0, v1 - v0);
  cartesian_to_polar(angle_uv_1, r, u2 - u0, v2 - v0);

  angle_scale = angle_uv / angle_xy;

  side_0_xy = compute_length(x1 - x0, y1 - y0);
  side_1_xy = compute_length(x1 - x2, y1 - y2);

  side_0_uv = compute_length(
    (picture.get_width() * sorted.u[1]) - (picture.get_width() * sorted.u[0]),
    (picture.get_height() * sorted.v[1]) - (picture.get_height() * sorted.v[0]));

  side_1_uv = compute_length(
    (picture.get_width() * sorted.u[1]) - (picture.get_width() * sorted.u[2]),
    (picture.get_height() * sorted.v[1]) - (picture.get_height() * sorted.v[2]));

  side_0_scale = side_0_xy / side_0_uv;
  side_1_scale = side_1_xy / side_1_uv;

  printf("%f %f\n", angle_xy, 360 * (angle_xy / 6.28));
  printf("%f %f\n", angle_uv, 360 * (angle_uv / 6.28));
}

uint32_t Texture::get_color(int x, int y)
{
  float a;
  int r;

  // Calculate (x, y) relative to the vertex of the triangle.
  x = x - sorted.x1;
  y = y - sorted.y1;

  // Convert to polar coordinates.
  cartesian_to_polar(a, r, x, y);

  // Scale the radius. The scale depends on angle since the left side
  // of the triangle has a different scale than the right side.

  // Scale the angle.
  a = a * angle_scale;

  return 0;
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

