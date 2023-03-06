#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "Kohn3D.h"

int main(int argc, char *argv[])
{
  Kohn3D kohn3d(320, 240);

  kohn3d.create("test.gif");
  kohn3d.add_color(0x000000);
  kohn3d.add_color(0xff0000);
  kohn3d.add_color(0x00ff00);
  kohn3d.add_color(0x0000ff);
  kohn3d.add_color(0xff00ff);
  kohn3d.add_color(0xffff00);
  kohn3d.add_color(0xffffff);
  kohn3d.set_bg_color_index(0);
  kohn3d.set_delay(10);
  kohn3d.set_loop_count(Kohn3D::LOOP_INFINITE);
  kohn3d.init_end();

  float d;
  int i = 1;

  for (d = 0; d < 6.18; d += 0.206)
  {
    kohn3d.clear();

    kohn3d.draw_rect(  0, 150, 100, 239, 6,  100);
    kohn3d.draw_rect(220, 150, 319, 239, 6, -100);

    int x = 90 * cosf(d);
    int y = 90 * sinf(d);
    kohn3d.draw_line(160, 120, 0, 160 + x, 120 + y, 0, i);
    kohn3d.write_frame();

    i++;
    if (i == 6) { i = 1; }
  }

  kohn3d.finish();

  return 0;
}

