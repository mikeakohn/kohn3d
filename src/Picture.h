/*

  Kohn3D - GIF drawing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#ifndef PICTURE_H
#define PICTURE_H

#include <stdint.h>

class Picture
{
public:
  Picture();
  virtual ~Picture();

  int create(int width, int height);
  int load_bmp(const char *filename);
  int load_gif(const char *filename);

  uint32_t *get_data() { return data; }
  int get_width() { return width; }
  int get_height() { return height; }
  int get_pixel_count() { return width * height; }
  void set_color_transparent(uint32_t value);
  void update_alpha(uint8_t value);
  void update_alpha(uint8_t value, uint32_t ignore_color);

  void set_data(uint32_t *value)
  {
    if (data != nullptr) { free(data); }
    data = value;
  }

  uint32_t get_pixel(int x, int y)
  {
    if (x < 0 || x >= width) { return 0; }
    if (y < 0 || y >= height) { return 0; }

    return data[(y * width) + x];
  }

  void set_pixel(int x, int y, uint32_t color)
  {
    if (x < 0 || x >= width) { return; }
    if (y < 0 || y >= height) { return; }

    data[(y * width) + x] = color;
  }

  uint32_t get_pixel(int index)
  {
    if (index < 0 || index > width * height) { return 0; }

    return data[index];
  }

  void set_pixel(int index, uint32_t color)
  {
    if (index < 0 || index > width * height) { return; }

    data[index] = color;
  }

private:
  uint32_t *data;
  int width;
  int height;

};

#endif

