#include <stdio.h>
#include <stdlib.h>
#define PY_SSIZE_T_CLEAN
#include <Python.h>

#include "Kohn3D.h"

typedef struct
{
  PyObject_HEAD
  Kohn3D *kohn3d;
} PyKohn3DObject;

static void kohn3d_dealloc(PyKohn3DObject *object)
{
  delete object->kohn3d;
  object->kohn3d = NULL;
  Py_DECREF(object);
  PyObject_Del(object);
}

PyTypeObject kohn3d_type =
{
  PyObject_HEAD_INIT(0)
  //0,
  "Kohn3D",
  sizeof(PyKohn3DObject),
  0,
  (destructor)kohn3d_dealloc,
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

  PyKohn3DObject *object = PyObject_NEW(PyKohn3DObject, &kohn3d_type);
  Py_INCREF(object);

  object->kohn3d = new Kohn3D(width, height);
  object->kohn3d->create(filename);

  return (PyObject *)object;
}

static PyObject *finish(
  PyObject *self,
  PyObject *args)
{
  PyKohn3DObject *object;

  if (!PyArg_ParseTuple(args, "O", &object))
  {
    return NULL;
  }

  object->kohn3d->finish();
  delete object->kohn3d;
  object->kohn3d = NULL;

  return Py_None;
}

static PyObject *add_color(
  PyObject *self,
  PyObject *args)
{
  PyKohn3DObject *object;
  int value;

  if (!PyArg_ParseTuple(args, "Oi", &object, &value))
  {
    return NULL;
  }

  object->kohn3d->add_color(value);

  return Py_None;
}

static PyObject *set_bg_color_index(
  PyObject *self,
  PyObject *args)
{
  PyKohn3DObject *object;
  int value;

  if (!PyArg_ParseTuple(args, "Oi", &object, &value))
  {
    return NULL;
  }

  object->kohn3d->set_bg_color_index(value);

  return Py_None;
}

static PyObject *set_transparent_color_index(
  PyObject *self,
  PyObject *args)
{
  PyKohn3DObject *object;
  int value;

  if (!PyArg_ParseTuple(args, "Oi", &object, &value))
  {
    return NULL;
  }

  object->kohn3d->set_transparent_color_index(value);

  return Py_None;
}

static PyObject *set_delay(
  PyObject *self,
  PyObject *args)
{
  PyKohn3DObject *object;
  int value;

  if (!PyArg_ParseTuple(args, "Oi", &object, &value))
  {
    return NULL;
  }

  object->kohn3d->set_delay(value);

  return Py_None;
}

static PyObject *set_loop_count(
  PyObject *self,
  PyObject *args)
{
  PyKohn3DObject *object;
  int value;

  if (!PyArg_ParseTuple(args, "Oi", &object, &value))
  {
    return NULL;
  }

  object->kohn3d->set_loop_count(value);

  return Py_None;
}

static PyObject *init_end(
  PyObject *self,
  PyObject *args)
{
  PyKohn3DObject *object;

  if (!PyArg_ParseTuple(args, "O", &object))
  {
    return NULL;
  }

  object->kohn3d->init_end();

  return Py_None;
}

static PyObject *clear(
  PyObject *self,
  PyObject *args)
{
  PyKohn3DObject *object;

  if (!PyArg_ParseTuple(args, "O", &object))
  {
    return NULL;
  }

  object->kohn3d->clear();

  return Py_None;
}

static PyObject *draw_pixel(
  PyObject *self,
  PyObject *args)
{
  PyKohn3DObject *object;
  int x, y, color_index, z;

  if (PyArg_ParseTuple(args, "Oiii", &object, &x, &y, &color_index))
  {
    object->kohn3d->draw_pixel(x, y, color_index);
    return Py_None;
  }

  if (PyArg_ParseTuple(args, "Oiiii", &object, &x, &y, &color_index, &z))
  {
    object->kohn3d->draw_pixel(x, y, color_index, z);
    return Py_None;
  }

  return NULL;
}

static PyObject *draw_line(
  PyObject *self,
  PyObject *args)
{
  PyKohn3DObject *object;
  int x0, y0, z0;
  int x1, y1, z1;
  int color_index;

  if (PyArg_ParseTuple(args, "Oiiii", &object, &x0, &y0, &x1, &y1, &color_index))
  {
    object->kohn3d->draw_line(x0, y0, x1, y1, color_index);
    return Py_None;
  }

  if (PyArg_ParseTuple(args, "Oiiiiii", &object, &x0, &y0, &z0, &x1, &y1, &z1, &color_index))
  {
    object->kohn3d->draw_line(x0, y0, z0, x1, y1, z1, color_index);
    return Py_None;
  }

  return NULL;
}

static PyObject *draw_rect(
  PyObject *self,
  PyObject *args)
{
  PyKohn3DObject *object;
  int x0, y0;
  int x1, y1;
  int z, color_index;

  if (PyArg_ParseTuple(args, "Oiiiii", &object, &x0, &y0, &x1, &y1, &color_index))
  {
    object->kohn3d->draw_rect(x0, y0, x1, y1, color_index);
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
  PyKohn3DObject *object;
  PyObject *triangle_list;
  PyObject *rotation_list;
  int x, y, z;
  int color_index;
  Kohn3D::Triangle triangle;
  Kohn3D::Rotation rotation;

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

        object->kohn3d->draw_triangle(triangle, x, y, color_index);
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

        object->kohn3d->draw_triangle(triangle, x, y, z, color_index);
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

        object->kohn3d->draw_triangle(triangle, rotation, x, y, z, color_index);
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
  PyKohn3DObject *object;

  if (!PyArg_ParseTuple(args, "O", &object))
  {
    return NULL;
  }

  object->kohn3d->write_frame();

  return Py_None;
}

static PyMethodDef Kohn3DMethods[] =
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

static struct PyModuleDef kohn3d_module =
{
  PyModuleDef_HEAD_INIT,
  "kohn3d",
  "Python interface for GIF Factory.",
  -1,
  Kohn3DMethods
};

PyMODINIT_FUNC PyInit_kohn3d(void)
{
  return PyModule_Create(&kohn3d_module);
}

