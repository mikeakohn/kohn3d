/*

  Kohn3D - GIF drawing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ImageWriterGif.h"

ImageWriterGif::ImageWriterGif(int width, int height) :
  ImageWriter(width, height)
{
  memset(&gif_header, 0, sizeof(gif_header));
  memcpy(gif_header.version, "GIF89a", 6);

  gif_header.width = width;
  gif_header.height = height;
}

ImageWriterGif::~ImageWriterGif()
{
  finish();
}

void ImageWriterGif::finish()
{
  if (fp != nullptr)
  {
    // End Marker.
    putc(';', fp);
  }
}

int ImageWriterGif::create_headers()
{
  uint8_t fields;

  fwrite(gif_header.version, 1, 6, fp);

  // Compute bits per pixel and max colors for that.
  int bits_per_pixel = compute_bits_per_pixel(max_colors);
  const int color_resolution = bits_per_pixel;
  max_colors = 1 << bits_per_pixel;

  // Copy GifHeader (Logical Screen Descriptor) to memory.
  write_uint16(gif_header.width);
  write_uint16(gif_header.height);
  fields = 0x80 | ((color_resolution - 1) << 4) | (bits_per_pixel - 1);
  putc(fields, fp);
  putc(bg_color_index, fp);
  putc(0, fp);

  // Global Color Map (palettes).
  for (int i = 0; i < max_colors; i++)
  {
    putc((palette[i] >> 16) & 0xff, fp);
    putc((palette[i] >> 8) & 0xff, fp);
    putc(palette[i] & 0xff, fp);
  }

  if (loop_count != -1)
  {
    // Application Extension Block for NETSCAPE (for looping GIFs).
    putc(0x21, fp);
    putc(0xff, fp);
    putc(0x0b, fp);
    fwrite("NETSCAPE", 1, 8, fp);
    fwrite("2.0", 1, 3, fp);
    putc(0x03, fp);
    putc(0x01, fp);
    write_uint16(loop_count);
    putc(0x00, fp);
  }

  return 0;
}

int ImageWriterGif::add_frame(uint8_t *image, uint32_t *color_table)
{
  int ptr = 0, i;

  int bits_per_pixel = compute_bits_per_pixel(max_colors);
  int code_size = bits_per_pixel;
  int clear_code = max_colors;
  int eof_code = max_colors + 1;

  // Graphics Control Extension Block (GIF89a only).
  int user_input_flag = 0;
  int disposal_method = 0;

  putc(0x21, fp);
  putc(0xf9, fp);
  putc(0x04, fp);
  putc((disposal_method << 2) | (user_input_flag << 1) | do_transparency, fp);
  write_uint16(delay);
  putc(transparent_color_index, fp);
  putc(0, fp);

  // Image Descriptor Block.
  putc(',', fp);
  write_uint16(0);
  write_uint16(0);
  write_uint16(gif_header.width);
  write_uint16(gif_header.height);
  putc(bits_per_pixel - 1, fp);


  putc(code_size, fp);

  // Setup LZW tree.
  CompressNode node[4097];

  for (i = 0; i < max_colors; i++)
  {
    node[i].color = i;
    node[i].right = i + 1;
    node[i].down = -1;
  }

  // Compressed data blocks follow.
  int table_start_size = max_colors + 2;
  int next_code = table_start_size;
  int curr_code_size = code_size + 1;
  int curr_code = -1;
  int data_size = 0;

  // LZW Compression.
  BitStream bit_stream;

  bit_stream.append(clear_code, curr_code_size);

  int image_ptr = 0;
  int length = gif_header.width * gif_header.height;
  uint8_t color = image[image_ptr++];
  int block_ptr;
  uint8_t data[1024];

  block_ptr = ptr;
  data[ptr++] = 0;

  curr_code = color;
  int last_code = curr_code;

  while (image_ptr < length)
  {
    color = image[image_ptr++];

    if (node[curr_code].down == -1)
    {
      last_code = curr_code;
      node[curr_code].down = next_code;
    }
      else
    {
      last_code = curr_code;
      curr_code = node[curr_code].down;

      while (node[curr_code].color != color && node[curr_code].right != -1)
      {
        curr_code=node[curr_code].right;
      }

      if (node[curr_code].color != color && node[curr_code].right == -1)
      {
        node[curr_code].right=next_code;
      }
        else
      {
        continue;
      }
    }

    bit_stream.append(last_code, curr_code_size);

    node[next_code].right = -1;
    node[next_code].down = -1;
    node[next_code].color = color;
    curr_code = color;

    if ((next_code >> curr_code_size) != 0)
    {
      if (curr_code_size >= 12)
      {
        bit_stream.append(clear_code, curr_code_size);

        for (i = 0; i < max_colors; i++)
        {
          node[i].down = -1;
        }

        next_code = table_start_size - 1;
        curr_code_size = code_size;
      }

      curr_code_size++;
    }

    next_code++;

    while (bit_stream.size() >= 8)
    {
      data[ptr++] = bit_stream.get_byte();
      data_size++;

      if (data_size >= 255)
      {
        data[block_ptr] = data_size;
        data_size = 0;
        fwrite(data, 1, ptr, fp);

        ptr = 0;
        block_ptr = ptr;
        data[ptr++] = 0;
      }
    }
  }

  bit_stream.append(curr_code, curr_code_size);
  bit_stream.append(eof_code, curr_code_size);

  while (bit_stream.size() > 0)
  {
    data[ptr++] = bit_stream.get_byte();
    data_size++;

    if (data_size >= 255)
    {
      data[block_ptr] = data_size;
      data_size = 0;
      fwrite(data, 1, ptr, fp);

      ptr = 0;
      block_ptr = ptr;
      data[ptr++] = 0;
    }
  }

  if (data_size > 0)
  {
    data[block_ptr] = data_size;
    fwrite(data, 1, data_size + 1, fp);
  }

  putc(0, fp);

  return 0;
}

int ImageWriterGif::compute_bits_per_pixel(int max_colors)
{
  uint8_t counts[16] =
  {
    0, 1, 2, 2, 3, 3, 3, 3,
    4, 4, 4, 4, 4, 4, 4, 4,
  };

  max_colors -= 1;

  int upper = counts[max_colors >> 4];
  int lower = counts[max_colors & 0xf];

  if (upper != 0) { return upper + 4; }
  if (lower == 0) { lower = 1; }

  return lower;
}

