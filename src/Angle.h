/*

  Kohn3D - GIF drawing library.

  Copyright 2023-2025 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#ifndef ANGLE_H
#define ANGLE_H

#include <stdint.h>
#include <math.h>

#include "PolarCoords.h"

class Angle
{
public:
  Angle() :
    scale_p  { 0.0 },
    scale_r0 { 0.0 },
    scale_r1 { 0.0 },
    delta_x  { 0.0 },
    delta_y  { 0.0 }
  {
  }

  virtual ~Angle() { }

  void set_center(int x, int y)
  {
    coords_0.set_center(x, y);
    coords_1.set_center(x, y);
  }

  void set_from_xy(const int x0, const int y0, const int x1, const int y1)
  {
    coords_0.from_xy_centered(x0, y0);
    coords_1.from_xy_centered(x1, y1);

    //ratio_r = (float)coords_0.r / (float)coords_1.r;
    //length_p = fabs(coords_0.p - coords_1.p);
    delta_r = coords_1.r - coords_0.r;
    delta_p = coords_1.p - coords_0.p;

    if (delta_p == 0) { delta_p = 0.0000001; }

    delta_x = x1 - x0;
    delta_y = y1 - y0;
  }

  double get_radians_0()
  {
    return coords_0.get_radians();
  }

  double get_radians_1()
  {
    return coords_1.get_radians();
  }

  double get_degrees_0()
  {
    return coords_0.get_degrees();
  }

  double get_degrees_1()
  {
    return coords_1.get_degrees();
  }

  int compute_length_at_1(double p, int &i, int &j)
  {
    //double dp = p - coords_0.p;

    int x0, y0;
    coords_0.to_xy(x0, y0);

static int count = 0;
#if 0
printf("%.1f   (%d, %d)\n", (float)count / 10.0, x0, y0);
printf("%.1f, %.1f\n",
  (float)x0 + (delta_x * ((float)count / 10.0)),
  (float)y0 + (delta_y * ((float)count / 10.0))
);
#endif

    int x = (float)x0 + (delta_x * ((float)count / 10.0));
    int y = (float)y0 + (delta_y * ((float)count / 10.0));

count = count + 1;

    i = x;
    j = y;

    PolarCoords coords = coords_0;
    coords.from_xy(x, y);

    return coords.r;
  }

  int compute_length_at(double p)
  {
    double dp = p - coords_0.p;

    int x0, y0;

    coords_0.to_xy_centered(x0, y0);

//printf("%.2f  %d\n", coords_0.p, coords_0.r);
//printf("%.3f   (%d, %d)\n", dp /delta_p, x0, y0);

    int x = x0 + delta_x * (dp / delta_p);
    int y = y0 + delta_y * (dp / delta_p);

    PolarCoords coords = coords_0;
    coords.from_xy_centered(x, y);
//coords.dump();

    return coords.r;
  }

  int compute_length_at_degrees(double p)
  {
    return compute_length_at(PolarCoords::to_radians(p));
  }

  void dump();

  PolarCoords coords_0;
  PolarCoords coords_1;

private:
  //int center_x, center_y;
  double scale_p;
  double scale_r0;
  double scale_r1;

  //double ratio_r;
  //double length_p;
  double delta_p;
  double delta_r;

  double delta_x;
  double delta_y;

};

#endif

