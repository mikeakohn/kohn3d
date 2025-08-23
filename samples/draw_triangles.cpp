#include <stdio.h>
#include <stdlib.h>

#include "Kohn3D.h"

int main(int argc, char *argv[])
{
  Kohn3D kohn3d(320, 240, Kohn3D::FORMAT_GIF);

  kohn3d.create("test.gif");
  kohn3d.clear();
  kohn3d.add_color(0x000000);
  kohn3d.add_color(0x0000ff);
  kohn3d.add_color(0x00ff00);
  kohn3d.add_color(0xff0000);
  kohn3d.set_bg_color_index(0);
  kohn3d.set_delay(10);
  kohn3d.set_loop_count(Kohn3D::LOOP_INFINITE);
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

  for (float r = 0; r < 6.18; r += 6.18 / 30)
  {
    rotation_0.rz = r;
    rotation_1.ry = r;

    kohn3d.clear();
    kohn3d.draw_triangle(triangle, rotation_0, 160, 120, 0, 2);
    kohn3d.draw_triangle(triangle, rotation_1, 190, 120, 0, 1);
    kohn3d.draw_triangle(triangle_2, 60, 60, 3);
    kohn3d.write_frame();
  }

  kohn3d.finish();

  return 0;
}

