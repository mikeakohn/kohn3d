/*

  Kohn3D - GIF drawing library.

  Copyright 2023-2025 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#ifndef POLAR_COORDS_H
#define POLAR_COORDS_H

#include <stdint.h>
#include <math.h>

class PolarCoords
{
public:
  PolarCoords() :
    p { 0.0 },
    r {   0 }
  {
  }

  virtual ~PolarCoords() { }

  static void from_xy(double &p, int &r, const int x, const int y)
  {
    r = (int)sqrt((float)((x * x) + (y * y)));
    p = atan2(y, x);
  }

  static void to_xy(int &x, int &y, const double p, const int r)
  {
    x = r * cos(p);
    y = r * sin(p);
  }

  void from_xy(const int x, const int y)
  {
    from_xy(p, r, x, y);
  }

  void to_xy(int &x, int &y)
  {
    to_xy(x, y, p, r);
  }

  double get_degrees() { return p * 180 / 3.14; }

  double p;
  int r;

private:

};

#endif

