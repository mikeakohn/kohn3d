/*

  Kohn3D - GIF drawing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
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

  uint32_t global_color_table[256];

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
    uint8_t transparent_index;
    uint8_t terminator;
  } graphics_control;

  struct ApplicationExtension : public Extension
  {
    char identifier[8];
    char authentication_code[3];
    // Application data and terminator;
  } application_extension;

  int read_file();
  int read_header();
  int read_image_descriptor();
  int read_extension();
  int read_plain_text(Extension &extension);
  int read_graphics_control(Extension &extension);
  int read_application_extension(Extension &extension);

  int global_colors;
  int local_colors;
  int loop_count;
  uint32_t local_palette[256];
};

#endif

