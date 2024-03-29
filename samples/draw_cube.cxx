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
  kohn3d.add_color(0xff00ff);
  kohn3d.add_color(0x00ffff);
  kohn3d.add_color(0xffff00);
  kohn3d.set_bg_color_index(0);
  //kohn3d.set_delay(10);
  //kohn3d.set_loop_count(Kohn3D::LOOP_INFINITE);
  kohn3d.init_end();

  Kohn3D::Triangle triangles[12];

  // Blue.
  triangles[0].x0 = -50; triangles[1].x0 =  50;
  triangles[0].y0 = -50; triangles[1].y0 =  50;
  triangles[0].z0 =  50; triangles[1].z0 =  50;
  triangles[0].x1 = -50; triangles[1].x1 =  50;
  triangles[0].y1 =  50; triangles[1].y1 = -50;
  triangles[0].z1 =  50; triangles[1].z1 =  50;
  triangles[0].x2 =  50; triangles[1].x2 = -50;
  triangles[0].y2 = -50; triangles[1].y2 =  50;
  triangles[0].z2 =  50; triangles[1].z2 =  50;

  // Green.
  triangles[2] = triangles[0];
  triangles[2].z0 = -50;
  triangles[2].z1 = -50;
  triangles[2].z2 = -50;
  triangles[3] = triangles[1];
  triangles[3].z0 = -50;
  triangles[3].z1 = -50;
  triangles[3].z2 = -50;

  // Red.
  triangles[4].x0 =  50; triangles[5].x0 =  50;
  triangles[4].y0 = -50; triangles[5].y0 =  50;
  triangles[4].z0 = -50; triangles[5].z0 =  50;
  triangles[4].x1 =  50; triangles[5].x1 =  50;
  triangles[4].y1 =  50; triangles[5].y1 = -50;
  triangles[4].z1 = -50; triangles[5].z1 =  50;
  triangles[4].x2 =  50; triangles[5].x2 =  50;
  triangles[4].y2 = -50; triangles[5].y2 =  50;
  triangles[4].z2 =  50; triangles[5].z2 = -50;

  // Purple.
  triangles[6] = triangles[4];
  triangles[6].x0 = -50;
  triangles[6].x1 = -50;
  triangles[6].x2 = -50;
  triangles[7] = triangles[5];
  triangles[7].x0 = -50;
  triangles[7].x1 = -50;
  triangles[7].x2 = -50;

  // Cyan.
  triangles[8].x0 = -50; triangles[9].x0 =  50;
  triangles[8].y0 =  50; triangles[9].y0 =  50;
  triangles[8].z0 = -50; triangles[9].z0 =  50;
  triangles[8].x1 = -50; triangles[9].x1 =  50;
  triangles[8].y1 =  50; triangles[9].y1 =  50;
  triangles[8].z1 =  50; triangles[9].z1 = -50;
  triangles[8].x2 =  50; triangles[9].x2 = -50;
  triangles[8].y2 =  50; triangles[9].y2 =  50;
  triangles[8].z2 = -50; triangles[9].z2 =  50;

  // Yellow.
  triangles[10] = triangles[8];
  triangles[10].y0 = -50;
  triangles[10].y1 = -50;
  triangles[10].y2 = -50;
  triangles[11] = triangles[9];
  triangles[11].y0 = -50;
  triangles[11].y1 = -50;
  triangles[11].y2 = -50;

  Kohn3D::Rotation rotation;

  for (float r = 1.5; r < 6.18; r += 6.18 / 30)
  {
    rotation.rz = r;
    rotation.ry = r + 2;
    rotation.rx = r + 3;

    kohn3d.clear();

    for (int n = 0; n < 12; n++)
    {
      kohn3d.draw_triangle(triangles[n], rotation, 160, 120, 0, (n / 2) + 1);
    }

    kohn3d.write_frame();
break;
  }

  kohn3d.finish();

  return 0;
}

