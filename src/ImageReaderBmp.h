/*

  Kohn3D - GIF drawing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#ifndef IMAGE_READER_BMP_H
#define IMAGE_READER_BMP_H

#include <stdint.h>

#include "ImageReader.h"

class ImageReaderBmp : public ImageReader
{
public:
  ImageReaderBmp();
  virtual ~ImageReaderBmp();

  virtual int load(const char *filename);

};

#endif

