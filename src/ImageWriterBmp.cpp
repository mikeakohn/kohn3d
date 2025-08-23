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
  ImageWriter(width, height),
  depth { depth },
  was_image_written { false },
  offset_to_image { 0 }
{
}

ImageWriterBmp::~ImageWriterBmp()
{
  finish();
}

void ImageWriterBmp::finish()
{
  long marker = ftell(fp);

  fseek(fp, 0x02, SEEK_SET);
  write_uint32((int)marker);

  fseek(fp, 0x0a, SEEK_SET);
  write_uint32((int)offset_to_image);

  fseek(fp, 0x22, SEEK_SET);
  write_uint32((int)(marker - offset_to_image));

  fseek(fp, marker, SEEK_SET);
}

int ImageWriterBmp::create_headers()
{
  putc('B', fp);
  putc('M', fp);
  write_uint32(0);
  write_uint16(0);
  write_uint16(0);
  write_uint32(0);

  write_uint32(40);
  write_uint32(width);
  write_uint32(height);
  write_uint16(1);
  write_uint16(depth);
  write_uint32(0);
  write_uint32(0);
  write_uint32(0);
  write_uint32(0);

  if (depth == 8)
  {
    write_uint32(max_colors);
    write_uint32(max_colors);
  }
    else
  {
    write_uint32(0);
    write_uint32(0);
  }

  for (int n = 0; n < max_colors; n++)
  {
    putc(palette[n] & 0xff, fp);
    putc((palette[n] >> 8) & 0xff, fp);
    putc((palette[n] >> 16) & 0xff, fp);
    putc(0, fp);
  }

  offset_to_image = ftell(fp);

  return 0;
}

int ImageWriterBmp::add_frame(uint8_t *image, uint32_t *color_table)
{
  int padding;
  int x, y, n;

  if (was_image_written) { return 0; }

  if (depth == 8)
  {
    padding = (4 - (width % 4)) & 0x3;

    for (y = height - 1; y >= 0; y--)
    {
      for (x = 0; x < width; x++)
      {
        putc(image[(y * width) + x], fp);
      }

      for (n = 0; n < padding; n++) { putc(0, fp); }
    }
  }
    else
  {
    uint32_t *image32 = (uint32_t *)image;

    padding = (4 - ((width * 3) % 4)) & 0x3;

    for (y = height - 1; y >= 0; y--)
    {
      for (x = 0; x < width; x++)
      {
        uint32_t color = image32[(y * width) + x];
        putc(color & 0xff, fp);
        putc((color >> 8) & 0xff, fp);
        putc((color >> 16) & 0xff, fp);
      }

      for (n = 0; n < padding; n++) { putc(0, fp); }
    }
  }

  was_image_written = true;

  return 0;
}

