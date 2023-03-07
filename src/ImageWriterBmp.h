/*

  Kohn3D - GIF drawing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#ifndef IMAGE_WRITER_BMP_H
#define IMAGE_WRITER_BMP_H

#include <stdint.h>

#include "ImageWriter.h"

class ImageWriterBmp : public ImageWriter
{
public:
  ImageWriterBmp(int width, int height, int depth);
  virtual ~ImageWriterBmp();

  virtual void finish();
  virtual int create_headers();
  virtual int add_frame(uint8_t *image, uint32_t *color_table);

private:

};

#endif

