#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Kohn3D.h"

int main(int argc, char *argv[])
{
  Kohn3D gif_factory(320, 240);

  gif_factory.create("test.gif");
  gif_factory.add_color(0x000000);
  gif_factory.add_color(0xff0000);
  gif_factory.add_color(0x00ff00);
  gif_factory.add_color(0x0000ff);
  gif_factory.add_color(0xff00ff);
  gif_factory.add_color(0xffff00);
  gif_factory.add_color(0xffffff);
  gif_factory.set_bg_color_index(0);
  gif_factory.set_delay(10);
  gif_factory.set_loop_count(Kohn3D::LOOP_INFINITE);
  gif_factory.init_end();

  float d;
  int i = 1;

  for (d = 0; d < 6.18; d += 0.206)
  {
    gif_factory.clear();

    gif_factory.draw_rect(  0, 150, 100, 239, 6,  100);
    gif_factory.draw_rect(220, 150, 319, 239, 6, -100);

    int x = 90 * cosf(d);
    int y = 90 * sinf(d);
    gif_factory.draw_line(160, 120, 0, 160 + x, 120 + y, 0, i);
    gif_factory.write_frame();

    i++;
    if (i == 6) { i = 1; }
  }

  gif_factory.finish();

  return 0;
}

