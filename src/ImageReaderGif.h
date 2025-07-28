/*

  Kohn3D - GIF drawing library.

  Copyright 2023-2025 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#ifndef IMAGE_READER_GIF_H
#define IMAGE_READER_GIF_H

#include <stdint.h>

#include "ImageReader.h"

class ImageReaderGif : public ImageReader
{
public:
  ImageReaderGif();
  virtual ~ImageReaderGif();

  virtual int load(const char *filename);

private:
  struct Header
  {
    char signature[6];
    uint16_t width;
    uint16_t height;
    uint8_t fields;
    uint8_t bg_color_index;
    uint8_t aspect_ratio;
  } header;

  //uint32_t global_color_table[256];

  struct ImageDescriptor
  {
    char separator;
    uint16_t left_position;
    uint16_t top_position;
    uint16_t width;
    uint16_t height;
    uint8_t fields;
  } image_descriptor;

  struct Extension
  {
    char introducer;
    uint8_t label;
    uint8_t block_size;
  };

  struct PlainTextExtension : public Extension
  {
    uint16_t text_grid_left;
    uint16_t text_grid_top;
    uint16_t text_grid_width;
    uint16_t text_grid_height;
    uint8_t cell_width;
    uint8_t cell_height;
    uint8_t fg_color_index;
    uint8_t bg_color_index;
    // Text data and terminator;
  } plain_text;

  struct GraphicsControlExtension : public Extension
  {
    uint8_t fields;
    uint16_t delay_time;
    uint32_t transparent_index;
    uint8_t terminator;
  } graphics_control;

  struct ApplicationExtension : public Extension
  {
    char identifier[8];
    char authentication_code[3];
    // Application data and terminator;
  } application_extension;

  struct Node
  {
    int prev;
    uint8_t color;
  };

  struct BitStream
  {
    BitStream() :
      holding { 0 },
      bitptr { 0 },
      code_size { 0 },
      data_count { 0 },
      data_next { 0 }
    {
    }

    void set_code_size(int code_size)
    {
      this->code_size = code_size;
      mask = (1 << code_size) - 1;
    }

    void inc_code_size()
    {
      code_size++;
      mask = (1 << code_size) - 1;
    }

    int read(FILE *fp, int data_count)
    {
      this->data_count = data_count;
      this->data_next = 0;
      if (fread(data, 1, data_count, fp) != (size_t)data_count) { return -1; }

      return 0;
    }

    int get_next()
    {
      while (bitptr < code_size)
      {
        if (data_next == data_count) { return -1; }

        holding |= data[data_next++] << bitptr;
        bitptr += 8;
      }

      int code = holding & mask;
      holding = holding >> code_size;
      bitptr -= code_size;

      return code;
    }

    uint32_t holding;
    int bitptr;
    uint32_t mask;
    int code_size;
    int data_count;
    int data_next;
    uint8_t data[256];
  };

  int read_file();
  int read_header();
  int read_image_descriptor();
  int read_extension();
  int read_plain_text(Extension &extension);
  int read_graphics_control(Extension &extension);
  int read_comment(Extension &extension);
  int read_application_extension(Extension &extension);
  int read_image();

  void set_pixel(int &x, int &y, uint32_t index)
  {
    uint32_t color = 0;

    if (index != graphics_control.transparent_index)
    {
      color =
        local_colors != 0 ?
        local_palette[index] : palette[index];
        color |= 0xff000000;
    }

    ImageReader::set_pixel(
      image_descriptor.left_position + x,
      image_descriptor.top_position + y,
      color);

    x++;

    if (x == image_descriptor.width)
    {
      x = 0;
      y++;
    }
  }

  int global_colors;
  int local_colors;
  int loop_count;
  uint32_t local_palette[256];
};

#endif

