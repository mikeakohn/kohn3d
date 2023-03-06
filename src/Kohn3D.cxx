/*

  GIF Factory - GIF drawing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "Kohn3D.h"

Kohn3D::Kohn3D(int width, int height) :
  width { width },
  height { height },
  color_count { 0 }
{
  picture = (uint8_t *)malloc(width * height);
  z_buffer = (int16_t *)malloc(width * height * sizeof(int16_t));
  gif.set_width(width);
  gif.set_height(height);
  clear();
}

Kohn3D::~Kohn3D()
{
  finish();
  free(picture);
  free(z_buffer);
}

int Kohn3D::create(const char *filename)
{
  return gif.create(filename);
}

void Kohn3D::finish()
{
  gif.finish();
}

int Kohn3D::add_color(int value)
{
  palette[color_count] = value;
  return color_count++;
}

void Kohn3D::set_color(int index, int value)
{
  palette[index] = value;
  if (color_count <= index) { color_count = index + 1; }
}

void Kohn3D::init_end()
{
  gif.set_palette(palette, color_count);
  gif.create_headers();
}

void Kohn3D::clear()
{
  memset(picture, 0, width * height);

  for (int i = 0; i < width * height; i++)
  {
    z_buffer[i] = -32767;
  }
}

void Kohn3D::draw_line(int x0, int y0, int x1, int y1, int color_index)
{
  if (y0 == y1)
  {
    if (x1 < x0) { exchange(x0, x1); }
    for (int x = x0; x <= x1; x++) { draw_pixel(x, y0, color_index); }
    return;
  }

  if (x0 == x1)
  {
    if (y1 < y0) { exchange(y0, y1); }
    for (int y = y0; y <= y1; y++) { draw_pixel(x0, y, color_index); }
    return;
  }

  int dx = (x0 - x1);
  int dy = (y0 - y1);

  if (abs(dx) < abs(dy))
  {
    if (y0 > y1)
    {
      exchange(x0, x1);
      exchange(y0, y1);
    }

    double dxdy = (double)(x0 - x1) / (double)(y0 - y1);
    double x = (double)x0;

    for (int y = y0; y <= y1; y++)
    {
      draw_pixel((int)x, y, color_index);
      x += dxdy;
    }
  }
    else
  {
    if (x0 > x1)
    {
      exchange(x0, x1);
      exchange(y0, y1);
    }

    double dydx = (double)(y0 - y1) / (double)(x0 - x1);
    double y = (double)y0;

    for (int x = x0; x <= x1; x++)
    {
      draw_pixel(x, (int)y, color_index);
      y += dydx;
    }
  }
}

void Kohn3D::draw_line(
  int x0, int y0, int z0,
  int x1, int y1, int z1,
  int color_index)
{
  double dz, z;

  if (y0 == y1)
  {
    if (x1 < x0)
    {
      exchange(x0, x1);
      exchange(z0, z1);
    }

    z = z0;
    dz = (double)(z1 - z0) / (double)(x1 - x0);

    for (int x = x0; x <= x1; x++)
    {
      draw_pixel(x, y0, color_index, z);
      z += dz;
    }

    return;
  }

  if (x0 == x1)
  {
    if (x1 < x0)
    {
      exchange(y0, y1);
      exchange(z0, z1);
    }

    z = z0;
    dz = (double)(z1 - z0) / (double)(y1 - y0);

    for (int y = y0; y <= y1; y++)
    {
      draw_pixel(x0, y, color_index);
      z += dz;
    }

    return;
  }

  int dx = (x0 - x1);
  int dy = (y0 - y1);

  if (abs(dx) < abs(dy))
  {
    if (y0 > y1)
    {
      exchange(x0, x1);
      exchange(y0, y1);
      exchange(z0, z1);
    }

    double dxdy = (double)(x0 - x1) / (double)(y0 - y1);
    double x = (double)x0;

    z = z0;
    dz = (double)(z1 - z0) / (double)(y1 - y0);

    for (int y = y0; y <= y1; y++)
    {
      draw_pixel((int)x, y, color_index, z);
      x += dxdy;
      z += dz;
    }
  }
    else
  {
    if (x0 > x1)
    {
      exchange(x0, x1);
      exchange(y0, y1);
      exchange(z0, z1);
    }

    double dydx = (double)(y0 - y1) / (double)(x0 - x1);
    double y = (double)y0;
    z = z0;
    dz = (double)(z1 - z0) / (double)(y1 - y0);

    for (int x = x0; x <= x1; x++)
    {
      draw_pixel(x, (int)y, color_index, z);
      y += dydx;
      z += dz;
    }
  }
}

void Kohn3D::draw_rect(int x0, int y0, int x1, int y1, int color_index)
{
  if (x0 > x1) { exchange(x0, x1); }
  if (y0 > y1) { exchange(y0, y1); }

  for (int y = y0; y <= y1; y++)
  {
    for (int x = x0; x <= x1; x++)
    {
      draw_pixel(x, y, color_index);
    }
  }
}

void Kohn3D::draw_rect(
  int x0, int y0,
  int x1, int y1,
  int color_index,
  int z)
{
  if (x0 > x1) { exchange(x0, x1); }
  if (y0 > y1) { exchange(y0, y1); }

  for (int y = y0; y <= y1; y++)
  {
    for (int x = x0; x <= x1; x++)
    {
      draw_pixel(x, y, color_index, z);
    }
  }
}

void Kohn3D::load_triangle(Triangle &triangle, const int coords[])
{
  triangle.x0 = coords[0];
  triangle.y0 = coords[1];
  triangle.z0 = coords[2];
  triangle.x1 = coords[3];
  triangle.y1 = coords[4];
  triangle.z1 = coords[5];
  triangle.x2 = coords[6];
  triangle.y2 = coords[7];
  triangle.z2 = coords[8];
}

void Kohn3D::load_rotation(Rotation &rotation, const float values[])
{
  rotation.rx = values[0];
  rotation.ry = values[1];
  rotation.rz = values[2];
}

void Kohn3D::draw_triangle(
  const Triangle &triangle,
  int x,
  int y,
  int color_index)
{
  Triangle v = triangle;

  sort_vertexes(v);

  if (v.y0 == v.y1)
  {
    double dxdy_0 = (double)(v.x2 - v.x0) / (double)(v.y2 - v.y0);
    double dxdy_1 = (double)(v.x2 - v.x1) / (double)(v.y2 - v.y1);
    double x0 = v.x0;
    double x1 = v.x1;

    for (int y0 = v.y0; y0 < v.y2; y0++)
    {
      draw_line((int)x0 + x, y0 + y, (int)x1 + x, y0 + y, color_index);

      x0 += dxdy_0;
      x1 += dxdy_1;
    }

    return;
  }

  double dxdy_0 = (double)(v.x2 - v.x0) / (double)(v.y2 - v.y0);
  double x0, x1;
  double dxdy_1;

  dxdy_1 = (double)(v.x1 - v.x0) / (double)(v.y1 - v.y0);

  x0 = x1 = v.x0;

  for (int y0 = v.y0; y0 < v.y1; y0++)
  {
    draw_line((int)x0 + x, y0 + y, (int)x1 + x, y0 + y, color_index);

    x0 += dxdy_0;
    x1 += dxdy_1;
  }

  dxdy_1 = (double)(v.x2 - v.x1) / (double)(v.y2 - v.y1);

  for (int y0 = v.y1; y0 < v.y2; y0++)
  {
    draw_line((int)x0 + x, y0 + y, (int)x1 + x, y0 + y, color_index);

    x0 += dxdy_0;
    x1 += dxdy_1;
  }
}

void Kohn3D::draw_triangle(
  const Triangle &triangle,
  int x,
  int y,
  int z,
  int color_index)
{
  Triangle v = triangle;

  sort_vertexes(v);

  if (v.y0 == v.y1)
  {
    double dxdy_0 = (double)(v.x2 - v.x0) / (double)(v.y2 - v.y0);
    double dxdy_1 = (double)(v.x2 - v.x1) / (double)(v.y2 - v.y1);
    double dzdy_0 = (double)(v.z2 - v.z0) / (double)(v.y2 - v.y0);
    double dzdy_1 = (double)(v.z2 - v.z1) / (double)(v.y2 - v.y1);
    double x0 = v.x0;
    double x1 = v.x1;
    double z0 = v.z0;
    double z1 = v.z1;

    for (int y0 = v.y0; y0 < v.y2; y0++)
    {
      draw_line((int)x0 + x, y0 + y, (int)z0 + z, (int)x1 + x, y0 + y, (int)z1 + z, color_index);

      x0 += dxdy_0;
      x1 += dxdy_1;
      z0 += dzdy_0;
      z1 += dzdy_1;
    }

    return;
  }

  double dxdy_0 = (double)(v.x2 - v.x0) / (double)(v.y2 - v.y0);
  double dzdy_0 = (double)(v.z2 - v.z0) / (double)(v.y2 - v.y0);
  double x0, x1;
  double z0, z1;
  double dxdy_1;
  double dzdy_1;

  dxdy_1 = (double)(v.x1 - v.x0) / (double)(v.y1 - v.y0);
  dzdy_1 = (double)(v.z1 - v.z0) / (double)(v.y1 - v.y0);

  x0 = x1 = v.x0;
  z0 = z1 = v.z0;

  for (int y0 = v.y0; y0 < v.y1; y0++)
  {
    draw_line((int)x0 + x, y0 + y, (int)z0 + z, (int)x1 + x, y0 + y, (int)z1 + z, color_index);

    x0 += dxdy_0;
    x1 += dxdy_1;
    z0 += dzdy_0;
    z1 += dzdy_1;
  }

  dxdy_1 = (double)(v.x2 - v.x1) / (double)(v.y2 - v.y1);
  dzdy_1 = (double)(v.z2 - v.z1) / (double)(v.y2 - v.y1);

  for (int y0 = v.y1; y0 < v.y2; y0++)
  {
    draw_line((int)x0 + x, y0 + y, (int)z0 + z, (int)x1 + x, y0 + y, (int)z1 + z, color_index);

    x0 += dxdy_0;
    x1 += dxdy_1;
    z0 += dzdy_0;
    z1 += dzdy_1;
  }
}

void Kohn3D::draw_triangle(
  const Triangle &triangle,
  const Rotation &rotation,
  int x,
  int y,
  int z,
  int color_index)
{
  Triangle v = triangle;

  rotate(v, rotation);
  draw_triangle(v, x, y, z, color_index);
}

void Kohn3D::write_frame()
{
  gif.compress(picture, palette);
}

void Kohn3D::sort_vertexes(Triangle &triangle)
{
  // Sort vertexes to y0 is top, y1 is middle, and y2 is bottom.
  if (triangle.y0 > triangle.y1)
  {
    exchange(triangle.x0, triangle.x1);
    exchange(triangle.y0, triangle.y1);
    exchange(triangle.z0, triangle.z1);
  }

  if (triangle.y1 > triangle.y2)
  {
    exchange(triangle.x1, triangle.x2);
    exchange(triangle.y1, triangle.y2);
    exchange(triangle.z1, triangle.z2);
  }

  if (triangle.y0 > triangle.y1)
  {
    exchange(triangle.x0, triangle.x1);
    exchange(triangle.y0, triangle.y1);
    exchange(triangle.z0, triangle.z1);
  }
}

void Kohn3D::rotate(Triangle &triangle, const Rotation &rotation)
{
  rotate(triangle.x0, triangle.y0, triangle.z0, rotation);
  rotate(triangle.x1, triangle.y1, triangle.z1, rotation);
  rotate(triangle.x2, triangle.y2, triangle.z2, rotation);
}

void Kohn3D::rotate(int &x, int &y, int &z, const Rotation &rotation)
{
  float t;
  float s, c;

  // [     1       0         0   ]   [ x ]
  // [     0   cos(rx)  -sin(ry) ] * [ y ]
  // [     0   sin(rx)   cos(ry) ]   [ z ]
  if (rotation.rx != 0)
  {
    c = cos(rotation.rx);
    s = sin(rotation.rx);

    t = (y * c) - (z * s);
    z = (y * s) + (z * c);
    y = t;
  }

  // [ cos(ry)     0     sin(ry) ]   [ x ]
  // [      0      1         0   ] * [ y ]
  // [ -sin(ry)    0     cos(ry) ]   [ z ]
  if (rotation.ry != 0)
  {
    c = cos(rotation.ry);
    s = sin(rotation.ry);

    t =  (x * c) + (z * s);
    z = -(x * s) + (z * c);
    x = t;
  }

  // [ cos(rz) -sin(rz)      0   ]   [ x ]
  // [ sin(rz)  cos(rz)      0   ] * [ y ]
  // [     0       0         1   ]   [ z ]
  if (rotation.rz != 0)
  {
    c = cos(rotation.rz);
    s = sin(rotation.rz);

    t = (x * c) - (y * s);
    y = (x * s) + (y * c);
    x = t;
  }
}

