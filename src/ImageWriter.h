/*

  Kohn3D - GIF drawing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#ifndef IMAGE_WRITER_H
#define IMAGE_WRITER_H

#include <stdint.h>

class ImageWriter
{
public:
  ImageWriter(int width, int height);
  virtual ~ImageWriter();

  int create(const char *filename);

  virtual void finish() = 0;

  void set_palette(uint32_t *color_table, int length);
  void set_bg_color_index(uint8_t value) { bg_color_index = value; }

  void set_transparent_color_index(uint8_t value)
  {
    transparent_color_index = value;
    do_transparency = true;
  }

  void set_delay(int value) { delay = value; }
  void set_loop_count(int value) { loop_count = value; }

  virtual int create_headers() = 0;
  virtual int add_frame(uint8_t *image, uint32_t *color_table) = 0;

protected:
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

  void write_uint32(int value)
  {
    putc(value & 0xff, fp);
    putc((value >> 8) & 0xff, fp);
    putc((value >> 16) & 0xff, fp);
    putc((value >> 24) & 0xff, fp);
  }

  FILE *fp;

  uint32_t palette[256];
  int width, height;
  int max_colors;
  uint8_t bg_color_index;
  uint8_t transparent_color_index;
  bool do_transparency;
  int delay;
  int loop_count;

private:
};

#endif

