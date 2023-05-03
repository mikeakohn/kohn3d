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

  // (x1, y1) is the middle vertex where the angle is calculated.
  float compute_angle(int x0, int y0, int x1, int y1, int x2, int y2);

  struct
  {
    double u0, v0;
    double u1, v1;
    double u2, v2;
  } sorted;

  Picture picture;
};

#endif

