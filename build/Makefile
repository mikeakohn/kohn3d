C?=gcc
CXX?=g++
DEBUG=-DDEBUG -g
INCLUDES=-I..
CFLAGS=-Wall -O3 -std=c++11 $(DEBUG) $(INCLUDES)
VPATH=../src

OBJECTS= \
  ImageReader.o \
  ImageReaderBmp.o \
  ImageReaderGif.o \
  ImageWriter.o \
  ImageWriterAvi.o \
  ImageWriterBmp.o \
  ImageWriterGif.o \
  Kohn3D.o \
  PolarCoords.o \
  Picture.o \
  Texture.o

TOOLS= \
  ../parse_bmp \
  ../parse_gif \

default: $(OBJECTS)
	$(CXX) -o ../libkohn3d.so $(OBJECTS) -shared -fPIC

tools: $(TOOLS)

old:
	$(CC) -o ../parse_gif ../src/parse_gif.c -Wall -O3 $(DEBUG)
	$(CC) -o ../parse_bmp ../src/parse_bmp.c -Wall -O3 $(DEBUG)

%.o: %.cpp %.h
	$(CXX) -c $< -o $*.o \
	  $(CFLAGS) -fPIC

../%: %.c
	$(CC) -o $@ $< \
	  -Wall -O3 $(DEBUG)

