/*

  Kohn3D - GIF drawing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ImageWriterAvi.h"

ImageWriterAvi::ImageWriterAvi(int width, int height, int depth) :
  ImageWriter(width, height),
  depth { depth },
  offset_to_image { 0 }
{
  memset(&avi_header, 0, sizeof(avi_header));
  memset(&stream_header, 0, sizeof(stream_header));
  memset(&stream_format, 0, sizeof(stream_format));
}

ImageWriterAvi::~ImageWriterAvi()
{
  finish();
}

void ImageWriterAvi::finish()
{
  long marker = ftell(fp);

  fseek(fp, list_marker, SEEK_SET);
  write_uint32((int)marker);

  fseek(fp, avi_header_marker, SEEK_SET);
  write_avi_header();

  fseek(fp, marker, SEEK_SET);
}

int ImageWriterAvi::create_headers()
{
  fwrite("RIFF", 1, 4, fp);
  write_uint32(0);
  fwrite("AVI ", 1, 4, fp);

  fwrite("LIST", 1, 4, fp);
  list_marker = ftell(fp);
  write_uint32(0);
  fwrite("movi", 1, 4, fp);

  avi_header_marker = ftell(fp);
  avi_header.time_delay = 1000000 / fps;
  avi_header.data_rate = width * height * 3;
  avi_header.reserved = 0;
  avi_header.flags = 0;
  avi_header.number_of_frames = 0;
  avi_header.initial_frames = 0;
  avi_header.data_streams = 1;
  avi_header.buffer_size = width * height * 3;
  avi_header.width = width;
  avi_header.height = height;
  avi_header.time_scale = 30;
  avi_header.playback_data_rate = 0;
  avi_header.starting_time = 0;
  avi_header.data_length = 0;

  write_avi_header();

  memcpy(stream_header.data_type, "vids", 4);
  //stream_header.codec[4];
  stream_header.flags = 0;
  stream_header.priority = 0;
  stream_header.initial_frames = 0;
  stream_header.time_scale = 1;
  stream_header.data_rate = fps;
  stream_header.start_time = 0;
  stream_header.data_length = 0;
  stream_header.suggested_buffer_size = width * height * 3;
  stream_header.quality = 0;
  stream_header.sample_size = 0;

  write_stream_header();

  stream_format.header_size = 40;
  stream_format.width = width;
  stream_format.height = height;
  stream_format.number_of_planes = 1;
  stream_format.bits_per_pixel = depth;
  stream_format.compression_type = 0;
  stream_format.image_size = depth == 8 ? width * height : width * height * 3;
  stream_format.x_pels_per_meter = 0;
  stream_format.y_pels_per_meter = 0;
  stream_format.colors_used = max_colors;
  stream_format.colors_important = max_colors;

  write_stream_format();

  return 0;
}

int ImageWriterAvi::add_frame(uint8_t *image, uint32_t *color_table)
{
  int padding;
  int x, y, n;

  avi_header.number_of_frames++;
  avi_header.data_length++;

  offsets.push_back((int)ftell(fp));

  if (depth == 8)
  {
    padding = (4 - (width % 4)) & 0x3;

    for (y = height - 1; y >= 0; y--)
    {
      for (x = 0; x < width; x++)
      {
        putc(image[(y * width) + x], fp);
      }

      for (n = 0; n < padding; n++) { putc(0, fp); }
    }
  }
    else
  {
    uint32_t *image32 = (uint32_t *)image;

    padding = (4 - ((width * 3) % 4)) & 0x3;

    for (y = height - 1; y >= 0; y--)
    {
      for (x = 0; x < width; x++)
      {
        uint32_t color = image32[(y * width) + x];
        putc(color & 0xff, fp);
        putc((color >> 8) & 0xff, fp);
        putc((color >> 16) & 0xff, fp);
      }

      for (n = 0; n < padding; n++) { putc(0, fp); }
    }
  }

  return 0;
}

void ImageWriterAvi::write_avi_header()
{
  int marker, t;

  fwrite("avih", 1, 4, fp);
  marker = ftell(fp);
  write_uint32(0);

  write_uint32(avi_header.time_delay);
  write_uint32(avi_header.data_rate);
  write_uint32(avi_header.reserved);
  write_uint32(avi_header.flags);
  write_uint32(avi_header.number_of_frames);
  write_uint32(avi_header.initial_frames);
  write_uint32(avi_header.data_streams);
  write_uint32(avi_header.buffer_size);
  write_uint32(avi_header.width);
  write_uint32(avi_header.height);
  write_uint32(avi_header.time_scale);
  write_uint32(avi_header.playback_data_rate);
  write_uint32(avi_header.starting_time);
  write_uint32(avi_header.data_length);

  t = ftell(fp);
  fseek(fp, marker, SEEK_SET);
  write_uint32(t - marker - 4);
  fseek(fp, t, SEEK_SET);
}

void ImageWriterAvi::write_stream_header()
{
  int marker, t;

  fwrite("strh", 1, 4, fp);
  marker = ftell(fp);
  write_uint32(0);

  fwrite(stream_header.data_type, 1, 4, fp);
  fwrite(stream_header.codec, 1, 4, fp);
  write_uint32(stream_header.flags);
  write_uint32(stream_header.priority);
  write_uint32(stream_header.initial_frames);
  write_uint32(stream_header.time_scale);
  write_uint32(stream_header.data_rate);
  write_uint32(stream_header.start_time);
  write_uint32(stream_header.data_length);
  write_uint32(stream_header.suggested_buffer_size);
  write_uint32(stream_header.quality);
  write_uint32(stream_header.sample_size);
  write_uint32(0);
  write_uint32(0);

  t = ftell(fp);
  fseek(fp, marker, SEEK_SET);
  write_uint32(t - marker - 4);
  fseek(fp, t, SEEK_SET);
}

void ImageWriterAvi::write_stream_format()
{
  int marker, t;

  fwrite("strf", 1, 4, fp);
  marker = ftell(fp);
  write_uint32(0);

  write_uint32(stream_format.header_size);
  write_uint32(stream_format.width);
  write_uint32(stream_format.height);
  write_uint16(stream_format.number_of_planes);
  write_uint16(stream_format.bits_per_pixel);
  write_uint32(stream_format.compression_type);
  write_uint32(stream_format.image_size);
  write_uint32(stream_format.x_pels_per_meter);
  write_uint32(stream_format.y_pels_per_meter);
  write_uint32(stream_format.colors_used);
  write_uint32(stream_format.colors_important);

  if (stream_format.colors_used != 0)
  {
    for (t = 0; t < stream_format.colors_used; t++)
    {
      putc(palette[t] & 0xff, fp);
      putc((palette[t] >> 8) & 0xff, fp);
      putc((palette[t] >> 16) & 0xff, fp);
      putc(0, fp);
    }
  }

  t = ftell(fp);
  fseek(fp, marker, SEEK_SET);
  write_uint32(t - marker - 4);
  fseek(fp, t, SEEK_SET);
}

void ImageWriterAvi::write_junk_chunk()
{
  int marker, t;
  int r, l, p;
  const char *junk = { "JUNK IN THE CHUNK! " };

  fwrite("JUNK", 1, 4, fp);
  marker = ftell(fp);
  write_uint32(0);

  r = 4096 - ftell(fp);
  l = strlen(junk);
  p = 0;

  for (t = 0; t < r; t++)
  {
    putc(junk[p++], fp);
    if (p >= l) { p = 0; }
  }

  long here = ftell(fp);
  fseek(fp, marker, SEEK_SET);
  write_uint32(here - marker - 4);
  fseek(fp, here, SEEK_SET);
}

void ImageWriterAvi::write_index()
{
  long marker;
  const int frame_size = depth == 8 ? width * height : width * height * 3;
  
  fwrite("idx1", 1, 4, fp);
  marker = ftell(fp);
  write_uint32(0);

  for (auto offset : offsets)
  {
    fwrite("00dc", 1, 4, fp);
    write_uint32(0x10);
    write_uint32(offset);
    write_uint32(frame_size);
  }

  long here = ftell(fp);
  fseek(fp, marker, SEEK_SET);
  write_uint32(here - marker - 4);
  fseek(fp, here, SEEK_SET);
}

#if 0
void ImageWriterAvi::write_bmp_header()
{
  putc('B', fp);
  putc('M', fp);
  write_uint32(0);
  write_uint16(0);
  write_uint16(0);
  write_uint32(0);

  write_uint32(40);
  write_uint32(width);
  write_uint32(height);
  write_uint16(1);
  write_uint16(depth);
  write_uint32(0);
  write_uint32(0);
  write_uint32(0);
  write_uint32(0);

  if (depth == 8)
  {
    write_uint32(max_colors);
    write_uint32(max_colors);
  }
    else
  {
    write_uint32(0);
    write_uint32(0);
  }

  for (int n = 0; n < max_colors; n++)
  {
    putc(palette[n] & 0xff, fp);
    putc((palette[n] >> 8) & 0xff, fp);
    putc((palette[n] >> 16) & 0xff, fp);
    putc(0, fp);
  }

  offset_to_image = ftell(fp);
}
#endif

