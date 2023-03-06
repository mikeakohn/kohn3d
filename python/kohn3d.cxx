#include <stdio.h>
#include <stdlib.h>
#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "GifFactory.h"

typedef struct
{
  PyObject_HEAD
  GifFactory *gif_factory;
} PyGifFactoryObject;

static void gif_factory_dealloc(PyGifFactoryObject *object)
{
  delete object->gif_factory;
  object->gif_factory = NULL;
  Py_DECREF(object);
  PyObject_Del(object);
}

PyTypeObject gif_factory_type =
{
  PyObject_HEAD_INIT(0)
  //0,
  "GifFactory",
  sizeof(PyGifFactoryObject),
  0,
  (destructor)gif_factory_dealloc,
};

static PyObject *create(
  PyObject *self,
  PyObject *args)
{
  char *filename;
  int width, height;

  if (!PyArg_ParseTuple(args, "sii", &filename, &width, &height))
  {
    return NULL;
  }

  PyGifFactoryObject *object = PyObject_NEW(PyGifFactoryObject, &gif_factory_type);
  Py_INCREF(object);

  object->gif_factory = new GifFactory(width, height);
  object->gif_factory->create(filename);

  return (PyObject *)object;
}

static PyObject *finish(
  PyObject *self,
  PyObject *args)
{
  PyGifFactoryObject *object;

  if (!PyArg_ParseTuple(args, "O", &object))
  {
    return NULL;
  }

  object->gif_factory->finish();
  delete object->gif_factory;
  object->gif_factory = NULL;

  return Py_None;
}

static PyObject *add_color(
  PyObject *self,
  PyObject *args)
{
  PyGifFactoryObject *object;
  int value;

  if (!PyArg_ParseTuple(args, "Oi", &object, &value))
  {
    return NULL;
  }

  object->gif_factory->add_color(value);

  return Py_None;
}

static PyObject *set_bg_color_index(
  PyObject *self,
  PyObject *args)
{
  PyGifFactoryObject *object;
  int value;

  if (!PyArg_ParseTuple(args, "Oi", &object, &value))
  {
    return NULL;
  }

  object->gif_factory->set_bg_color_index(value);

  return Py_None;
}

static PyObject *set_transparent_color_index(
  PyObject *self,
  PyObject *args)
{
  PyGifFactoryObject *object;
  int value;

  if (!PyArg_ParseTuple(args, "Oi", &object, &value))
  {
    return NULL;
  }

  object->gif_factory->set_transparent_color_index(value);

  return Py_None;
}

static PyObject *set_delay(
  PyObject *self,
  PyObject *args)
{
  PyGifFactoryObject *object;
  int value;

  if (!PyArg_ParseTuple(args, "Oi", &object, &value))
  {
    return NULL;
  }

  object->gif_factory->set_delay(value);

  return Py_None;
}

static PyObject *set_loop_count(
  PyObject *self,
  PyObject *args)
{
  PyGifFactoryObject *object;
  int value;

  if (!PyArg_ParseTuple(args, "Oi", &object, &value))
  {
    return NULL;
  }

  object->gif_factory->set_loop_count(value);

  return Py_None;
}

static PyObject *init_end(
  PyObject *self,
  PyObject *args)
{
  PyGifFactoryObject *object;

  if (!PyArg_ParseTuple(args, "O", &object))
  {
    return NULL;
  }

  object->gif_factory->init_end();

  return Py_None;
}

static PyObject *clear(
  PyObject *self,
  PyObject *args)
{
  PyGifFactoryObject *object;

  if (!PyArg_ParseTuple(args, "O", &object))
  {
    return NULL;
  }

  object->gif_factory->clear();

  return Py_None;
}

static PyObject *draw_pixel(
  PyObject *self,
  PyObject *args)
{
  PyGifFactoryObject *object;
  int x, y, color_index, z;

  if (PyArg_ParseTuple(args, "Oiii", &object, &x, &y, &color_index))
  {
    object->gif_factory->draw_pixel(x, y, color_index);
    return Py_None;
  }

  if (PyArg_ParseTuple(args, "Oiiii", &object, &x, &y, &color_index, &z))
  {
    object->gif_factory->draw_pixel(x, y, color_index, z);
    return Py_None;
  }

  return NULL;
}

