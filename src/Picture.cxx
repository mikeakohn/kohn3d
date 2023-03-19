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

