Kohn3D
======

Kohn3D is a C++ library for drawing pictures to create GIFs, animated GIFs,
BMPs, avi, etc.

Still in the beginning stages.

https://www.mikekohn.net/

C++ API
=======

To build the library and sample programs, from the root directory type:

    make
    make sample

Then there should be a libkohn3d.so and the sample programs in the
root dir. From there type:

    export LD_LIBRARY_PATH=.
    ./draw_cube

This will tell the OS to use the current directory to search for
library files and ./draw_cube will run the draw_cube.cxx program
which will create test.gif. This will be a rotating cube.

The sample apps should give enough info how to use the library.
All of the available functions are:

    Kohn3D(int width, int height);
    ~Kohn3D();

    int create(const char *filename);
    void finish();

    // The following methods must be called before init_end();
    int add_color(int value);
    void set_color(int index, int value);
    void set_bg_color_index(uint8_t value);
    void set_transparent_color_index(uint8_t value);

    // Delay value is 100ths of a second.
    void set_delay(int value);

    // Set frame rate (for AVI file).
    void set_fps(int value);

    static const int LOOP_INFINITE = 0;
    void set_loop_count(int value);

    void init_end();
    uint8_t *get_picture();
    void clear();

    void draw_pixel(int x, int y, int color_index);
    void draw_pixel(int x, int y, int color_index, int z)

    void draw_line(int x0, int y0, int x1, int y1, int color_index);
    void draw_line(int x0, int y0, int z0, int x1, int y1, int z1, int color_index);
    void draw_rect(int x0, int y0, int x1, int y1, int color_index);
    void draw_rect(int x0, int y0, int x1, int y1, int color_index, int z);

    struct Triangle
    {
      int x0, y0, z0;
      int x1, y1, z1;
      int x2, y2, z2;
    };

    struct Rotation
    {
      Rotation() : rx { 0 }, ry { 0 }, rz { 0 } { }
      float rx, ry, rz;
    };

    void load_triangle(Triangle &triangle, const int coords[]);
    void draw_triangle(const Triangle &triangle, int x, int y, int color_index);
    void draw_triangle(const Triangle &triangle, int x, int y, int z, int color_index);

    void draw_triangle(
      const Triangle &triangle,
      const Rotation &rotation,
      int x, int y, int z,
      int color_index);

    void write_frame();

Python API
==========

To build the Python module, the main libkohn3d.so must be built
first. To build the module the Python dev files must be installed
otherwise the compiler won't be able to find Python.h. On Ubuntu,
this can be done with:

    sudo apt install python3-dev

After that type:

    make
    cd python
    make
    python3 draw_triangles.py

The draw_triangles.py script will create test_py.gif. The full API is
pretty much the same as the C API:

    gif = create(filename, width, height);
    finish(gif);

    # The following functions must be called before init_end();
    add_color(gif, value);
    set_color(gif, index, value);
    set_bg_color_index(gif, value);
    set_transparent_color_index(gif, value);

    # Delay value is 100ths of a second.
    set_delay(gif, value);

    # LOOP_INFINITE = 0;
    set_loop_count(gif, value);

    init_end(gif);
    clear(gif);

    draw_pixel(gif, x, int y, int color_index);
    draw_pixel(gif, x, int y, int color_index, z)

    draw_line(gif, x0, y0, x1, y1, color_index);
    draw_line(gif, x0, y0, z0, x1, y1, z1, color_index);
    draw_rect(gif, x0, y0, x1, y1, color_index);
    draw_rect(gif, x0, y0, x1, y1, color_index, z);

    # triangle should be a list of 9 int points [ x0, y0, z0, x1, xy, z1, x2, y2, z2 ]
    # rotation should be a list of 3 float values [ rx, ry, rz ]
    draw_triangle(gif, triangle, x, y, color_index);
    draw_triangle(gif, triangle, x, y, z, color_index);

    draw_triangle(gif, triangle, rotation, x, y, z, color_index);
    void write_frame(gif);

TODO
====

1. STL file reading.
2. Projections.
3. Scaled draw_picture().
4. Textures in triangles.
5. AVI files are upside down.

