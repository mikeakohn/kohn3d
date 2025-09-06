/*

  Kohn3D - GIF drawing library.

  Copyright 2023-2025 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <math.h>

#include "Kohn3D.h"
#include "Angle.h"
#include "PolarCoords.h"

Kohn3D::Kohn3D(int width, int height, Format format) :
  do_alpha_blending { false },
  is_32bit          { false },
  width             { width },
  height            { height },
  color_count       { 0 },
  //format            { format },
  picture_32bit     { nullptr },
  image_writer      { nullptr }
{
  switch (format)
  {
    case FORMAT_GIF:
      image_writer = new ImageWriterGif(width, height);
      break;
    case FORMAT_AVI8:
      image_writer = new ImageWriterAvi(width, height, 8);
      break;
    case FORMAT_AVI24:
      image_writer = new ImageWriterAvi(width, height, 24);
      is_32bit = true;
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

  picture  = (uint8_t *)malloc(width * height * sizeof(uint32_t));
  z_buffer = (int16_t *)malloc(width * height * sizeof(int16_t));

  if (is_32bit) { picture_32bit = (uint32_t *)picture; }

  memset(picture, 0, width * height * sizeof(uint32_t));

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
  memset(picture, 0, is_32bit ? width * height * sizeof(uint32_t) : width * height);

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

  int dx = x0 - x1;
  int dy = y0 - y1;

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

void Kohn3D::draw_line(const PolarCoords &coords, uint32_t color)
{
  int x1, y1;

  int x0 = coords.get_center_x();
  int y0 = coords.get_center_y();

  coords.to_xy_centered(x1, y1);

  draw_line(x0, y0, x1, y1, color);
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
  Texture &texture,
  int center_x, int center_y)
{
  double dz, z;
  uint32_t color;

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
      double p;
      int r;

      PolarCoords::from_xy(p, r, center_x - x, center_y - y0);

      //color = texture.get_pixel(x, y0);
      color = texture.get_pixel(p, r);
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
      double p;
      int r;

      PolarCoords::from_xy(p, r, center_x - x0, center_y - y);

      //color = texture.get_pixel(x0, y);
      color = texture.get_pixel(p, r);
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
      double p;
      int r;

      PolarCoords::from_xy(p, r, center_x - x, center_y - y);

      //color = texture.get_pixel(x, y);
      color = texture.get_pixel(p, r);
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
      double p;
      int r;

      PolarCoords::from_xy(p, r, center_x - x, center_y - y);

      //color = texture.get_pixel(x, y);
      color = texture.get_pixel(p, r);
      draw_pixel(x, (int)y, color, z);
      y += dydx;
      z += dz;
    }
  }
}

void Kohn3D::draw_line(
  int x0, int y0, int z0,
  int x1, int y1, int z1,
  int a0, int r0, int g0, int b0,
  int a1, int r1, int g1, int b1)
{
  uint32_t color;
  double dz, z;
  double da, a;
  double dr, r;
  double dg, g;
  double db, b;

  if (y0 == y1)
  {
    if (x1 < x0)
    {
      exchange(x0, x1);
      exchange(z0, z1);
      exchange(a0, a1);
      exchange(r0, r1);
      exchange(g0, g1);
      exchange(b0, b1);
    }

    double dx = (double)(x1 - x0);

    z = z0;
    a = a0;
    r = r0;
    g = g0;
    b = b0;
    dz = (double)(z1 - z0) / dx;
    da = (double)(a1 - a0) / dx;
    dr = (double)(r1 - r0) / dx;
    dg = (double)(g1 - g0) / dx;
    db = (double)(b1 - b0) / dx;

    for (int x = x0; x <= x1; x++)
    {
      color =
        ((uint32_t)a << 24) |
        ((uint32_t)r << 16) |
        ((uint32_t)g << 8) |
         (uint32_t)b;

      draw_pixel(x, y0, color, z);

      z += dz;
      a += da;
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
      exchange(a0, a1);
      exchange(r0, r1);
      exchange(g0, g1);
      exchange(b0, b1);
    }

    double dy = (double)(y1 - y0);

    z = z0;
    a = a0;
    r = r0;
    g = g0;
    b = b0;
    dz = (double)(z1 - z0) / dy;
    da = (double)(a1 - a0) / dy;
    dr = (double)(r1 - r0) / dy;
    dg = (double)(g1 - g0) / dy;
    db = (double)(b1 - b0) / dy;

    for (int y = y0; y <= y1; y++)
    {
      color =
        ((uint32_t)a << 24) |
        ((uint32_t)r << 16) |
        ((uint32_t)g << 8) |
         (uint32_t)b;

      draw_pixel(x0, y, color);

      z += dz;
      a += da;
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
    a = a0;
    r = r0;
    g = g0;
    b = b0;
    dz = (double)(z0 - z1) / dy;
    da = (double)(a0 - a1) / dy;
    dr = (double)(r0 - r1) / dy;
    dg = (double)(g0 - g1) / dy;
    db = (double)(b0 - b1) / dy;

    for (int y = y0; y <= y1; y++)
    {
      color =
        ((uint32_t)a << 24) |
        ((uint32_t)r << 16) |
        ((uint32_t)g << 8) |
         (uint32_t)b;

      draw_pixel((int)x, y, color, z);

      x += dxdy;
      z += dz;
      a += da;
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
    a = a0;
    r = r0;
    g = g0;
    b = b0;
    double dy = (double)(y0 - y1);
    dz = (double)(z0 - z1) / dy;
    da = (double)(a0 - a1) / dy;
    dr = (double)(r0 - r1) / dy;
    dg = (double)(g0 - g1) / dy;
    db = (double)(b0 - b1) / dy;

    for (int x = x0; x <= x1; x++)
    {
      color =
        ((uint32_t)a << 24) |
        ((uint32_t)r << 16) |
        ((uint32_t)g << 8) |
         (uint32_t)b;

      draw_pixel(x, (int)y, color, z);

      y += dydx;
      z += dz;
      a += da;
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

  translation(v, x, y, z);
  projection(v);

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
      draw_line((int)x0, y0, (int)z0, (int)x1, y0, (int)z1, color);

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
    draw_line((int)x0, y0, (int)z0, (int)x1, y0, (int)z1, color);

    x0 += dxdy_0;
    x1 += dxdy_1;
    z0 += dzdy_0;
    z1 += dzdy_1;
  }

  dxdy_1 = (double)(v.x2 - v.x1) / (double)(v.y2 - v.y1);
  dzdy_1 = (double)(v.z2 - v.z1) / (double)(v.y2 - v.y1);

  for (int y0 = v.y1; y0 < v.y2; y0++)
  {
    draw_line((int)x0, y0, (int)z0, (int)x1, y0, (int)z1, color);

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

  translation(v, x, y, z);
  projection(v);

  sort_vertexes(v, colors);

  double a0, r0, g0, b0;
  double a1, r1, g1, b1;
  double a2, r2, g2, b2;

  split_rgb(colors[0], a0, r0, g0, b0);
  split_rgb(colors[1], a1, r1, g1, b1);
  split_rgb(colors[2], a2, r2, g2, b2);

  double delta_y1y0 = (double)(v.y1 - v.y0);
  double delta_y2y0 = (double)(v.y2 - v.y0);
  double delta_y2y1 = (double)(v.y2 - v.y1);

  if (v.y0 == v.y1)
  {
    double dxdy_0 = (double)(v.x2 - v.x0) / delta_y2y0;;
    double dxdy_1 = (double)(v.x2 - v.x1) / delta_y2y1;
    double dzdy_0 = (double)(v.z2 - v.z0) / delta_y2y0;
    double dzdy_1 = (double)(v.z2 - v.z1) / delta_y2y1;
    double x0 = v.x0;
    double x1 = v.x1;
    double z0 = v.z0;
    double z1 = v.z1;

    double da0dy = (a2 - a0) / delta_y2y0;
    double dr0dy = (r2 - r0) / delta_y2y0;
    double dg0dy = (g2 - g0) / delta_y2y0;
    double db0dy = (b2 - b0) / delta_y2y0;
    double da1dy = (a2 - a1) / delta_y2y1;
    double dr1dy = (r2 - r1) / delta_y2y1;
    double dg1dy = (g2 - g1) / delta_y2y1;
    double db1dy = (b2 - b1) / delta_y2y1;

    for (int y0 = v.y0; y0 < v.y2; y0++)
    {
      draw_line((int)x0, y0, (int)z0, (int)x1, y0, (int)z1, a0, r0, g0, b0, a1, r1, g1, b1);

      x0 += dxdy_0;
      x1 += dxdy_1;
      z0 += dzdy_0;
      z1 += dzdy_1;
      a0 += da0dy;
      r0 += dr0dy;
      g0 += dg0dy;
      b0 += db0dy;
      a1 += da1dy;
      r1 += dr1dy;
      g1 += dg1dy;
      b1 += db1dy;
    }

    return;
  }

  double dxdy_0 = (double)(v.x2 - v.x0) / delta_y2y0;
  double dzdy_0 = (double)(v.z2 - v.z0) / delta_y2y0;
  double x0, x1;
  double z0, z1;
  double dxdy_1;
  double dzdy_1;

  dxdy_1 = (double)(v.x1 - v.x0) / delta_y1y0;
  dzdy_1 = (double)(v.z1 - v.z0) / delta_y1y0;

  double daedy = (a2 - a0) / delta_y2y0;
  double dredy = (r2 - r0) / delta_y2y0;
  double dgedy = (g2 - g0) / delta_y2y0;
  double dbedy = (b2 - b0) / delta_y2y0;

  double dady_0 = (a1 - a0) / delta_y1y0;
  double drdy_0 = (r1 - r0) / delta_y1y0;
  double dgdy_0 = (g1 - g0) / delta_y1y0;
  double dbdy_0 = (b1 - b0) / delta_y1y0;

  double ae = a0;
  double re = r0;
  double ge = g0;
  double be = b0;

  x0 = x1 = v.x0;
  z0 = z1 = v.z0;

  for (int y0 = v.y0; y0 < v.y1; y0++)
  {
    draw_line((int)x0, y0, (int)z0, (int)x1, y0, (int)z1, ae, re, ge, be, a0, r0, g0, b0);

    x0 += dxdy_0;
    x1 += dxdy_1;
    z0 += dzdy_0;
    z1 += dzdy_1;

    ae += daedy;
    re += dredy;
    ge += dgedy;
    be += dbedy;
    a0 += dady_0;
    r0 += drdy_0;
    g0 += dgdy_0;
    b0 += dbdy_0;
  }

  dxdy_1 = (double)(v.x2 - v.x1) / delta_y2y1;
  dzdy_1 = (double)(v.z2 - v.z1) / delta_y2y1;

  double dady_1 = (a1 - a0) / delta_y1y0;
  double drdy_1 = (r1 - r0) / delta_y1y0;
  double dgdy_1 = (g1 - g0) / delta_y1y0;
  double dbdy_1 = (b1 - b0) / delta_y1y0;

  for (int y0 = v.y1; y0 < v.y2; y0++)
  {
    draw_line((int)x0, y0, (int)z0, (int)x1, y0, (int)z1, ae, re, ge, be, a0, r0, g0, b0);

    x0 += dxdy_0;
    x1 += dxdy_1;
    z0 += dzdy_0;
    z1 += dzdy_1;

    ae += daedy;
    re += dredy;
    ge += dgedy;
    be += dbedy;
    a0 += dady_1;
    r0 += drdy_1;
    g0 += dgdy_1;
    b0 += dbdy_1;
  }
}

void Kohn3D::draw_triangle(
  const Triangle &triangle,
  int x,
  int y,
  int z,
  Texture &texture)
{
  Triangle v = triangle;

  translation(v, x, y, z);
  projection(v);

  texture.set_image_angle(v.x0, v.y0, v.x1, v.y1, v.x2, v.y2);
  //Angle angle;
  //angle.set_center(v.x1, v.y1);
  //angle.set_from_xy(v.x0, v.y0, v.x2, v.y2);

#if 0
  PolarCoords polar_a;
  PolarCoords polar_b;

  polar_a.from_xy(v.x1 - v.x0, v.y1 - v.y0);
  polar_a.from_xy(v.x1 - v.x2, v.y1 - v.y2);
#endif

  int center_x = v.x1;
  int center_y = v.y1;

  sort_vertexes(v, texture);

  //texture.set_scale(polar_a, polar_b);

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
      draw_line(
        (int)x0, y0, (int)z0,
        (int)x1, y0, (int)z1,
        texture, center_x, center_y);

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
    draw_line(
      (int)x0, y0, (int)z0,
      (int)x1, y0, (int)z1,
      texture, center_x, center_y);

    x0 += dxdy_0;
    x1 += dxdy_1;
    z0 += dzdy_0;
    z1 += dzdy_1;
  }

  dxdy_1 = (double)(v.x2 - v.x1) / (double)(v.y2 - v.y1);
  dzdy_1 = (double)(v.z2 - v.z1) / (double)(v.y2 - v.y1);

  for (int y0 = v.y1; y0 < v.y2; y0++)
  {
    draw_line(
      (int)x0, y0, (int)z0,
      (int)x1, y0, (int)z1,
      texture, center_x, center_y);

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

void Kohn3D::draw_triangle(
  const Triangle &triangle,
  const Rotation &rotation,
  int x,
  int y,
  int z,
  Texture &texture)
{
  Triangle v = triangle;

  rotate(v, rotation);
  draw_triangle(v, x, y, z, texture);
}

void Kohn3D::draw_picture(Picture &picture, int x0, int y0, int z)
{
  int w = picture.get_width();
  int h = picture.get_height();
  int y = y0;

  for (int m = 0; m < h; m++)
  {
    int x = x0;

    for (int n = 0; n < w; n++)
    {
      uint32_t color = picture.get_pixel(n, m);

      if (z == INT32_MIN)
      {
        draw_pixel(x, y, color);
      }
        else
      {
        draw_pixel(x, y, color, z);
      }

      x++;
    }

    y++;
  }
}

void Kohn3D::draw_picture(
  Picture &picture,
  int x0,
  int y0,
  int width,
  int height,
  int z)
{
  int w = picture.get_width();
  int h = picture.get_height();

  double scale_x = (double)w / (double)width;
  double scale_y = (double)h / (double)height;
  double v = 0.0;

  for (int y = 0; y < height; y++)
  {
    double u = 0.0;

    for (int x = 0; x < width; x++)
    {
      uint32_t color = picture.get_pixel(u, v);

      if (z == INT32_MIN)
      {
        draw_pixel(x0 + x, y0 + y, color);
      }
        else
      {
        draw_pixel(x0 + x, y0 + y, color, z);
      }

      u += scale_x;
    }

    v += scale_y;
  }
}

void Kohn3D::draw_picture_high_quality(
  Picture &picture,
  int x0,
  int y0,
  int width,
  int height,
  int z)
{
  int w = picture.get_width();
  int h = picture.get_height();

  double scale_x = (double)w / (double)width;
  double scale_y = (double)h / (double)height;
  double v = 0.0;

  for (int y = 0; y < height; y++)
  {
    double u = 0.0;

    for (int x = 0; x < width; x++)
    {
      uint32_t color = picture.get_scaled_pixel(u, v, scale_x, scale_y);

      if (z == INT32_MIN)
      {
        draw_pixel(x0 + x, y0 + y, color);
      }
        else
      {
        draw_pixel(x0 + x, y0 + y, color, z);
      }

      u += scale_x;
    }

    v += scale_y;
  }
}

void Kohn3D::write_frame()
{
  image_writer->add_frame(picture, palette);
}

void Kohn3D::dump()
{
  printf(" -- Kohn3D --\n");
  printf("width=%d height=%d\n", width, height);
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

void Kohn3D::sort_vertexes(Triangle &triangle, Texture &texture)
{
  texture.reset_sort();

  // Sort vertexes to y0 is top, y1 is middle, and y2 is bottom.
  if (triangle.y0 > triangle.y1)
  {
    exchange(triangle.x0, triangle.x1);
    exchange(triangle.y0, triangle.y1);
    exchange(triangle.z0, triangle.z1);
    texture.exchange(0, 1);
  }

  if (triangle.y1 > triangle.y2)
  {
    exchange(triangle.x1, triangle.x2);
    exchange(triangle.y1, triangle.y2);
    exchange(triangle.z1, triangle.z2);
    texture.exchange(1, 2);
  }

  if (triangle.y0 > triangle.y1)
  {
    exchange(triangle.x0, triangle.x1);
    exchange(triangle.y0, triangle.y1);
    exchange(triangle.z0, triangle.z1);
    texture.exchange(0, 1);
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

void Kohn3D::translation(Triangle &triangle, int x, int y, int z)
{
  triangle.x0 += x;
  triangle.y0 += y;
  triangle.z0 += z;
  triangle.x1 += x;
  triangle.y1 += y;
  triangle.z1 += z;
  triangle.x2 += x;
  triangle.y2 += y;
  triangle.z2 += z;
}

void Kohn3D::projection(Triangle &triangle)
{
  // Weak perspective projection.
  //const double scale = -16738.0;
  const double scale = -1024.0;

  if (triangle.z0 != 0)
  {
    double z = triangle.z0 + scale;
    triangle.x0 = scale * ((double)triangle.x0 / z);
    triangle.y0 = scale * ((double)triangle.y0 / z);
  }

  if (triangle.z1 != 0)
  {
    double z = triangle.z1 + scale;
    triangle.x1 = scale * ((double)triangle.x1 / z);
    triangle.y1 = scale * ((double)triangle.y1 / z);
  }

  if (triangle.z2 != 0)
  {
    double z = triangle.z2 + scale;
    triangle.x2 = scale * ((double)triangle.x2 / z);
    triangle.y2 = scale * ((double)triangle.y2 / z);
  }
}

uint32_t Kohn3D::calculate_alpha(uint32_t color, int pixel)
{
  int level = color >> 24;
  if (level == 0) { return picture_32bit[pixel]; }
  if (level == 0xff) { return color; }

  uint32_t old = picture_32bit[pixel];
  float color_fract = (float)level / 255.0f;
  float old_fract = (float)(0xff - level) / 255.0f;

  int old_r = ((old >> 16) & 0xff);
  int old_g = ((old >> 8) & 0xff);
  int old_b = (old & 0xff);
  int color_r = ((color >> 16) & 0xff);
  int color_g = ((color >> 8) & 0xff);
  int color_b = (color & 0xff);

  color_r = (color_r * color_fract) + (old_r * old_fract);
  color_g = (color_g * color_fract) + (old_g * old_fract);
  color_b = (color_b * color_fract) + (old_b * old_fract);

  if (color_r > 255) { color_r = 255; }
  if (color_g > 255) { color_g = 255; }
  if (color_b > 255) { color_b = 255; }

  return (color_r << 16) | (color_g << 8) | color_b;
}

