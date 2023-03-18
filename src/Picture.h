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

  uint32_t *get_data() { return data; }
  int get_width() { return width; }
  int get_height() { return height; }

private:
  uint32_t *data;
  int width;
  int height;

};

#endif

