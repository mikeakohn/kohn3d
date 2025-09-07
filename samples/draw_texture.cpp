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

  triangle.set_vertex_0(0, -150, 0);
  triangle.set_vertex_1(0,    0, 0);
  triangle.set_vertex_2(150,  0, 0);

  Kohn3D::Rotation rotation;

  rotation.rz = 0;
  rotation.ry = 0;
  rotation.rx = 0;

  texture_hello.set_coords(0.0, 0.0,  0.0, 1.0,  1.0, 1.0);

  for (float r = 0; r < 6.18 * 3; r += 6.18 / 120)
  {
    rotation.rz = r;

    kohn3d.clear();

    kohn3d.draw_triangle(
      triangle,
      rotation,
      300,
      300,
      0,
      0x00ff00);

    kohn3d.write_frame();
  }

  for (float r = 0; r < 6.18 * 3; r += 6.18 / 120)
  {
    rotation.rz = r;

    kohn3d.clear();

    kohn3d.draw_triangle(
      triangle,
      rotation,
      300,
      300,
      0,
      texture_hello);

    kohn3d.write_frame();
  }

  kohn3d.finish();

  return 0;
}

