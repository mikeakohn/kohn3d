/*
  Kohn3D - GIF drawing library.

  Copyright 2023-2025 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Angle.h"

void Angle::dump()
{
  printf(" -- Angle --\n");
  //printf("  ratio_r=%.2f\n", ratio_r);
  //printf("  length_p=%.2f\n", length_p);
  printf("  delta_x=%.2f\n", delta_x);
  printf("  delta_y=%.2f\n", delta_y);
  printf("  delta_p=%.2f\n", delta_p);
  coords_0.dump();
  coords_1.dump();
}

