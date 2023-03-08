/*

  Kohn3D - GIF drawing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ImageWriter.h"

ImageWriter::ImageWriter(int width, int height) :
  fp { nullptr },
  width { width },
  height { height },
  max_colors { 0 },
  bg_color_index { 0 },
  transparent_color_index { 0 },
  do_transparency { false },
  delay { 0 },
  loop_count { -1 }
{
  memset(palette, 0, sizeof(palette));
}

ImageWriter::~ImageWriter()
{
  if (fp != nullptr) { fclose(fp); }
  fp = nullptr;
}

int ImageWriter::create(const char *filename)
{
  fp = fopen(filename, "wb");
  if (fp == nullptr) { return -1; }
  return 0;
}

void ImageWriter::set_palette(uint32_t *color_table, int length)
{
  max_colors = 0;

  for (int i = 0; i < length; i++)
  {
    palette[max_colors] = color_table[i];
    max_colors++;
  }

  // FIXME: Why is this happening?
  if (max_colors <= 2) { max_colors = 4; }
}

