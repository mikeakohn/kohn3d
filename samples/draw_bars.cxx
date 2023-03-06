#include <stdio.h>
#include <stdlib.h>

#include "Kohn3D.h"

int main(int argc, char *argv[])
{
  Kohn3D gif_factory(320, 240);

  gif_factory.create("test.gif");
  gif_factory.add_color(0x000000);
  gif_factory.add_color(0x000020);
  gif_factory.add_color(0x000060);
  gif_factory.add_color(0x0000a0);
  gif_factory.add_color(0x0000ff);
  gif_factory.set_bg_color_index(0);
  gif_factory.set_delay(1);
  gif_factory.set_loop_count(Kohn3D::LOOP_INFINITE);
  gif_factory.init_end();

  int y, i;

  for (y = 100; y < 130; y++)
  {
    gif_factory.clear();
    for (i = 1; i < 4; i++) { gif_factory.draw_line(0, y + i, 319, y + i, i); }
    for (i = 4; i > 0; i--) { gif_factory.draw_line(0, y + i, 319, y + i, i); }
    gif_factory.write_frame();
  }

  for (y = 130; y > 100; y--)
  {
    gif_factory.clear();
    for (i = 1; i < 4; i++) { gif_factory.draw_line(0, y + i, 319, y + i, i); }
    for (i = 4; i > 0; i--) { gif_factory.draw_line(0, y + i, 319, y + i, i); }
    gif_factory.write_frame();
  }

  gif_factory.finish();

  return 0;
}

