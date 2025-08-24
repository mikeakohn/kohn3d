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

  void set_center(int x, int y)
  {
    center_x = x;
    center_y = y;
  }

  void set_angle(double value) { p = value; }
  void set_radius(int value)   { r = value; }

  void set_angle_degrees(double value) { p = to_radians(value); }

  static void from_xy(double &p, int &r, const int x, const int y)
  {
    r = (int)sqrt((float)((x * x) + (y * y)));
    p = atan2(y, x);

    p = fmod(p - (M_PI / 2), M_PI * 2);
  }

  static void to_xy(int &x, int &y, const double p, const int r)
  {
    double p0 = fmod(p - (M_PI / 2), M_PI * 2);

    x = r * cos(p0);
    y = r * sin(p0);
  }

  void from_xy(const int x, const int y)
  {
    from_xy(p, r, x, y);
  }

  void to_xy(int &x, int &y)
  {
    to_xy(x, y, p, r);
  }

  void to_xy_centered(int &x, int &y) const
  {
    to_xy(x, y, p, r);

    x = center_x + x;
    y = center_y + y;
  }

  void from_xy_centered(double &p, int &r, const int x, const int y)
  {
    from_xy(p, r, center_x - x, center_y - y);
  }

  void from_xy_centered(const int x, const int y)
  {
    from_xy_centered(p, r, x, y);
  }

  static double to_degrees(double p)
  {
    p = fmod(p, (2 * M_PI));
    return p * 180 / M_PI;
  }

  double get_degrees() { return to_degrees(p); }

  static double to_radians(double d)
  {
    d = fmod(d, 360.0);
    return (d * (2 * M_PI)) / 360;
  }

  void dump();

  double p;
  int r;

private:
  int center_x, center_y;

};

#endif

