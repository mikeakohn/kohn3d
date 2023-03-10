/*

  Kohn3D - GIF drawing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#ifndef IMAGE_WRITER_AVI_H
#define IMAGE_WRITER_AVI_H

#include <stdint.h>
#include <vector>

#include "ImageWriter.h"

class ImageWriterAvi : public ImageWriter
{
public:
  ImageWriterAvi(int width, int height, int depth);
  virtual ~ImageWriterAvi();

  virtual void finish();
  virtual int create_headers();
  virtual int add_frame(uint8_t *image, uint32_t *color_table);

private:
  void write_avi_header();
  void write_stream_header();
  void write_stream_format();
  void write_junk_chunk();
  void write_index();
  void write_bmp_header();

  struct AviHeader
  {
    int time_delay;
    int data_rate;
    int reserved;
    int flags;
    int number_of_frames;
    int initial_frames;
    int data_streams;
    int buffer_size;
    int width;
    int height;
    int time_scale;
    int playback_data_rate;
    int starting_time;
    int data_length;
  };

  struct StreamHeader
  {
    char data_type[4];
    char codec[4];
    int flags;
    int priority;
    int initial_frames;
    int time_scale;
    int data_rate;
    int start_time;
    int data_length;
    int suggested_buffer_size;
    int quality;
    int sample_size;
  };

  struct StreamFormat
  {
    int header_size;
    int width;
    int height;
    int number_of_planes;
    int bits_per_pixel;
    int compression_type;
    int image_size;
    int x_pels_per_meter;
    int y_pels_per_meter;
    int colors_used;
    int colors_important;
  };

  struct IndexEntry
  {
    char ckid[4];
    int dwFlags;
    int dwChunkOffset;
    int dwChunkLength;
  };

  AviHeader avi_header;
  StreamHeader stream_header;
  StreamFormat stream_format;
  std::vector<uint32_t> offsets;

  int depth;
  long offset_to_image;
  long list_marker;
  long avi_header_marker;
};

#endif

