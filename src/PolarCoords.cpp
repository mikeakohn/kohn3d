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

void PolarCoords::dump()
{
  printf(" -- PolarCoords --\n");
  printf("  p=[ %.2f %.2f ]  r=%d\n", p, get_degrees(), r);
  printf("  center=(%d, %d)\n\n", center_x, center_y);
}

