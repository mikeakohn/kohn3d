#include <stdio.h>
#include <stdlib.h>

#include "Kohn3D.h"

int main(int argc, char *argv[])
{
  Kohn3D kohn3d(320, 240, Kohn3D::FORMAT_BMP24);

  kohn3d.create("test.bmp");
  kohn3d.clear();
  kohn3d.init_end();

  Kohn3D::Triangle triangle;
  triangle.x0 = 0;
  triangle.y0 = -50;
  triangle.z0 = 0;
  triangle.x1 = -50;
  triangle.y1 = 50;
  triangle.z1 = 0;
  triangle.x2 = 50;
  triangle.y2 = 60;
  triangle.z2 = 0;

  Kohn3D::Triangle triangle_2;
  triangle_2.x0 = -30;
  triangle_2.y0 = -30;
  triangle_2.z0 = 0;
  triangle_2.x1 = 30;
  triangle_2.y1 = -30;
  triangle_2.z1 = 0;
  triangle_2.x2 = 0;
  triangle_2.y2 = 10;
  triangle_2.z2 = 0;

  Kohn3D::Rotation rotation_0;
  Kohn3D::Rotation rotation_1;

  uint32_t colors[] = { 0x00ff00ff, 0xffff0000, 0xff0000ff };

  float r = 2.0;

  rotation_0.rz = r;
  rotation_1.ry = r;

  kohn3d.clear();
  kohn3d.draw_triangle(triangle, rotation_0, 160, 120, 0, colors);
  kohn3d.draw_triangle(triangle, rotation_1, 190, 120, 0, 0x00ff00);

  kohn3d.enable_alpha_blending(true);
  kohn3d.draw_triangle(triangle_2, 60, 60, 0, colors);
  kohn3d.enable_alpha_blending(false);

  kohn3d.write_frame();
  kohn3d.finish();

  return 0;
}

