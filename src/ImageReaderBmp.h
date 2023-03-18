/*

  Kohn3D - GIF drawing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#ifndef IMAGE_READER_BMP_H
#define IMAGE_READER_BMP_H

#include <stdint.h>

#include "ImageReader.h"

class ImageReaderBmp : public ImageReader
{
public:
  ImageReaderBmp();
  virtual ~ImageReaderBmp();

  virtual int load(const char *filename);

private:
  struct Header
  {
    char signature[2];
    uint32_t file_size;
    uint16_t unused_0;
    uint16_t unused_1;
    uint32_t data_offset;
  } header;

  struct InfoHeader
  {
    uint32_t header_size;
    int width;
    int height;
    uint16_t planes;
    uint16_t bits_per_pixel;
    uint32_t compression;
    uint32_t image_size;
    uint32_t vertical_res;
    uint32_t horizontal_res;
    uint32_t colors;
    uint32_t important_colors;
  } info_header;

  void set_pixel(uint32_t color, int x, int y)
  {
    if (do_upside_down)
    {
      ImageReader::set_pixel(color, x, height - 1 - y);
    }
      else
    {
      ImageReader::set_pixel(color, x, y);
    }
  }

  int read_header();
  int read_info_header();
  int load_4bit();
  int load_8bit();
  int load_rle4();
  int load_rle8();
  int load_24bit();
  int load_32bit();

  bool do_upside_down;
};

#endif

