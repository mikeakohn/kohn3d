/*

  Kohn3D - GIF drawing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ImageReaderBmp.h"

ImageReaderBmp::ImageReaderBmp() : ImageReader(), do_upside_down { false }
{
}

ImageReaderBmp::~ImageReaderBmp()
{
}

int ImageReaderBmp::load(const char *filename)
{
  fp = fopen(filename, "rb");

  if (fp == NULL) { return -2; }
  int result = read_file();
  fclose(fp);

  fp = NULL;

  return result;
}

int ImageReaderBmp::read_file()
{
  if (read_header() != 0) { return -1; }
  if (read_info_header() != 0) { return -1; }

  for (uint32_t n = 0; n < info_header.colors; n++)
  {
    uint32_t color = read_uint32();
    palette[n] = color;
  }

  const int length = width * height * sizeof(uint32_t);
  image = (uint32_t *)malloc(length);
  memset(image, 0, length);

  fseek(fp, header.data_offset, SEEK_SET);

  if (info_header.compression == 0)
  {
    switch (info_header.bits_per_pixel)
    {
      case 4: return load_4bit();
      case 8: return load_8bit();
      case 24: return load_24bit();
      case 32: return load_32bit();
      default: return -1;
    }
  }
    else
  if (info_header.compression == 1)
  {
    return load_rle8();
  }
    else
  if (info_header.compression == 2)
  {
    return load_rle4();
  }

  printf("Error: Unknown BMP format bits_per_pixel=%d, compression=%d\n",
    info_header.bits_per_pixel,
    info_header.compression);

  return -3;
}

int ImageReaderBmp::read_header()
{
  if (fread(header.signature, 1, 2, fp) != 2) { return -1; }

  header.file_size = read_uint32();
  header.unused_0 = read_uint16();
  header.unused_1 = read_uint16();
  header.data_offset = read_uint32();

  return 0;
}

int ImageReaderBmp::read_info_header()
{
  long marker = ftell(fp);

  info_header.header_size = read_uint32();
  info_header.width = read_uint32();
  info_header.height = read_uint32();
  info_header.planes = read_uint16();
  info_header.bits_per_pixel = read_uint16();
  info_header.compression = read_uint32();
  info_header.image_size = read_uint32();
  info_header.vertical_res = read_uint32();
  info_header.horizontal_res = read_uint32();
  info_header.colors = read_uint32();
  info_header.important_colors = read_uint32();

  width = info_header.width;

  if (info_header.height > 0)
  {
    height = info_header.height;
    do_upside_down = true;
  }
    else
  {
    height = -info_header.height;
  }

  long here = ftell(fp);

  if (marker + info_header.header_size != here)
  {
    fseek(fp, info_header.header_size - (here - marker), SEEK_CUR);
  }

  return 0;
}

int ImageReaderBmp::load_4bit()
{
  int x, y, w;
  w = width / 2;
  if ((width & 1) == 1) { w++; }

  const int padding = (4 - (w % 4)) & 0x3;

  for (y = 0; y < height; y++)
  {
    for (x = 0; x < width; x += 2)
    {
      int index = getc(fp);
      if (index > 0xff) { return -1; }

      set_pixel(x, y, palette[index >> 4]);
      set_pixel(x, y, palette[index & 0xf]);
    }

    for (int n = 0; n < padding; n++) { getc(fp); }
  }

  return 0;
}

int ImageReaderBmp::load_8bit()
{
  int x, y;
  const int padding = (4 - (width % 4)) & 0x3;

  for (y = 0; y < height; y++)
  {
    for (x = 0; x < width; x++)
    {
      int index = getc(fp);
      if (index > 0xff) { return -1; }

      set_pixel(x, y, palette[index]);
    }

    for (int n = 0; n < padding; n++) { getc(fp); }
  }

  return 0;
}

int ImageReaderBmp::load_rle4()
{
  return -1;
}

int ImageReaderBmp::load_rle8()
{
  int x, y;
  int ch, count, n;
  uint32_t color;

  x = 0;
  y = 0;

  while (true)
  {
    ch = getc(fp);
    if (ch == EOF) { return -1; }

    if (ch != 0)
    {
      count = ch;

      color = palette[getc(fp) & 0xff];

      for (n = 0; n < count; n++)
      {
        set_pixel(x, y, color);
        x++;

        // This shouldn't be needed?
        //if (x >= width) { x = 0; y++; }
      }
    }
      else
    {
      ch = getc(fp);
      if (ch == EOF) { return -1; }

      if (ch == 0)
      {
        // End of line.
        x = 0;
        y++;
      }
        else
      if (ch == 1)
      {
        // End of bitmap.
        break;
      }
        else
      if (ch == 2)
      {
        // Delta (2 bytes unsigned. x = x + byte0, y = y + byte1).
        int dx = getc(fp);
        int dy = getc(fp);

        if (dx == EOF) { return -1; }
        if (dy == EOF) { return -1; }

        x += dx;
        y += dy;
      }
        else
      {
        // Absolute mode.
        count = ch;

        for (n = 0; n < count; n++)
        {
          color = palette[getc(fp) & 0xff];

          set_pixel(x, y, color);
          x++;

          // This shouldn't be needed?
          //if (x >= width) { x = 0; y++; }
        }

        if ((count & 1) == 1) { getc(fp); }
      }
    }

  }

  return 0;
}

int ImageReaderBmp::load_24bit()
{
  int x, y;
  const int padding = (4 - ((width * 3) % 4)) & 0x3;

  for (y = 0; y < height; y++)
  {
    for (x = 0; x < width; x++)
    {
      uint32_t color;

      color = getc(fp);
      color |= getc(fp) << 8;
      color |= getc(fp) << 16;

      set_pixel(x, y, color);
    }

    for (int n = 0; n < padding; n++) { getc(fp); }
  }

  return 0;
}

int ImageReaderBmp::load_32bit()
{
  int x, y;

  for (y = 0; y < height; y++)
  {
    for (x = 0; x < width; x++)
    {
      set_pixel(x, y, read_uint32());
    }
  }

  return 0;
}

