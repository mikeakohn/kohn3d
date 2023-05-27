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

class Texture
{
public:
  Texture();
  virtual ~Texture();

  uint32_t get_pixel(double u, double v);
  int load(const char *filename);
  void set_scale(int x0, int y0, int x1, int y1, int x2, int y2);
  uint32_t get_color(int x, int y);

  void set_coords(
    double u0, double v0,
    double u1, double v1,
    double u2, double v2)
  {
    coords.u0 = u0;
    coords.v0 = v0;
    coords.u1 = u1;
    coords.v1 = v1;
    coords.u2 = u2;
    coords.v2 = v2;
  }

  struct
  {
    double u0, v0;
    double u1, v1;
    double u2, v2;
  } coords;

  void reset_sort()
  {
    sorted.u[0] = coords.u0;
    sorted.v[0] = coords.v0;
    sorted.u[1] = coords.u1;
    sorted.v[1] = coords.v1;
    sorted.u[2] = coords.u2;
    sorted.v[2] = coords.v2;
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

private:
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
  float compute_angle(int x0, int y0, int x1, int y1, int x2, int y2);

  struct
  {
    double u[3], v[3];

    int x0, y0;
    int x1, y1;
    int x2, y2;
  } sorted;

  float angle_xy;
  float angle_xy_0;
  float angle_xy_1;
  float angle_uv;
  float angle_uv_0;
  float angle_uv_1;
  int side_0_xy;
  int side_0_uv;
  int side_1_xy;
  int side_1_uv;
  float angle_scale;
  float side_0_scale;
  float side_1_scale;

  Picture picture;
};

#endif

