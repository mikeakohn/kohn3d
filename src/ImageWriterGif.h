/*

  Kohn3D - GIF drawing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#ifndef IMAGE_WRITER_GIF_H
#define IMAGE_WRITER_GIF_H

#include <stdint.h>

#include "ImageWriter.h"

class ImageWriterGif : public ImageWriter
{
public:
  ImageWriterGif(int width, int height);
  virtual ~ImageWriterGif();

  virtual void finish();
  virtual int create_headers();
  virtual int add_frame(uint8_t *image, uint32_t *color_table);

private:
  struct GifHeader
  {
    char version[6];
    uint16_t width;
    uint16_t height;
    uint8_t resolution_flag;
    uint8_t bgcolor_index;
    uint8_t aspect;
  };

  struct ImageDescriptorBlock
  {
    uint8_t image_separator_header;
    int16_t coordinate_left_border;
    int16_t coordinate_top_border;
    int16_t width;
    int16_t height;
    uint8_t flags;
  };

  struct Node
  {
    int prev;
    uint8_t color;
  };

  struct CompressNode
  {
    int down;
    int right;
    uint8_t color;
  };

  struct BitStream
  {
    BitStream() : data{0}, bitptr{0} { }

    int size() { return bitptr; }
    void dump() { printf("--> data=0x%08x bitptr=%d\n", data, bitptr); }

    uint8_t get_byte()
    {
      uint8_t next = data & 0xff;
      data = data >> 8;
      bitptr -= 8;
      return next;
    }

    void append(int value, int size)
    {
      data |= value << bitptr;
      bitptr += size;
    }

    uint32_t data;
    int bitptr;
  };

  int compute_bits_per_pixel(int max_colors);

  GifHeader gif_header;
};

#endif

