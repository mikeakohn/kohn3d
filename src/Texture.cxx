/*
  Kohn3D - GIF drawing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Texture.h"

Texture::Texture()
{
}

Texture::~Texture()
{
}

uint32_t Texture::get_pixel(double u, double v)
{
  double u0 = (double)picture.get_width() * u;
  double v0 = (double)picture.get_height() * v;

  return picture.get_pixel(u0, v0);
}

