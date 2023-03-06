#!/usr/bin/env python3

import gif_factory
import sys

gif = gif_factory.create("test_py.gif", 640, 480)

gif_factory.add_color(gif, 0x000000)
gif_factory.add_color(gif, 0x0000ff)
gif_factory.add_color(gif, 0x00ff00)
gif_factory.add_color(gif, 0xff0000)
gif_factory.add_color(gif, 0xff00ff)
gif_factory.add_color(gif, 0xffff00)
gif_factory.add_color(gif, 0xffffff)
gif_factory.init_end(gif)

triangle = [
  0, -50, 0,
 -50, 50, 0,
  50, 50, 0,
]

rotation = [ 2.2, 1.0, 0.0 ]

gif_factory.clear(gif)
gif_factory.draw_rect(gif, 50, 50, 100, 100, 1)
gif_factory.draw_triangle(gif, triangle, 300, 300, 2)
gif_factory.draw_triangle(gif, triangle, rotation, 300, 100, 10, 3)
gif_factory.write_frame(gif)
gif_factory.finish(gif)

print("finish")

