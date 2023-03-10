C?=gcc
CXX?=g++
DEBUG=-DDEBUG -g
INCLUDES=-I..
CFLAGS=-Wall -O3 -std=c++11 $(DEBUG) $(INCLUDES)
VPATH=../src

OBJECTS= \
  ImageWriter.o \
  ImageWriterAvi.o \
  ImageWriterBmp.o \
  ImageWriterGif.o \
  Kohn3D.o

default: $(OBJECTS)
	$(CC) -o ../parse_gif ../src/parse_gif.c -Wall -O3 $(DEBUG)
	$(CC) -o ../parse_bmp ../src/parse_bmp.c -Wall -O3 $(DEBUG)
	$(CXX) -o ../libkohn3d.so $(OBJECTS) -shared -fPIC

%.o: %.cxx %.h
	$(CXX) -c $< -o $*.o \
	  $(CFLAGS) -fPIC

