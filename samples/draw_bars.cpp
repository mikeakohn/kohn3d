#include <stdio.h>
#include <stdlib.h>

#include "Kohn3D.h"

int main(int argc, char *argv[])
{
  Kohn3D kohn3d(320, 240, Kohn3D::FORMAT_GIF);

  kohn3d.create("test.gif");
  kohn3d.add_color(0x000000);
  kohn3d.add_color(0x000020);
  kohn3d.add_color(0x000060);
  kohn3d.add_color(0x0000a0);
  kohn3d.add_color(0x0000ff);
  kohn3d.set_bg_color_index(0);
  kohn3d.set_delay(1);
  kohn3d.set_loop_count(Kohn3D::LOOP_INFINITE);
  kohn3d.init_end();

  int y, i;

  for (y = 100; y < 130; y++)
  {
    kohn3d.clear();
    for (i = 1; i < 4; i++) { kohn3d.draw_line(0, y + i, 319, y + i, i); }
    for (i = 4; i > 0; i--) { kohn3d.draw_line(0, y + i, 319, y + i, i); }
    kohn3d.write_frame();
  }

  for (y = 130; y > 100; y--)
  {
    kohn3d.clear();
    for (i = 1; i < 4; i++) { kohn3d.draw_line(0, y + i, 319, y + i, i); }
    for (i = 4; i > 0; i--) { kohn3d.draw_line(0, y + i, 319, y + i, i); }
    kohn3d.write_frame();
  }

  kohn3d.finish();

  return 0;
}

