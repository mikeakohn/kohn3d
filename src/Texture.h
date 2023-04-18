/*

  Kohn3D - GIF drawing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#ifndef TEXTURE_H
#define TEXTURE_H

#include <stdint.h>

#include "Picture.h"

class Texture
{
public:
  Texture();
  virtual ~Texture();

  uint32_t get_pixel(double u, double v);
  int load(const char *filename);

  struct
  {
    double u0, v0;
    double u1, v1;
    double u2, v2;
  } coords;

private:
  struct
  {
    double u0, v0;
    double u1, v1;
    double u2, v2;
  } sorted;

  Picture picture;
};

#endif

