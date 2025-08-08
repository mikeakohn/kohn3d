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
  length_a      {   0 },
  length_b      {   0 },
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

void Texture::set_scale(int x0, int y0, int x1, int y1, int x2, int y2)
{
#if 0
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
    (picture.get_width()  * sorted.u[1]) - (picture.get_width()  * sorted.u[0]),
    (picture.get_height() * sorted.v[1]) - (picture.get_height() * sorted.v[0]));

  side_1_uv = compute_length(
    (picture.get_width()  * sorted.u[1]) - (picture.get_width()  * sorted.u[2]),
    (picture.get_height() * sorted.v[1]) - (picture.get_height() * sorted.v[2]));

  side_0_scale = side_0_xy / side_0_uv;
  side_1_scale = side_1_xy / side_1_uv;

  printf("%f %f\n", angle_xy, 360 * (angle_xy / 6.28));
  printf("%f %f\n", angle_uv, 360 * (angle_uv / 6.28));
#endif
}

uint32_t Texture::get_pixel(double angle, int r)
{
#if 0
  double u0 = (double)picture.get_width() * u;
  double v0 = (double)picture.get_height() * v;

  return picture.get_pixel(u0, v0);
#endif

  // Convert triangle angle to texture angle.
  //double a = angle * scale_angle;

  //return picture.get_pixel(x, y);

  return 0;
}

uint32_t Texture::get_color(int x, int y)
{
#if 0
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
  a = a * scale_angle;
#endif

  return 0;
}

void Texture::compute_scale(double angle, int length_a, int length_b)
{
  scale_angle = angle_xy / angle;
  scale_a = (double)this->length_a / (double)length_a;
  scale_b = (double)this->length_b / (double)length_b;
}

#if 0
void Texture::compute_polar(double angle, int length_a, int length_b)
{
}
#endif

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

#if 0
  angle_xy = compute_angle(
    area.x0, area.y0,
    area.x1, area.y1,
    area.x2, area.y2);
    
  double a = (v1 - v0) / (u1 - u0);
  double b = (v2 - v0) / (u2 - u0);

  polar_angle_a = atan(a);
  polar_angle_b = atan(b);
#endif

  polar_a.from_xy(area.x1 - area.x0,  area.y1 - area.y0);
  polar_b.from_xy(area.x1 - area.x2,  area.y1 - area.y2);

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

  //printf("  angle_xy=%f\n", angle_xy);
  //printf("  angle_xy=%f\n", angle_xy);

#if 0
  printf("  scale_angle=%f\n", polar_angle_a);
  printf("      scale_a=%f\n", scale_a);
  printf("      scale_b=%f\n", scale_b);
#endif

  printf("  a=[ angle=%f (%.2f), r=%d ]\n",
    polar_a.p,
    polar_a.get_degrees(),
    polar_a.r);
  printf("  b=[ angle=%f (%.2f), r=%d ]\n",
    polar_b.p,
    polar_b.get_degrees(),
    polar_b.r);
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

