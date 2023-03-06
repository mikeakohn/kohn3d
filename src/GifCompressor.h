/*

  GIF Factory - GIF drawing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#ifndef GIF_COMPRESSOR_H
#define GIF_COMPRESSOR_H

#include <stdint.h>

class GifCompressor
{
public:
  GifCompressor();
  ~GifCompressor();

  int create(const char *filename);
  void finish();
  void set_width(int value) { gif_header.width = value; }
  void set_height(int value) { gif_header.height = value; }
  void set_palette(uint32_t *color_table, int length);
  void set_bg_color_index(uint8_t value) { bg_color_index = value; }

  void set_transparent_color_index(uint8_t value)
  {
    transparent_color_index = value;
    do_transparency = true;
  }

  void set_delay(int value) { delay = value; }
  void set_loop_count(int value) { loop_count = value; }
  int create_headers();
  int compress(uint8_t *image, uint32_t *color_table);

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

  void set_uint16(uint8_t *data, int ptr, uint16_t value)
  {
    *(data + ptr + 0) = value & 0xff;
    *(data + ptr + 1) = value >> 8;
  }

  void write_uint16(int value)
  {
    putc(value & 0xff, fp);
    putc((value >> 8) & 0xff, fp);
  }

  inline int compute_bits_per_pixel(int max_colors);

  GifHeader gif_header;
  FILE *fp;
  uint8_t data[1024];
  //int data_length;
  //int gif_length;
  uint32_t gif_palette[256];
  int max_colors;
  uint8_t bg_color_index;
  uint8_t transparent_color_index;
  bool do_transparency;
  int delay;
  int loop_count;
};

#endif

