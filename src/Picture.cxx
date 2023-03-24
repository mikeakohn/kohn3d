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
#include "ImageReaderGif.h"
#include "Picture.h"

Picture::Picture() :
  data { nullptr },
  width { 0 },
  height { 0 }
{
}

Picture::~Picture()
{
  if (data != nullptr) { free(data); }
  data = nullptr;
}

int Picture::create(int width, int height)
{
  this->width = width;
  this->height = height;

  data = (uint32_t *)malloc(width * height * sizeof(uint32_t));

  return 0;
}

int Picture::load_bmp(const char *filename)
{
  ImageReaderBmp image_reader;

  int result = image_reader.load(filename);
  set_data(image_reader.get_image());
  width = image_reader.get_width();
  height = image_reader.get_height();

  return result;
}

int Picture::load_gif(const char *filename)
{
  ImageReaderGif image_reader;

  int result = image_reader.load(filename);
  set_data(image_reader.get_image());
  width = image_reader.get_width();
  height = image_reader.get_height();

  return result;
}

void Picture::set_color_transparent(uint32_t value)
{
  int pixel_count = get_pixel_count();

  for (int n = 0; n < pixel_count; n++)
  {
    uint32_t color = get_pixel(n);

    if (color != value) { continue; }

    color = color & 0xffffff;
    set_pixel(n, color);
  }
}

void Picture::update_alpha(uint8_t value)
{
  int pixel_count = get_pixel_count();
  uint32_t alpha_mask = value << 24;

  for (int n = 0; n < pixel_count; n++)
  {
    uint32_t color = get_pixel(n);

    color = color & 0xffffff;
    color |= alpha_mask;
    set_pixel(n, color);
  }
}

void Picture::update_alpha(uint8_t value, uint32_t ignore_color)
{
  int pixel_count = get_pixel_count();
  uint32_t alpha_mask = value << 24;

  for (int n = 0; n < pixel_count; n++)
  {
    uint32_t color = get_pixel(n);

    if (color != ignore_color)
    {
      color = color & 0xffffff;
      color |= alpha_mask;
      set_pixel(n, color);
    }
  }
}

