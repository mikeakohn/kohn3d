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

  struct
  {
    double u0, v0;
    double u1, v1;
    double u2, v2;
  } coords;

private:
  int compute_length(int a, int b)
  {
    return sqrt((a * a) + (b * b));
  }

  void polar_to_cartesian(int &x, int &y, float a, int r)
  {
    // x = r * cos(a);
    // y = r * sin(a);

    x = r * cos(a);
    y = r * sin(a);
  }

  void cartesian_to_polar(float &a, int &r, int x, int y)
  {
    // r = sqrt(x ^ 2 + y ^2);
    // a = atan2(y / x);

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
    double u0, v0;
    double u1, v1;
    double u2, v2;

    int x0, y0;
    int x1, y1;
    int x2, y2;
  } sorted;

  float angle_xy;
  float angle_uv;
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