static PyObject *draw_line(
  PyObject *self,
  PyObject *args)
{
  PyGifFactoryObject *object;
  int x0, y0, z0;
  int x1, y1, z1;
  int color_index;

  if (PyArg_ParseTuple(args, "Oiiii", &object, &x0, &y0, &x1, &y1, &color_index))
  {
    object->gif_factory->draw_line(x0, y0, x1, y1, color_index);
    return Py_None;
  }

  if (PyArg_ParseTuple(args, "Oiiiiii", &object, &x0, &y0, &z0, &x1, &y1, &z1, &color_index))
  {
    object->gif_factory->draw_line(x0, y0, z0, x1, y1, z1, color_index);
    return Py_None;
  }

  return NULL;
}

static PyObject *draw_rect(
  PyObject *self,
  PyObject *args)
{
  PyGifFactoryObject *object;
  int x0, y0;
  int x1, y1;
  int z, color_index;

  if (PyArg_ParseTuple(args, "Oiiiii", &object, &x0, &y0, &x1, &y1, &color_index))
  {
    object->gif_factory->draw_rect(x0, y0, x1, y1, color_index);
    return Py_None;
  }

  if (PyArg_ParseTuple(args, "Oiiiiii", &object, &x0, &y0, &x1, &y1, &color_index, &z))
  {
    return Py_None;
  }

  return NULL;
}

static PyObject *draw_triangle(
  PyObject *self,
  PyObject *args)
{
  PyGifFactoryObject *object;
  PyObject *triangle_list;
  PyObject *rotation_list;
  int x, y, z;
  int color_index;
  GifFactory::Triangle triangle;
  GifFactory::Rotation rotation;

  switch (PyTuple_Size(args))
  {
    case 5:
    {
      if (PyArg_ParseTuple(args, "OOiii", &object, &triangle_list, &x, &y, &color_index))
      {
        if (PyList_Size(triangle_list) != 9) { return NULL; }

        triangle.x0 = (int)PyLong_AsLong(PyList_GetItem(triangle_list, 0));
        triangle.y0 = (int)PyLong_AsLong(PyList_GetItem(triangle_list, 1));
        triangle.z0 = (int)PyLong_AsLong(PyList_GetItem(triangle_list, 2));
        triangle.x1 = (int)PyLong_AsLong(PyList_GetItem(triangle_list, 3));
        triangle.y1 = (int)PyLong_AsLong(PyList_GetItem(triangle_list, 4));
        triangle.z1 = (int)PyLong_AsLong(PyList_GetItem(triangle_list, 5));
        triangle.x2 = (int)PyLong_AsLong(PyList_GetItem(triangle_list, 6));
        triangle.y2 = (int)PyLong_AsLong(PyList_GetItem(triangle_list, 7));
        triangle.z2 = (int)PyLong_AsLong(PyList_GetItem(triangle_list, 8));

        object->gif_factory->draw_triangle(triangle, x, y, color_index);
        return Py_None;
      }

      break;
    }
    case 6:
    {
      if (PyArg_ParseTuple(args, "OOiiii", &object, &triangle_list, &x, &y, &z, &color_index))
      {
        if (PyList_Size(triangle_list) != 9) { return NULL; }

        triangle.x0 = (int)PyLong_AsLong(PyList_GetItem(triangle_list, 0));
        triangle.y0 = (int)PyLong_AsLong(PyList_GetItem(triangle_list, 1));
        triangle.z0 = (int)PyLong_AsLong(PyList_GetItem(triangle_list, 2));
        triangle.x1 = (int)PyLong_AsLong(PyList_GetItem(triangle_list, 3));
        triangle.y1 = (int)PyLong_AsLong(PyList_GetItem(triangle_list, 4));
        triangle.z1 = (int)PyLong_AsLong(PyList_GetItem(triangle_list, 5));
        triangle.x2 = (int)PyLong_AsLong(PyList_GetItem(triangle_list, 6));
        triangle.y2 = (int)PyLong_AsLong(PyList_GetItem(triangle_list, 7));
        triangle.z2 = (int)PyLong_AsLong(PyList_GetItem(triangle_list, 8));

        object->gif_factory->draw_triangle(triangle, x, y, z, color_index);
        return Py_None;
      }

      break;
    }
    case 7:
    {
      if (PyArg_ParseTuple(args, "OOOiiii", &object, &triangle_list, &rotation_list, &x, &y, &z, &color_index))
      {
        if (PyList_Size(triangle_list) != 9) { return NULL; }
        if (PyList_Size(rotation_list) != 3) { return NULL; }

        triangle.x0 = (int)PyLong_AsLong(PyList_GetItem(triangle_list, 0));
        triangle.y0 = (int)PyLong_AsLong(PyList_GetItem(triangle_list, 1));
        triangle.z0 = (int)PyLong_AsLong(PyList_GetItem(triangle_list, 2));
        triangle.x1 = (int)PyLong_AsLong(PyList_GetItem(triangle_list, 3));
        triangle.y1 = (int)PyLong_AsLong(PyList_GetItem(triangle_list, 4));
        triangle.z1 = (int)PyLong_AsLong(PyList_GetItem(triangle_list, 5));
        triangle.x2 = (int)PyLong_AsLong(PyList_GetItem(triangle_list, 6));
        triangle.y2 = (int)PyLong_AsLong(PyList_GetItem(triangle_list, 7));
        triangle.z2 = (int)PyLong_AsLong(PyList_GetItem(triangle_list, 8));

        rotation.rx = (int)PyFloat_AsDouble(PyList_GetItem(rotation_list, 0));
        rotation.ry = (int)PyFloat_AsDouble(PyList_GetItem(rotation_list, 1));
        rotation.rz = (int)PyFloat_AsDouble(PyList_GetItem(rotation_list, 2));

        object->gif_factory->draw_triangle(triangle, rotation, x, y, z, color_index);
        return Py_None;
      }

      break;
    }
  }

  return NULL;
}

