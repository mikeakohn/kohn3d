/*

  Kohn3D - GIF drawing library.

  Copyright 2023 - Michael Kohn (mike@mikekohn.net)
  https://www.mikekohn.net/

  This code falls under the LGPL license.

*/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

static int read_int16(FILE *in)
{
  int n;

  n = getc(in);
  n |= getc(in) << 8;

  return n;
}

int parse_extensions(FILE *in)
{
  uint8_t fields;
  int n;

  while (1)
  {
    n = getc(in);

    if (n != 0x21)
    {
      ungetc(n, in);
      break;
    }

    int label = getc(in);

    switch (label)
    {
      case 0x01:
        printf("-- plain text extension block --\n");
        printf("         introducer: 0x%02x\n", n);
        printf("              label: 0x%02x\n", label);
        printf("         block_size: 0x%02x\n", getc(in));
        printf("     text_grid_left: %d\n", read_int16(in));
        printf("      text_grid_top: %d\n", read_int16(in));
        printf("    text_grid_width: %d\n", read_int16(in));
        printf("   text_grid_height: %d\n", read_int16(in));
        printf("         cell_width: %d\n", getc(in));
        printf("        cell_height: %d\n", getc(in));
        printf("text_fg_color_index: %d\n", getc(in));
        printf("text_bg_color_index: %d\n", getc(in));
        printf("               text: ");
        while (1)
        {
          int length = getc(in);
          if (length == 0) { break; }

          for (n = 0; n < length; n++) { printf("%c", getc(in)); }
        }
        printf("\n");
        printf("       terminator: %d\n", 0);
        break;
      case 0xf9:
        printf("-- graphics control extension block --\n");
        printf("       introducer: 0x%02x\n", n);
        printf("            label: 0x%02x\n", label);
        printf("       block_size: 0x%02x\n", getc(in));
        fields = getc(in);
        printf("           packed: 0x%02x\n", fields);
        printf("             disposal_method=%d\n", (fields >> 2) & 7);
        printf("             user_input_flag=%d\n", (fields >> 1) & 1);
        printf("             do_transparency=%d\n", fields & 1);
        printf("       delay_time: %d hundreths of second\n", read_int16(in));
        printf("transparent_index: %d\n", getc(in));
        printf("       terminator: %d\n", getc(in));
        break;
      case 0xff:
        printf("-- application extension block --\n");
        printf("       introducer: 0x%02x\n", n);
        printf("            label: 0x%02x\n", label);
        printf("       block_size: 0x%02x\n", getc(in));
        printf("       identifier: ");
        for (n = 0; n < 8; n++) { printf("%c", getc(in)); }
        printf("\n");
        printf("     authent_code: ");
        for (n = 0; n < 3; n++) { printf("%c", getc(in)); }
        printf("\n");
        printf(" application_data:");
        while (1)
        {
          int length = getc(in);
          if (length == 0) { break; }
          for (n = 0; n < length; n++) { printf(" %02x", getc(in)); }
        }
        printf("\n");
        printf("       terminator: %d\n", 0);
        break;
      case 0xfe:
        printf("-- comment extension block --\n");
        printf("       introducer: 0x%02x\n", n);
        printf("            label: 0x%02x\n", label);
        //printf("       block_size: 0x%02x\n", getc(in));
        printf("             text: ");
        while (1)
        {
          int length = getc(in);
          if (length == 0) { break; }

          for (n = 0; n < length; n++) { printf("%c", getc(in)); }
        }
        printf("\n");
        printf("       terminator: %d\n", 0);
        break;
      default:
        printf("Unknown label %02x\n", label);
        exit(1);
    }
  }

  return 0;
}

int parse_gif(FILE *in)
{
  char signature[8];
  uint8_t fields;
  int colors;
  int n;

  memset(signature, 0, sizeof(signature));
  if (fread(signature, 1, 6, in) != 6) { return -1; }

  printf("     signature: %s\n\n", signature);

  printf("-- logical screen descriptor --\n");
  printf("         width: %d\n", read_int16(in));
  printf("        height: %d\n", read_int16(in));
  fields = getc(in);
  colors = 1 << ((fields & 0x7) + 1);
  if (((fields >> 7) & 1) == 0) { colors = 0; }
  printf("        fields: %d\n", fields);
  printf("          global_color_table=%d\n", (fields >> 7) & 1);
  printf("          color_resolution=%d\n", (fields >> 4) & 0x7);
  printf("          sort_flag=%d\n", (fields >> 3) & 0x1);
  printf("          size_of_global_color_table=%d (%d)\n", fields & 0x7, colors);
  printf("bg_color_index: %d\n", getc(in));
  printf("  aspect_ratio: %d\n", getc(in));

  printf("-- global color table --");
  for (n = 0; n < colors; n++)
  {
    if ((n % 4) == 0) { printf("\n  "); }
    printf(" %02x%02x%02x", getc(in), getc(in), getc(in));
  }
  printf("\n");

  while (1)
  {
    parse_extensions(in);

    printf("-- image descriptor --\n");
    printf("image_separator: %c\n", getc(in));
    printf("  left_position: %d\n", read_int16(in));
    printf("   top_position: %d\n", read_int16(in));
    printf("          width: %d\n", read_int16(in));
    printf("         height: %d\n", read_int16(in));
    fields = getc(in);
    colors = 1 << ((fields & 0x7) + 1);
    if (((fields >> 7) & 1) == 0) { colors = 0; }
    printf("         fields: %d\n", fields);
    printf("          local_color_table=%d\n", (fields >> 7) & 1);
    printf("          interlace_flag=%d\n", (fields >> 6) & 1);
    printf("          sort_flag=%d\n", (fields >> 5) & 1);
    printf("          size_of_local_color_table=%d (%d)\n", fields & 0x7, colors);

    printf("-- local color table --");
    for (n = 0; n < colors; n++)
    {
      if ((n % 4) == 0) { printf("\n  "); }
      printf(" %02x%02x%02x", getc(in), getc(in), getc(in));
    }
    printf("\n");

    printf("-- image data --\n");
    printf(" minimum_code_size: %d\n", getc(in));

    while (1)
    {
      int length = getc(in);
      printf("image data block length=%d\n", length);
      if (length == 0) { break; }
      fseek(in, length, SEEK_CUR);
    }

    int marker = getc(in);
    if (marker == 0x3b) { break; }

    ungetc(marker, in);
  }

  return 0;
}

int main(int argc, char *argv[])
{
  FILE *in;

  if (argc != 2)
  {
    printf("Usage: parse_gif <filename>\n");
    exit(0);
  }

  in = fopen(argv[1], "rb");

  if (in == NULL)
  {
    printf("Error: File not found %s\n", argv[1]);
    exit(1);
  }

  int n = parse_gif(in);

  switch (n)
  {
    case -1:
      printf("Error: Premature end of file.\n");
      break;
    default:
      break;
  }

  fclose(in);

  return 0;
}

