/*

  Kohn3D - GIF drawing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdint.h>
#include <math.h>

#include "Picture.h"
#include "PolarCoords.h"

class Texture
{
public:
  Texture();
  virtual ~Texture();

  int load(const char *filename);
  void set_scale(int x0, int y0, int x1, int y1, int x2, int y2);

  uint32_t get_pixel(double angle, int r);
  uint32_t get_color(int x, int y);

  void compute_scale(double angle, int length_a, int length_b);

  void set_coords(
    double u0, double v0,
    double u1, double v1,
    double u2, double v2);

  struct AreaUV
  {
    AreaUV() :
      u0 { 0.0 },
      v0 { 0.0 },
      u1 { 0.0 },
      v1 { 0.0 },
      u2 { 0.0 },
      v2 { 0.0 }
    {
    }

    double u0, v0;
    double u1, v1;
    double u2, v2;
  };

  // New code.
  struct AreaXY
  {
    AreaXY() :
      x0 { 0 },
      y0 { 0 },
      x1 { 0 },
      y1 { 0 },
      x2 { 0 },
      y2 { 0 }
    {
    }

    int x0, y0;
    int x1, y1;
    int x2, y2;
  };

  void reset_sort()
  {
#if 0
    sorted.u[0] = coords.u0;
    sorted.v[0] = coords.v0;
    sorted.u[1] = coords.u1;
    sorted.v[1] = coords.v1;
    sorted.u[2] = coords.u2;
    sorted.v[2] = coords.v2;
#endif
  }

  void exchange(int a, int b)
  {
    double temp;

    temp = sorted.u[a];
    sorted.u[a] = sorted.u[b];
    sorted.u[b] = temp;

    temp = sorted.v[a];
    sorted.v[a] = sorted.v[b];
    sorted.v[b] = temp;
  }

  void dump();

private:
  // New code.
  int convert_uv_to_xy(int &x, int &y, double u, double v)
  {
    int width  = picture.get_width();
    int height = picture.get_height();

    x = (float)width  * u;
    y = (float)height * v;

    return 0;
  }

  int compute_length(int a, int b)
  {
    return sqrt((a * a) + (b * b));
  }

  void polar_to_cartesian(int &x, int &y, float a, int r)
  {
    x = r * cos(a);
    y = r * sin(a);
  }

  void cartesian_to_polar(float &a, int &r, int x, int y)
  {
    // r = sqrt(x ^ 2 + y ^2);
    // a = atan2(y / x);    (only for positive x, positive y quadrant)

    r = sqrt((x * x) + (y * y));

    if (x == 0)
    {
      if (y > 0) { a = 6.28 / 4; }
      else if (y < 0) { a = (6.28 / 4) * 3; }
      else { a = 0; }
      return;
    }

    if (y == 0)
    {
      if (x > 0) { a = 0; }
      else if (x < 0) { a = (6.28 / 2); }
      else { a = 0; }
      return;
    }

    if (x > 0 && y < 0)
    {
      // atan(-1 / 4) will come out negative.
      a = 6.28 + atan(y / x);
    }
      else
    if (x < 0 && y < 0)
    {
      a = 3.14 + atan(y / x);
    }
      else
    if (x < 0 && y > 0)
    {
      a = 3.14 + atan(y / x);
    }
      else
    {
      a = atan(y / x);
    }
  }

  // (x1, y1) is the middle vertex where the angle is calculated.
  //double compute_angle(int x0, int y0, int x1, int y1, int x2, int y2);

  int compute_length_at(double p, double p0, double p1, int r0, int r1);

  struct
  {
    double u[3], v[3];

    int x0, y0;
    int x1, y1;
    int x2, y2;
  } sorted;

#if 0
  float angle_xy_0;
  float angle_xy_1;
  float angle_uv;
  float angle_uv_0;
  float angle_uv_1;
#endif
  int side_0_xy;
  int side_0_uv;
  int side_1_xy;
  int side_1_uv;
  int center_uv_x;
  int center_uv_y;

  double angle_xy;
  int length_a;
  int length_b;

  //double angle_scale;
  //double side_0_scale;
  //double side_1_scale;
  double scale_angle;
  double scale_a;
  double scale_b;

  //double polar_angle_a;
  //double polar_angle_b;

  PolarCoords polar_a;
  PolarCoords polar_b;

  // New code.
  // (x1, y1) is vertex.
  AreaXY area;
  AreaUV coords_uv;

  int center_x;
  int center_y;

  Picture picture;
};

#endif

