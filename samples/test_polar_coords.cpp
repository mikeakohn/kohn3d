#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Kohn3D.h"

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

  const int length = 80;
  const int center_x = width / 4;
  const int center_y = height / 4;

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

  kohn3d.write_frame();
  kohn3d.finish();

  return 0;
}

