/*

  Kohn3D - GIF drawing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ImageReader.h"

ImageReader::ImageReader() :
  image { nullptr },
  width { 0 },
  height { 0 }
{
}

ImageReader::~ImageReader()
{
  if (image != nullptr) { free(image); }
  image = nullptr;
}

