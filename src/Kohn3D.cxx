/*

  Kohn3D - GIF drawing library.

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

Kohn3D::Kohn3D(int width, int height, Format format) :
  is_32bit { false },
  width { width },
  height { height },
  color_count { 0 },
  format { format },
  picture_32bit { nullptr },
  image_writer { nullptr }
{
  switch (format)
  {
    case FORMAT_GIF:
      image_writer = new ImageWriterGif(width, height);
      break;
    case FORMAT_BMP8:
      image_writer = new ImageWriterBmp(width, height, 8);
      break;
    case FORMAT_BMP24:
      image_writer = new ImageWriterBmp(width, height, 24);
      is_32bit = true;
      break;
    default:
      break;
  }

  z_buffer = (int16_t *)malloc(width * height * sizeof(int16_t));

  if (is_32bit)
  {
    picture = (uint8_t *)malloc(width * height * sizeof(uint32_t));
    picture_32bit = (uint32_t *)picture;
  }
    else
  {
    picture = (uint8_t *)malloc(width * height);
  }

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
  return image_writer->create(filename);
}

void Kohn3D::finish()
{
  return image_writer->finish();
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
  image_writer->set_palette(palette, color_count);
  image_writer->create_headers();
}

void Kohn3D::clear()
{
  memset(picture, 0, width * height);

  for (int i = 0; i < width * height; i++)
  {
    z_buffer[i] = -32767;
  }
}

void Kohn3D::draw_line(int x0, int y0, int x1, int y1, uint32_t color)
{
  if (y0 == y1)
  {
    if (x1 < x0) { exchange(x0, x1); }
    for (int x = x0; x <= x1; x++) { draw_pixel(x, y0, color); }
    return;
  }

  if (x0 == x1)
  {
    if (y1 < y0) { exchange(y0, y1); }
    for (int y = y0; y <= y1; y++) { draw_pixel(x0, y, color); }
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
      draw_pixel((int)x, y, color);
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
      draw_pixel(x, (int)y, color);
      y += dydx;
    }
  }
}

void Kohn3D::draw_line(
  int x0, int y0, int z0,
  int x1, int y1, int z1,
  uint32_t color)
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
      draw_pixel(x, y0, color, z);
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
      draw_pixel(x0, y, color);
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
      draw_pixel((int)x, y, color, z);
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
      draw_pixel(x, (int)y, color, z);
      y += dydx;
      z += dz;
    }
  }
}

void Kohn3D::draw_line(
  int x0, int y0, int z0,
  int x1, int y1, int z1,
  int r0, int g0, int b0,
  int r1, int g1, int b1)
{
  uint32_t color;
  double dz, z;
  double dr, r;
  double dg, g;
  double db, b;

  if (y0 == y1)
  {
    if (x1 < x0)
    {
      exchange(x0, x1);
      exchange(z0, z1);
      exchange(r0, r1);
      exchange(g0, g1);
      exchange(b0, b1);
    }

    double dx = (double)(x1 - x0);

    z = z0;
    r = r0;
    g = g0;
    b = b0;
    dz = (double)(z1 - z0) / dx;
    dr = (double)(r1 - r0) / dx;
    dg = (double)(g1 - g0) / dx;
    db = (double)(b1 - b0) / dx;

    for (int x = x0; x <= x1; x++)
    {
      color = ((int)r << 16) | ((int)g << 8) | (int)b;
      draw_pixel(x, y0, color, z);
      z += dz;
      r += dr;
      g += dg;
      b += db;
    }

    return;
  }

  if (x0 == x1)
  {
    if (x1 < x0)
    {
      exchange(y0, y1);
      exchange(z0, z1);
      exchange(r0, r1);
      exchange(g0, g1);
      exchange(b0, b1);
    }

    double dy = (double)(y1 - y0);

    z = z0;
    r = r0;
    g = g0;
    b = b0;
    dz = (double)(z1 - z0) / dy;
    dr = (double)(r1 - r0) / dy;
    dg = (double)(g1 - g0) / dy;
    db = (double)(b1 - b0) / dy;

    for (int y = y0; y <= y1; y++)
    {
      color = ((int)r << 16) | ((int)g << 8) | (int)b;
      draw_pixel(x0, y, color);
      z += dz;
      r += dr;
      g += dg;
      b += db;
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
      exchange(r0, r1);
      exchange(g0, g1);
      exchange(b0, b1);
    }

    double dy = (double)(y0 - y1);
    double dxdy = (double)(x0 - x1) / dy;
    double x = (double)x0;

    z = z0;
    r = r0;
    g = g0;
    b = b0;
    dz = (double)(z0 - z1) / dy;
    dr = (double)(r0 - r1) / dy;
    dg = (double)(g0 - g1) / dy;
    db = (double)(b0 - b1) / dy;

    for (int y = y0; y <= y1; y++)
    {
      color = ((int)r << 16) | ((int)g << 8) | (int)b;
      draw_pixel((int)x, y, color, z);
      x += dxdy;
      z += dz;
      r += dr;
      g += dg;
      b += db;
    }
  }
    else
  {
    if (x0 > x1)
    {
      exchange(x0, x1);
      exchange(y0, y1);
      exchange(z0, z1);
      exchange(r0, r1);
      exchange(g0, g1);
      exchange(b0, b1);
    }

    double dydx = (double)(y0 - y1) / (double)(x0 - x1);
    double y = (double)y0;

    z = z0;
    r = r0;
    g = g0;
    b = b0;
    double dy = (double)(y0 - y1);
    dz = (double)(z0 - z1) / dy;
    dr = (double)(r0 - r1) / dy;
    dg = (double)(g0 - g1) / dy;
    db = (double)(b0 - b1) / dy;

    for (int x = x0; x <= x1; x++)
    {
      color = ((int)r << 16) | ((int)g << 8) | (int)b;
      draw_pixel(x, (int)y, color, z);
      y += dydx;
      z += dz;
      r += dr;
      g += dg;
      b += db;
    }
  }
}

void Kohn3D::draw_rect(int x0, int y0, int x1, int y1, uint32_t color)
{
  if (x0 > x1) { exchange(x0, x1); }
  if (y0 > y1) { exchange(y0, y1); }

  for (int y = y0; y <= y1; y++)
  {
    for (int x = x0; x <= x1; x++)
    {
      draw_pixel(x, y, color);
    }
  }
}

void Kohn3D::draw_rect(
  int x0, int y0,
  int x1, int y1,
  uint32_t color,
  int z)
{
  if (x0 > x1) { exchange(x0, x1); }
  if (y0 > y1) { exchange(y0, y1); }

  for (int y = y0; y <= y1; y++)
  {
    for (int x = x0; x <= x1; x++)
    {
      draw_pixel(x, y, color, z);
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
  uint32_t color)
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
      draw_line((int)x0 + x, y0 + y, (int)x1 + x, y0 + y, color);

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
    draw_line((int)x0 + x, y0 + y, (int)x1 + x, y0 + y, color);

    x0 += dxdy_0;
    x1 += dxdy_1;
  }

  dxdy_1 = (double)(v.x2 - v.x1) / (double)(v.y2 - v.y1);

  for (int y0 = v.y1; y0 < v.y2; y0++)
  {
    draw_line((int)x0 + x, y0 + y, (int)x1 + x, y0 + y, color);

    x0 += dxdy_0;
    x1 += dxdy_1;
  }
}

void Kohn3D::draw_triangle(
  const Triangle &triangle,
  int x,
  int y,
  int z,
  uint32_t color)
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
      draw_line((int)x0 + x, y0 + y, (int)z0 + z, (int)x1 + x, y0 + y, (int)z1 + z, color);

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
    draw_line((int)x0 + x, y0 + y, (int)z0 + z, (int)x1 + x, y0 + y, (int)z1 + z, color);

    x0 += dxdy_0;
    x1 += dxdy_1;
    z0 += dzdy_0;
    z1 += dzdy_1;
  }

  dxdy_1 = (double)(v.x2 - v.x1) / (double)(v.y2 - v.y1);
  dzdy_1 = (double)(v.z2 - v.z1) / (double)(v.y2 - v.y1);

  for (int y0 = v.y1; y0 < v.y2; y0++)
  {
    draw_line((int)x0 + x, y0 + y, (int)z0 + z, (int)x1 + x, y0 + y, (int)z1 + z, color);

    x0 += dxdy_0;
    x1 += dxdy_1;
    z0 += dzdy_0;
    z1 += dzdy_1;
  }
}

void Kohn3D::draw_triangle(
  const Triangle &triangle,
  int x,
  int y,
  int z,
  uint32_t *colors)
{
  Triangle v = triangle;

  sort_vertexes(v, colors);

  double r0 = (colors[0] >> 16) & 0xff;
  double g0 = (colors[0] >> 8) & 0xff;
  double b0 =  colors[0] & 0xff;
  double r1 = (colors[1] >> 16) & 0xff;
  double g1 = (colors[1] >> 8) & 0xff;
  double b1 =  colors[1] & 0xff;
  double r2 = (colors[2] >> 16) & 0xff;
  double g2 = (colors[2] >> 8) & 0xff;
  double b2 =  colors[2] & 0xff;

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

    double dr0dy = (r2 - r0) / (double)(v.y2 - v.y0);
    double dg0dy = (g2 - g0) / (double)(v.y2 - v.y0);
    double db0dy = (b2 - b0) / (double)(v.y2 - v.y0);
    double dr1dy = (r2 - r1) / (double)(v.y2 - v.y1);
    double dg1dy = (g2 - g1) / (double)(v.y2 - v.y1);
    double db1dy = (b2 - b1) / (double)(v.y2 - v.y1);

    for (int y0 = v.y0; y0 < v.y2; y0++)
    {
      draw_line((int)x0 + x, y0 + y, (int)z0 + z, (int)x1 + x, y0 + y, (int)z1 + z, r0, g0, b0, r1, g1, b1);

      x0 += dxdy_0;
      x1 += dxdy_1;
      z0 += dzdy_0;
      z1 += dzdy_1;
      r0 += dr0dy;
      g0 += dg0dy;
      b0 += db0dy;
      r1 += dr1dy;
      g1 += dg1dy;
      b1 += db1dy;
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

  double dredy = (r2 - r0) / (double)(v.y2 - v.y0);
  double dgedy = (g2 - g0) / (double)(v.y2 - v.y0);
  double dbedy = (b2 - b0) / (double)(v.y2 - v.y0);

  double drdy_0 = (r1 - r0) / (double)(v.y1 - v.y0);
  double dgdy_0 = (g1 - g0) / (double)(v.y1 - v.y0);
  double dbdy_0 = (b1 - b0) / (double)(v.y1 - v.y0);

  double re = r0;
  double ge = g0;
  double be = b0;

  x0 = x1 = v.x0;
  z0 = z1 = v.z0;

  for (int y0 = v.y0; y0 < v.y1; y0++)
  {
    draw_line((int)x0 + x, y0 + y, (int)z0 + z, (int)x1 + x, y0 + y, (int)z1 + z, re, ge, be, r0, g0, b0);

    x0 += dxdy_0;
    x1 += dxdy_1;
    z0 += dzdy_0;
    z1 += dzdy_1;

    re += dredy;
    ge += dgedy;
    be += dbedy;
    r0 += drdy_0;
    g0 += dgdy_0;
    b0 += dbdy_0;
  }

  dxdy_1 = (double)(v.x2 - v.x1) / (double)(v.y2 - v.y1);
  dzdy_1 = (double)(v.z2 - v.z1) / (double)(v.y2 - v.y1);

  double drdy_1 = (r1 - r0) / (double)(v.y1 - v.y0);
  double dgdy_1 = (g1 - g0) / (double)(v.y1 - v.y0);
  double dbdy_1 = (b1 - b0) / (double)(v.y1 - v.y0);

  for (int y0 = v.y1; y0 < v.y2; y0++)
  {
    draw_line((int)x0 + x, y0 + y, (int)z0 + z, (int)x1 + x, y0 + y, (int)z1 + z, re, ge, be, r0, g0, b0);

    x0 += dxdy_0;
    x1 += dxdy_1;
    z0 += dzdy_0;
    z1 += dzdy_1;

    re += dredy;
    ge += dgedy;
    be += dbedy;
    r0 += drdy_1;
    g0 += dgdy_1;
    b0 += dbdy_1;
  }
}

void Kohn3D::draw_triangle(
  const Triangle &triangle,
  const Rotation &rotation,
  int x,
  int y,
  int z,
  uint32_t color)
{
  Triangle v = triangle;

  rotate(v, rotation);
  draw_triangle(v, x, y, z, color);
}

void Kohn3D::draw_triangle(
  const Triangle &triangle,
  const Rotation &rotation,
  int x,
  int y,
  int z,
  uint32_t *colors)
{
  Triangle v = triangle;

  rotate(v, rotation);
  draw_triangle(v, x, y, z, colors);
}

void Kohn3D::write_frame()
{
  image_writer->add_frame(picture, palette);
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

void Kohn3D::sort_vertexes(Triangle &triangle, uint32_t *colors)
{
  // Sort vertexes to y0 is top, y1 is middle, and y2 is bottom.
  if (triangle.y0 > triangle.y1)
  {
    exchange(triangle.x0, triangle.x1);
    exchange(triangle.y0, triangle.y1);
    exchange(triangle.z0, triangle.z1);
    exchange(colors[0], colors[1]);
  }

  if (triangle.y1 > triangle.y2)
  {
    exchange(triangle.x1, triangle.x2);
    exchange(triangle.y1, triangle.y2);
    exchange(triangle.z1, triangle.z2);
    exchange(colors[1], colors[2]);
  }

  if (triangle.y0 > triangle.y1)
  {
    exchange(triangle.x0, triangle.x1);
    exchange(triangle.y0, triangle.y1);
    exchange(triangle.z0, triangle.z1);
    exchange(colors[0], colors[1]);
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

