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

static int read_int32(FILE *in)
{
  int n;

  n = getc(in);
  n |= getc(in) << 8;
  n |= getc(in) << 16;
  n |= getc(in) << 24;

  return n;
}

int parse_bmp(FILE *in)
{
  char signature[4];
  int colors;
  int n;

  memset(signature, 0, sizeof(signature));
  if (fread(signature, 1, 2, in) != 2) { return -1; }

  printf("-- header --\n");
  printf("       signature: %s\n\n", signature);
  printf("       file_size: %d\n", read_int32(in));
  printf("          unused: %d\n", read_int16(in));
  printf("          unused: %d\n", read_int16(in));
  printf("     data_offset: %d\n", read_int32(in));

  printf("-- info header --\n");
  printf("     header_size: %d\n", read_int32(in));
  printf("           width: %d\n", read_int32(in));
  printf("          height: %d\n", read_int32(in));
  printf("          planes: %d\n", read_int16(in));
  printf("  bits_per_pixel: %d\n", read_int16(in));
  printf("     compression: 0x%08x\n", read_int32(in));
  printf("      image_size: %d\n", read_int32(in));
  printf("    vertical_res: %d\n", read_int32(in));
  printf("   horzontal_res: %d\n", read_int32(in));
  colors = read_int32(in);
  printf("     colors_used: %d\n", colors);
  printf("important_colors: %d\n", read_int32(in));

  printf("-- color palette --\n");
  for (n = 0; n < colors; n++)
  {
    if ((n % 4) == 0) { printf("\n  "); }
    uint32_t color = read_int32(in);
    printf(" %08x", color);
  }
  printf("\n");

  printf("start of image: %d (0x%04x)\n", (int)ftell(in), (int)ftell(in));

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

  int n = parse_bmp(in);

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

