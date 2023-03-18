/*

  Kohn3D - GIF drawing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#ifndef IMAGE_READER_H
#define IMAGE_READER_H

#include <stdint.h>

class ImageReader
{
public:
  ImageReader();
  virtual ~ImageReader();

  virtual int load(const char *filename) = 0;

  int get_width() { return width; }
  int get_height() { return height; }
  uint32_t *get_image() { return image; }

protected:
  uint32_t read_uint16()
  {
    uint32_t n;

    n = getc(fp);
    n |= getc(fp) << 8;

    return n;
  }

  uint32_t read_uint32(int value)
  {
    uint32_t n;

    n = getc(fp);
    n |= getc(fp) << 8;
    n |= getc(fp) << 16;
    n |= getc(fp) << 24;

    return n;
  }

  FILE *fp;

  uint32_t *image;
  int width, height;
  uint32_t palette[256];

};

#endif

