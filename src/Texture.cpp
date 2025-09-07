/*
  Kohn3D - GIF drawing library.

  Copyright 2023-2025 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Texture.h"

Texture::Texture() :
  scale_angle { 0.0 },
  center_x    {   0 },
  center_y    {   0 }
{
}

Texture::~Texture()
{
}

int Texture::load(const char *filename)
{
  return picture.load(filename);
}

void Texture::set_image_angle(int x0, int y0, int x1, int y1, int x2, int y2)
{
  image_angle.set_center(x1, y1);
  image_angle.set_from_xy(x0, y0, x2, y2);

  scale_angle = fabs(image_angle.get_delta_p() / angle.get_delta_p());
}

uint32_t Texture::get_pixel(double ip, int ir)
{
  double diff_p = image_angle.get_angle_diff(ip);
  double tp = angle.get_angle_from_offset(diff_p, scale_angle);

  int image_length   = image_angle.compute_length_at(ip);
  int texture_length = angle.compute_length_at(tp);

  PolarCoords coords;
  coords = angle.coords_0;
  coords.p = tp;

  //  ir      image_length
  // ----- =  --------------
  //   r      texture_length

  coords.r = (double)ir * (double)texture_length / (double)image_length;

  int x, y;
  coords.to_xy_centered(x, y);

  return picture.get_pixel(x, y);
}

uint32_t Texture::get_pixel(int x, int y)
{
  PolarCoords coords = image_angle.coords_0;
  coords.from_xy_centered(x, y);

  return get_pixel(coords.p, coords.r);
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

  angle.set_center(center_x, center_y);
  angle.set_from_xy(area.x0, area.y0, area.x2, area.y2);

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

  angle.dump();

  printf("  scale_angle=%f\n", scale_angle);
}