static PyObject *write_frame(
  PyObject *self,
  PyObject *args)
{
  PyGifFactoryObject *object;

  if (!PyArg_ParseTuple(args, "O", &object))
  {
    return NULL;
  }

  object->gif_factory->write_frame();

  return Py_None;
}

static PyMethodDef GifFactoryMethods[] =
{
  { "create", create, METH_VARARGS, "Start a new GIF."},
  { "finish", finish, METH_VARARGS, "Close the GIF."},
  { "add_color", add_color, METH_VARARGS, "Push new color to palette."},
  { "set_bg_color_index", set_bg_color_index, METH_VARARGS, "Set background color."},
  { "set_transparent_color_index", set_transparent_color_index, METH_VARARGS, "Set transparent index."},
  { "set_delay", set_delay, METH_VARARGS, "Set GIF animation delay speed in 100'ths of second."},
  { "set_loop_count", set_loop_count, METH_VARARGS, "Set GIF animation loop count (0 for inifinite)."},
  { "init_end", init_end, METH_VARARGS, "Write GIF header to file."},
  //{ "get_picture", get_picture, METH_VARARGS, "Write raw picture."},
  { "clear", clear, METH_VARARGS, "Write GIF header to file."},
  { "draw_pixel", draw_pixel, METH_VARARGS, "Draw pixel in image."},
  { "draw_line", draw_line, METH_VARARGS, "Draw line in image."},
  { "draw_rect", draw_rect, METH_VARARGS, "Draw rectangle in image."},
  { "draw_triangle", draw_triangle, METH_VARARGS, "Draw triangle in image."},
  { "write_frame", write_frame, METH_VARARGS, "Add a frame to the GIF."},
  { NULL, NULL, 0, NULL }
};

static struct PyModuleDef gif_factory_module =
{
  PyModuleDef_HEAD_INIT,
  "gif_factory",
  "Python interface for GIF Factory.",
  -1,
  GifFactoryMethods
};

PyMODINIT_FUNC PyInit_gif_factory(void)
{
  return PyModule_Create(&gif_factory_module);
}

