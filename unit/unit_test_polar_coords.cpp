#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "PolarCoords.h"

#define TEST_INT(a, b) \
  if (a != b) \
  { \
    printf("Error %d != %d  -- %s:%d\n", a, b, __FILE__, __LINE__); \
    errors += 1; \
  }

#define TEST_DOUBLE(a, b) \
  if (a != b) \
  { \
    printf("Error %.1f != %.1f  -- %s:%d\n", a, b, __FILE__, __LINE__); \
    errors += 1; \
  }

int test_degrees(double p, int d)
{
  int errors = 0;
  int i = PolarCoords::to_degrees(p);

  TEST_INT(i, d);

  return errors;
}

int test_from_xy(int x0, int y0, double degrees, int r)
{
  int errors = 0;
  PolarCoords coords;

  coords.from_xy(x0, y0);

  //printf("Test (%d, %d)\n", x0, y0);

  TEST_INT(coords.r, r);
  TEST_DOUBLE(coords.get_degrees(), degrees);

  return errors;
}

int test_polar_coords(int x0, int y0)
{
  int errors = 0;

  PolarCoords coords;
  coords.from_xy(x0, y0);

  //printf("%.1f %d\n", coords.get_degrees(), coords.r);

  int x = 0, y = 0;
  coords.to_xy(x, y);
  //printf("(%d, %d)\n", x, y);

  TEST_INT(x, x0);
  TEST_INT(y, y0);

  return errors;
}

int main(int argc, char *argv[])
{
  int errors = 0;

  errors += test_degrees(M_PI / 2, 90);
  errors += test_degrees(M_PI, 180);
  errors += test_degrees(M_PI + (M_PI / 2), 270);
  errors += test_degrees(M_PI * 2, 0);

  errors += test_from_xy( 0, -10,   0, 10);
  errors += test_from_xy(10, -10,  45, 14);
  errors += test_from_xy(10,  10, 135, 14);

  errors += test_polar_coords(-99,  99);
  errors += test_polar_coords( 99, -99);
  errors += test_polar_coords( 99,  99);
  errors += test_polar_coords( 96,  45);
  errors += test_polar_coords(-96,  45);

  printf("Errors: %d  (%s)\n", errors, errors == 0 ? "PASS" : "FAIL");

  return 0;
}

