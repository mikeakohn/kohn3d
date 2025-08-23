/*

  Kohn3D - GIF drawing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ImageReaderGif.h"

ImageReaderGif::ImageReaderGif() :
  ImageReader(),
  global_colors { 0 },
  local_colors { 0 },
  loop_count { 0 }
{
  memset(&header, 0, sizeof(header));
  memset(&local_palette, 0, sizeof(local_palette));
  memset(&image_descriptor, 0, sizeof(image_descriptor));
  memset(&plain_text, 0, sizeof(plain_text));
  memset(&graphics_control, 0, sizeof(graphics_control));
  memset(&application_extension, 0, sizeof(application_extension));

  graphics_control.transparent_index = 256;
}

ImageReaderGif::~ImageReaderGif()
{
}

int ImageReaderGif::load(const char *filename)
{
  fp = fopen(filename, "rb");

  if (fp == NULL) { return -2; }
  int result = read_file();
  fclose(fp);

  fp = NULL;

  return result;
}

int ImageReaderGif::read_file()
{
  if (read_header() != 0) { return -1; }

  width = header.width;
  height = header.height;

  const int length = width * height * sizeof(uint32_t);
  image = (uint32_t *)malloc(length);
  memset(image, 0, length);

  while (true)
  {
    int separator = getc(fp);

    // End of file.
    if (separator == ';' || separator == EOF) { break; }

    if (separator == ',')
    {
      read_image_descriptor();
      read_image();
    }
      else
    if (separator == 0x21)
    {
      read_extension();
    }
      else
    {
      printf("Error: Unknown separator 0x%02x\n", separator);
      return -1;
    }
  }

  return 0;
}

int ImageReaderGif::read_header()
{
  if (fread(header.signature, 1, 6, fp) != 6) { return -2; }

  if (memcmp(header.signature, "GIF89a", 6) != 0 &&
      memcmp(header.signature, "GIF87a", 6) != 0)
  {
    return -3;
  }

  header.width = read_uint16();
  header.height = read_uint16();
  header.fields = getc(fp);
  header.bg_color_index = getc(fp);
  header.aspect_ratio = getc(fp);

  global_colors = 1 << ((header.fields & 0x7) + 1);
  if (((header.fields >> 7) & 1) == 0) { global_colors = 0; }

  for (int n = 0; n < global_colors; n++)
  {
    palette[n] = getc(fp) << 16;
    palette[n] |= getc(fp) << 8;
    palette[n] |= getc(fp);
  }

  return 0;
}

int ImageReaderGif::read_image_descriptor()
{
  image_descriptor.left_position = read_uint16();
  image_descriptor.top_position = read_uint16();
  image_descriptor.width = read_uint16();
  image_descriptor.height = read_uint16();
  image_descriptor.fields = getc(fp);

  local_colors = 1 << ((image_descriptor.fields & 0x7) + 1);
  if (((image_descriptor.fields >> 7) & 1) == 0) { local_colors = 0; }

  for (int n = 0; n < local_colors; n++)
  {
    local_palette[n] = getc(fp) << 16;
    local_palette[n] |= getc(fp) << 8;
    local_palette[n] |= getc(fp);
  }

  return 0;
}

int ImageReaderGif::read_extension()
{
  Extension extension;

  extension.introducer = 0x21;
  extension.label = getc(fp);
  extension.block_size = getc(fp);

  switch (extension.label)
  {
    case 0x01: read_plain_text(extension); break;
    case 0xf9: read_graphics_control(extension); break;
    case 0xfe: read_comment(extension); break;
    case 0xff: read_application_extension(extension); break;
    default:   fseek(fp, extension.block_size, SEEK_SET); break;
  }

  return 0;
}

int ImageReaderGif::read_plain_text(Extension &extension)
{
  plain_text.introducer = extension.introducer;
  plain_text.label = extension.label;
  plain_text.block_size = extension.block_size;

  plain_text.text_grid_left = read_uint16();
  plain_text.text_grid_top = read_uint16();
  plain_text.text_grid_width = read_uint16();
  plain_text.text_grid_height = read_uint16();
  plain_text.cell_width = getc(fp);
  plain_text.cell_height = getc(fp);
  plain_text.fg_color_index = getc(fp);
  plain_text.bg_color_index = getc(fp);

  while (true)
  {
    int length = getc(fp);
    if (length == 0) { break; }

    for (int n = 0; n < length; n++) { printf("%c", getc(fp)); }
  }

  return 0;
}

int ImageReaderGif::read_graphics_control(Extension &extension)
{
  graphics_control.introducer = extension.introducer;
  graphics_control.label = extension.label;
  graphics_control.block_size = extension.block_size;

  graphics_control.fields = getc(fp);
  graphics_control.delay_time = read_uint16();
  graphics_control.transparent_index = getc(fp);
  graphics_control.terminator = getc(fp);

  return 0;
}

int ImageReaderGif::read_comment(Extension &extension)
{
  int length = extension.block_size;

  while (true)
  {
    if (length == 0) { break; }

    for (int n = 0; n < length; n++)
    {
      printf("%c", getc(fp));
    }

    length = getc(fp);
  }

  printf("\n");

  return 0;
}

int ImageReaderGif::read_application_extension(Extension &extension)
{
  application_extension.introducer = extension.introducer;
  application_extension.label = extension.label;
  application_extension.block_size = extension.block_size;

  if (fread(application_extension.identifier, 1, 8, fp) != 8) { return -1; }
  if (fread(application_extension.authentication_code, 1, 3, fp) != 3) { return -1; }

  uint8_t data[256];

  while (1)
  {
    int length = getc(fp);
    if (length == 0) { break; }
    for (int n = 0; n < length; n++) { data[n] = getc(fp); }
  }

  if (memcmp(application_extension.identifier, "NETSCAPE", 8) == 0)
  {
    loop_count = data[2] | ((int)data[3] << 8);
  }

  return 0;
}

int ImageReaderGif::read_image()
{
  int n, x, y;
  int code_size = getc(fp);
  int start_table_size = (1 << code_size);
  int clear_code = start_table_size;
  int eof_code = start_table_size + 1;
  Node nodes[4096];
  uint8_t temp[4096];

  code_size++;

  for (n = 0; n < start_table_size; n++)
  {
    nodes[n].prev = -1;
    nodes[n].color = n;
  }

  //x = image_descriptor.left_position;
  //y = image_descriptor.top_position;
  x = 0;
  y = 0;

  BitStream bit_stream;
  bit_stream.set_code_size(code_size);

  bool running = true;
  int last_code = -1;
  int total_bytes = width * height;
  int next_code = start_table_size + 2;

  while (running)
  {
    int data_count = getc(fp);
    if (data_count == 0) { break; }
    bit_stream.read(fp, data_count);

    while (true)
    {
      int code = bit_stream.get_next();
      if (code == -1) { break; }

      if (code == clear_code)
      {
        last_code = -1;
        next_code = start_table_size + 2;
        bit_stream.set_code_size(code_size);
        continue;
      }

      if (next_code == 4096)
      {
        printf("Error: next_code overflow\n");
        running = false;
        break;
      }

      if (code == eof_code)
      {
        // Even though there is an EOF code, there is a block of 0
        // size next (signifying end of data.
        data_count = getc(fp);

        if (data_count != 0)
        {
          printf("Error: EOF code detected, but next block is non-zero.\n");
          return -1;
        }

        running = false;
        break;
      }

      if (last_code == -1)
      {
        set_pixel(x, y, nodes[code].color);
        last_code = code;
        total_bytes--;
        continue;
      }

      int length = 0;

      if (code < next_code)
      {
        int index = code;

        while (index != -1)
        {
          temp[length++] = nodes[index].color;
          index = nodes[index].prev;
          total_bytes--;
        }

        nodes[next_code].color = temp[length - 1];
        nodes[next_code].prev = last_code;

        if (next_code == (int)bit_stream.mask) { bit_stream.inc_code_size(); }
        next_code++;

        for (int n = length - 1; n >= 0; n--)
        {
          set_pixel(x, y, temp[n]);
        }
      }
        else
      {
        int index = last_code;
        int last_color = -1;

        while (index != -1)
        {
          last_color = nodes[index].color;
          temp[length++] = last_color;
          index = nodes[index].prev;
          total_bytes--;
        }

        nodes[next_code].color = temp[length - 1];
        nodes[next_code].prev = last_code;

        if (next_code == (int)bit_stream.mask) { bit_stream.inc_code_size(); }
        next_code++;

        for (int n = length - 1; n >= 0; n--)
        {
          set_pixel(x, y, temp[n]);
        }

        set_pixel(x, y, temp[length - 1]);
      }

      last_code = code;
    }
  }

  while (total_bytes != 0)
  {
    set_pixel(x, y, header.bg_color_index);
    total_bytes--;
  }

  return 0;
}

