#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Kohn3D.h"

int main(int argc, char *argv[])
{
  const int width = 800;
  const int height = 480;

  Kohn3D kohn3d(width, height, Kohn3D::FORMAT_AVI24);

  kohn3d.create("test.avi");
  kohn3d.clear();
  kohn3d.set_fps(30);
  kohn3d.init_end();

  Texture texture_coins;
  Texture texture_hello;

  if (texture_coins.load("samples/assets/coins_640x408x24.bmp") != 0)
  {
    printf("Error loading background BMP.\n");
    exit(1);
  }

  if (texture_hello.load("samples/assets/hello.gif") != 0)
  {
    printf("Error loading hello GIF.\n");
    exit(1);
  }

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

  texture_coins.set_scale(
    triangle.x0,
    triangle.y0,
    triangle.x1,
    triangle.y1,
    triangle.x2,
    triangle.y2);

  Kohn3D::Rotation rotation;

  //float bg_r = 0;

  rotation.rz = 0;
  rotation.ry = 0;
  rotation.rx = 0;

  for (float r = 0; r < 6.18 * 3; r += 6.18 / 120)
  {
#if 0
    rotation.rz = r;
    rotation.ry = r + 2;
    rotation.rx = r + 3;
#endif

    kohn3d.clear();

    //bg_r += 0.1;

    kohn3d.draw_triangle(
      triangle,
      rotation,
      300,
      300,
      0,
      0xff0000);

    kohn3d.write_frame();
  }

  kohn3d.finish();

  return 0;
}

