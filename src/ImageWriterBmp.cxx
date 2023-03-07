/*

  Kohn3D - GIF drawing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ImageWriterBmp.h"

ImageWriterBmp::ImageWriterBmp(int width, int height, int depth) :
  ImageWriter(width, height)
{
}

ImageWriterBmp::~ImageWriterBmp()
{
  finish();
}

void ImageWriterBmp::finish()
{
}

int ImageWriterBmp::create_headers()
{

  return 0;
}

int ImageWriterBmp::add_frame(uint8_t *image, uint32_t *color_table)
{
  return 0;
}

