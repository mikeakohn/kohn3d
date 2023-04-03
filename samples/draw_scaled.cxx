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
  //picture_background.create(640, 480);

  if (picture_hello.load_gif("samples/assets/hello.gif") != 0)
  {
    printf("Error loading hello GIF.\n");
    exit(1);
  }

  //float bg_r = 0;
  //int alpha = 0xff;
  //int alpha_dx = -5;
  int hello_width = 134 / 2;
  int hello_height = 50 / 2;
  int hello_x = (800 / 2) - (hello_width / 2);
  int hello_y = (480 / 2) - (hello_height / 2);

  for (float r = 0; r < 6.18 * 3; r += 6.18 / 120)
  {
    kohn3d.clear();

#if 0
    picture_hello.update_alpha(alpha, 0x00000000);

    alpha += alpha_dx;
    if (alpha < 50) { alpha_dx = 5; }
    if (alpha >= 255) { alpha_dx = -5; }
#endif

    //kohn3d.enable_alpha_blending(true);
    kohn3d.draw_picture_high_quality(
      picture_hello,
      hello_x,
      hello_y,
      hello_width,
      hello_height);
    //kohn3d.enable_alpha_blending(false);

    hello_x -= 2;
    hello_y -= 2;
    hello_width += 4;
    hello_height += 4;

    //bg_r += 0.1;

    kohn3d.write_frame();
  }

  kohn3d.finish();

  return 0;
}

