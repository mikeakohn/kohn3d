#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Kohn3D.h"

int main(int argc, char *argv[])
{
  const int width = 800;
  const int height = 480;

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

  //for (int i = 1; i < height; i++) { kohn3d.draw_line(0, 0, i, i, 0xff0000); }

  Kohn3D::Triangle triangle;

  triangle.x0 = -50;
  triangle.y0 = -50;
  triangle.z0 =  0;
  triangle.x1 = -50;
  triangle.y1 =  50;
  triangle.z1 =  0;
  triangle.x2 =  100;
  triangle.y2 =  60;
  triangle.z2 =  0;

  Kohn3D::Rotation rotation;

  //float bg_r = 0;

  rotation.rz = 0;
  rotation.ry = 0;
  rotation.rx = 0;

  texture_hello.set_coords(0.0, 0.0,  0.0, 1.0,  1.0, 1.0);

  kohn3d.draw_triangle(
    triangle,
    rotation,
    300,
    300,
    0,
    texture_hello);

  kohn3d.write_frame();
  kohn3d.finish();

  return 0;
}

