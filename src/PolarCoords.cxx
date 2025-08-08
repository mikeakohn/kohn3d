/*
  Kohn3D - GIF drawing library.

  Copyright 2023-2025 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "PolarCoords.h"

// double atan2(double y, double x);

#if 0
void FromXY(double &angle, int r, const int x, const int y)
{
  r = (int)sqrt((float)((x * x) + (y * y)));
  angle = atan2(y, x);
}
#endif

