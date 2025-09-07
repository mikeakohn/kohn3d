/*

  Kohn3D - GIF drawing library.

  Copyright 2023-2025 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdint.h>
#include <math.h>

#include "Angle.h"
#include "Picture.h"
#include "PolarCoords.h"

class Texture
{
public:
  Texture();
  virtual ~Texture();

  int load(const char *filename);
  //void set_scale(int x0, int y0, int x1, int y1, int x2, int y2);
  //void set_scale(const PolarCoords &a, const PolarCoords &b);

  void set_image_angle(int x0, int y0, int x1, int y1, int x2, int y2);

  uint32_t get_pixel(double angle, int r);
  uint32_t get_pixel(int x, int y);

  //void compute_scale(double angle, int length_a, int length_b);

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

  struct
  {
    double u[3], v[3];

    int x0, y0;
    int x1, y1;
    int x2, y2;
  } sorted;

  double scale_angle;

  Angle angle;
  Angle image_angle;

  AreaXY area;
  AreaUV coords_uv;

  int center_x;
  int center_y;

  Picture picture;
};

#endif

