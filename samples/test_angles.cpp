#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Kohn3D.h"
#include "Angle.h"
#include "PolarCoords.h"

int main(int argc, char *argv[])
{
  const int width = 800;
  const int height = 800;

  Kohn3D kohn3d(width, height, Kohn3D::FORMAT_BMP24);

  kohn3d.create("test.bmp");
  kohn3d.clear();
  kohn3d.init_end();

  Texture texture_hello;

  if (texture_hello.load("samples/assets/colors.bmp") != 0)
  {
    printf("Error loading hello GIF.\n");
    exit(1);
  }

  PolarCoords coords;

  // 1st image.

  const int length = 80;

  int center_x = width / 4;
  int center_y = height / 4;

  coords.set_center(center_x, center_y);
  coords.set_radius(length);

  int color;

  for (double d = 0; d < 360; d += 0.1)
  {
    coords.set_angle_degrees(d);

    if      (d < 90)  { color = 0xff0000; }
    else if (d < 180) { color = 0x00ff00; }
    else if (d < 270) { color = 0x0000ff; }
    else              { color = 0xffffff; }

    kohn3d.draw_pixel(coords, color);
  }

  printf("%.2f %.2f\n",   0.0, PolarCoords::to_radians(0.0));
  printf("%.2f %.2f\n",  90.0, PolarCoords::to_radians(90.0));
  printf("%.2f %.2f\n", 180.0, PolarCoords::to_radians(180.0));
  printf("%.2f %.2f\n", 270.0, PolarCoords::to_radians(270.0));
  printf("%.2f %.2f\n", 360.0, PolarCoords::to_radians(360.0));
  printf("%.2f %.2f\n", 450.0, PolarCoords::to_radians(450.0));

  printf("%.2f %.2f\n", 0.00, PolarCoords::to_degrees(0.00));
  printf("%.2f %.2f\n", 1.57, PolarCoords::to_degrees(1.57));
  printf("%.2f %.2f\n", 3.14, PolarCoords::to_degrees(3.14));
  printf("%.2f %.2f\n", 4.71, PolarCoords::to_degrees(4.71));
  printf("%.2f %.2f\n", 6.28, PolarCoords::to_degrees(6.28));
  printf("%.2f %.2f\n", 7.85, PolarCoords::to_degrees(7.85));

  int x, y;

  for (int d = 0; d < 360; d = d + 45)
  {
    coords.set_angle_degrees(d);
    coords.to_xy(x, y);
    printf("(%.2f, %d) -> (%d, %d)\n", coords.p, coords.r, x, y);
  }

  double p;
  int r;

  printf("center: (%d, %d)\n", center_x, center_y);

  x = center_x;
  y = center_y - 50;
  coords.from_xy_centered(p, r, x, y);
  printf("%f %f, %d -> (%d, %d)\n", p, PolarCoords::to_degrees(p), r, x, y);

  x = center_x + 50;
  y = center_y;
  coords.from_xy_centered(p, r, x, y);
  printf("%f %f, %d -> (%d, %d)\n", p, PolarCoords::to_degrees(p), r, x, y);

  x = center_x;
  y = center_y + 50;
  coords.from_xy_centered(p, r, x, y);
  printf("%f %f, %d -> (%d, %d)\n", p, PolarCoords::to_degrees(p), r, x, y);

  x = center_x - 50;
  y = center_y;
  coords.from_xy_centered(p, r, x, y);
  printf("%f %f, %d -> (%d, %d)\n", p, PolarCoords::to_degrees(p), r, x, y);

  // 2nd image.

  Angle angle;

  center_x = width - center_x;

  angle.set_center(center_x, center_y);
  angle.set_from_xy(center_x, center_y - 80, center_x + 80, center_y);
  //angle.set_from_xy(center_x + 80, center_y, center_x, center_y - 80);

  angle.dump();

  kohn3d.draw_line(angle.coords_0, 0x00ff00);
  kohn3d.draw_line(angle.coords_1, 0x0000ff);

  coords.set_center(center_x, center_y);

printf("FROM %.1f TO %.1f\n", angle.get_degrees_0(), angle.get_degrees_1());

  for (double p = angle.get_degrees_0(); p < angle.get_degrees_1(); p += 0.1)
  {
    int r = angle.compute_length_at_degrees(p);

    coords.set_angle_degrees(p);
    coords.r = r;

    kohn3d.draw_pixel(coords, 0xff0000);
  }

  // 3rd image.

  center_x = width / 4;
  center_y = height - (height / 4);

  angle.set_center(center_x, center_y);
  angle.set_from_xy(center_x, center_y + 45, center_x - 45, center_y - 80);

  angle.dump();

  kohn3d.draw_line(angle.coords_0, 0x00ff00);
  kohn3d.draw_line(angle.coords_1, 0x0000ff);

  coords.set_center(center_x, center_y);

printf("FROM %.1f TO %.1f\n", angle.get_degrees_0(), angle.get_degrees_1());

  for (double p = angle.get_degrees_0(); p < angle.get_degrees_1(); p += 0.1)
  {
    int r = angle.compute_length_at_degrees(p);

    coords.set_angle_degrees(p);
    coords.r = r;

    //kohn3d.draw_pixel(coords, 0xff00ff);
  }

{
  double p = angle.get_degrees_0();
  double dp = (angle.get_degrees_1() - angle.get_degrees_0()) / 10;

  for (int i = 0; i < 10; i++)
  {
    int ix, jy;
    int r = angle.compute_length_at_1(0, ix, jy);

    coords.set_angle_degrees(p);
    coords.r = r;

p = p + dp;

int xx, yy;

coords.to_xy(xx, yy);

printf("(%d, %d)  (%d, %d)\n", ix, jy, xx, yy);

    kohn3d.draw_pixel(coords, 0xffffff);
    //kohn3d.draw_pixel(center_x + ix, center_y + jy, 0xffffff);
    //kohn3d.draw_pixel(center_x + xx, center_y + yy, 0xffffff);
  }
}

  kohn3d.write_frame();
  kohn3d.finish();

  return 0;
}

