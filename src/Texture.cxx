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

Texture::Texture() :
  angle_xy      { 0.0 },
  //length_a      {   0 },
  //length_b      {   0 },
  scale_angle   { 0.0 },
  scale_a       { 0.0 },
  scale_b       { 0.0 }
  //polar_angle_a { 0.0 },
  //polar_angle_b { 0.0 }
{
}

Texture::~Texture()
{
}

int Texture::load(const char *filename)
{
  return picture.load(filename);
}

void Texture::set_scale(const PolarCoords &a, const PolarCoords &b)
{
  double p = fabs(b.p - a.p);

  compute_scale(p, a.r, b.r);
}

uint32_t Texture::get_pixel(double angle, int r)
{
  angle = angle * scale_angle;

  int scaled_r = compute_length_at(angle);
  int x, y;

  PolarCoords::to_xy(x, y, polar_a.p + angle, scaled_r);

  return picture.get_pixel(center_x + x, center_y + y);
}

uint32_t Texture::get_pixel(int x, int y)
{
  //if (x > width || y > height) { return 0; }

  return picture.get_pixel(x, y);
}

void Texture::compute_scale(double angle, int r0, int r1)
{
  scale_angle = angle_xy / angle;
  scale_a = (double)polar_a.r / (double)r0;
  scale_b = (double)polar_b.r / (double)r1;
}

void Texture::set_coords(
  double u0, double v0,
  double u1, double v1,
  double u2, double v2)
{
  convert_uv_to_xy(area.x0, area.y0, u0, v0);
  convert_uv_to_xy(area.x1, area.y1, u1, v1);
  convert_uv_to_xy(area.x2, area.y2, u2, v2);

  coords_uv.u0 = u0;
  coords_uv.v0 = v0;
  coords_uv.u1 = u1;
  coords_uv.v1 = v1;
  coords_uv.u2 = u2;
  coords_uv.v2 = v2;

  center_x = area.x1,
  center_y = area.y1,

  polar_a.from_xy(area.x1 - area.x0,  area.y1 - area.y0);
  polar_b.from_xy(area.x1 - area.x2,  area.y1 - area.y2);

  angle_xy = fabs(polar_b.p - polar_a.p);

  dump();
}

void Texture::dump()
{
  printf(" -- Texture --\n");
  printf("  picture: %d x %d\n", picture.get_width(), picture.get_height());

  printf("  u0=%f v0=%f\n", coords_uv.u0, coords_uv.v0);
  printf("  u1=%f v1=%f\n", coords_uv.u1, coords_uv.v1);
  printf("  u2=%f v2=%f\n", coords_uv.u2, coords_uv.v2);

  printf("  x0=%d y0=%d\n", area.x0, area.y0);
  printf("  x1=%d y1=%d\n", area.x1, area.y1);
  printf("  x2=%d y2=%d\n", area.x2, area.y2);

  printf("  a=[ angle=%f (%.2f), r=%d ]\n",
    polar_a.p,
    polar_a.get_degrees(),
    polar_a.r);
  printf("  b=[ angle=%f (%.2f), r=%d ]\n",
    polar_b.p,
    polar_b.get_degrees(),
    polar_b.r);

  printf("  angle_xy=%f\n", angle_xy);

  printf("  scale_angle=%f\n", scale_angle);
  printf("      scale_a=%f\n", scale_a);
  printf("      scale_b=%f\n", scale_b);
}

#if 0
double Texture::compute_angle(int x0, int y0, int x1, int y1, int x2, int y2)
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

  length_a = compute_length(ax, ay);
  length_b = compute_length(bx, by);
  const int ab = (ax * bx) + (ay * by);

  return acos((float)ab / (float)(length_a * length_b));
}
#endif

int Texture::compute_length_at(double p, double p0, double p1, int r0, int r1)
{
  // Calculate the fraction of the distance moved p is between p0 to p1.
  double n = (p - p0) / (p1 - p0);

  return (int)(((float)r1 - r0) * n);
}

int Texture::compute_length_at(double p)
{
  return compute_length_at(p, polar_a.p, polar_b.p, polar_a.r, polar_b.r);
}

