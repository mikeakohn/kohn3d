C?=gcc
CXX?=g++
INCLUDES=-I/usr/include/python3.10 -I../src
CFLAGS=-Wall -O3 -g

OBJECTS= \
  ../build/GifCompressor.o \
  ../build/Kohn3D.o

default:
	$(CXX) -o kohn3d.so kohn3d.cpp \
	  $(OBJECTS) \
	  -shared -fPIC $(CFLAGS) $(INCLUDES)

clean:
	@rm -f kohn3d.so

