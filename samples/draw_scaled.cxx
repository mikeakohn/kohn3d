#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Kohn3D.h"

int main(int argc, char *argv[])
{
  Kohn3D kohn3d(800, 480, Kohn3D::FORMAT_AVI24);

  kohn3d.create("test.avi");
  kohn3d.clear();
  kohn3d.set_fps(30);
  kohn3d.init_end();

  Picture picture_hello;
  Picture picture_background;

  if (picture_hello.load("samples/assets/hello.gif") != 0)
  {
    printf("Error loading hello GIF.\n");
    exit(1);
  }

  if (picture_background.load("samples/assets/coins_640x408x24.bmp") != 0)
  {
    printf("Error loading hello GIF.\n");
    exit(1);
  }

  //float bg_r = 0;
  //int alpha = 0xff;
  //int alpha_dx = -5;

  int x, y;
  int width, height;

  width = picture_hello.get_width() / 5;
  height = picture_hello.get_height() / 5;
  x = (800 / 2) - (width / 2);
  y = (480 / 2) - (height / 2);

  for (int i = 0; i < 150; i++)
  {
    kohn3d.clear();

    kohn3d.draw_picture_high_quality(
      picture_hello,
      x,
      y,
      width,
      height);

    x -= 5;
    y -= 5;
    width += 10;
    height += 10;

    kohn3d.write_frame();
  }

  for (int i = 0; i < 150; i++)
  {
    kohn3d.clear();

    kohn3d.draw_picture(
      picture_hello,
      x,
      y,
      width,
      height);

    x += 5;
    y += 5;
    width -= 10;
    height -= 10;

    kohn3d.write_frame();
  }

  double scale = 0.1;

  for (int i = 0; i < 150; i++)
  {
    width = picture_background.get_width() * scale;
    height = picture_background.get_height() * scale;

    x = (800 / 2) - (width / 2);
    y = (480 / 2) - (height / 2);

    kohn3d.clear();

    kohn3d.draw_picture_high_quality(
      picture_background,
      x,
      y,
      width,
      height);

    scale += 0.03;

    kohn3d.write_frame();
  }

  for (int i = 0; i < 150; i++)
  {
    width = picture_background.get_width() * scale;
    height = picture_background.get_height() * scale;

    x = (800 / 2) - (width / 2);
    y = (480 / 2) - (height / 2);

    kohn3d.clear();

    kohn3d.draw_picture(
      picture_background,
      x,
      y,
      width,
      height);

    scale -= 0.03;

    kohn3d.write_frame();
  }

  kohn3d.finish();

  return 0;
}

