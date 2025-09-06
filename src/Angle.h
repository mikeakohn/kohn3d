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
    start_x0 {   0 },
    start_y0 {   0 },
    start_x1 {   0 },
    start_y1 {   0 },
    //delta_x  { 0.0 },
    //delta_y  { 0.0 },
    slope    { 0.0 },
    b        { 0.0 }
  {
  }

  virtual ~Angle() { }

  void set_center(int x, int y)
  {
    coords_0.set_center(x, y);
    coords_1.set_center(x, y);
  }

  static double compute_slope(int x0, int y0, int x1, int y1)
  {
    double delta_x = x1 - x0;
    double delta_y = y1 - y0;

    if (delta_x == 0) { delta_x = 0.0000001; }

    return delta_y / delta_x;
  }

  static double compute_b(int x0, int y0, double slope)
  {
    return (double)y0 - slope * (double)x0;
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

    coords_0.to_xy(start_x0, start_y0);
    coords_1.to_xy(start_x1, start_y1);

    //delta_x = start_x1 - start_x0;
    //delta_y = start_y1 - start_y0;
    //if (delta_x == 0) { delta_x = 0.0000001; }

    // y = mx + b
    // y - mx = b
    slope = compute_slope(start_x0, start_y0, start_x1, start_y1);
    b = compute_b(start_x0, start_y0, slope);
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

  int compute_length_at(double p)
  {
    PolarCoords coords = coords_0;
    coords.p = p;

    int x0 = 0, y0 = 0, x1, y1;

    coords.to_xy(x1, y1);

    double slope_0 = compute_slope(x0, y0, x1, y1);
    double b0 = compute_b(x0, y0, slope);

    double x = (b0 - b) / (slope - slope_0);
    double y = x * slope_0 + b0;

    //double t = x * slope + b;
    //printf("(%d, %d) and (%d, %d)\n", (int)x, (int)y, (int)x, (int)t);

    coords.from_xy((int)x, (int)y);

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

  int start_x0;
  int start_y0;
  int start_x1;
  int start_y1;

  //double delta_x;
  //double delta_y;
  double slope;
  double b;
};

#endif

