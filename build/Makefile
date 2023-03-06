C?=gcc
CXX?=g++
DEBUG=-DDEBUG -g
INCLUDES=-I..
CFLAGS=-Wall -O3 -std=c++11 $(DEBUG) $(INCLUDES)
VPATH=../src

OBJECTS= \
  GifCompressor.o \
  Kohn3D.o

default: $(OBJECTS)
	$(CC) -o ../parse_gif ../src/parse_gif.c -Wall -O3 $(DEBUG)
	$(CXX) -o ../libkohn3d.so $(OBJECTS) -shared -fPIC

%.o: %.cxx %.h
	$(CXX) -c $< -o $*.o \
	  $(CFLAGS)

