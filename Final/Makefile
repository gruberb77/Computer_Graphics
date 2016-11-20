# This sample Makefile allows you to make an OpenGL application
#   whose source is exactly one .c file or one .cc file under Solaris.
#   check the location of the X libraries on your implementation.
#
# To use this Makefile, you type:
#
#        make xxxx
#                  
# where
#       xxxx.cc is the name of the c++-file you wish to compile 
#  or 
#       xxxx.c is the name of the c-file you wish to compile 
#       
# A binary named xxxx will be produced
# Libraries are assumed to be in the default search paths
# as are any required include files

#
# Modified by Prof. Chelberg to also allow you to make from a single
# c++ source file.  So if xxxx.cc is the sourcefile, you type make
# xxxx to produce the binary file.

# last-modified: Fri Oct 16 13:01:06 2015

# Assumes the following file exists in the proper place.
include ../Makefile.txt

# The first item in the file is what will be made if you just type
# make (i.e. with no arguments).
all: house clean

#make the program
house: house.cc cube.o room.o camera.o bitmap.o skybox.o
	$(CC) house.cc  $(InitShader) cube.o room.o camera.o bitmap.o skybox.o $(OPTIONS) $(LDLIBS) -o house

# Auxilliary file for rooms.
room.o: ./Room/room.h ./Room/room.cc cube.o
	$(CC) ./Room/room.cc -c $(OPTIONS)

# Auxilliary file for cubes.
cube.o: ./Cube/cube.h ./Cube/cube.cc
	$(CC) ./Cube/cube.cc -c $(OPTIONS)

#Auxilliary file for cameras
camera.o: ./Camera/camera.h ./Camera/camera.cc
	$(CC) ./Camera/camera.cc -c $(OPTIONS)

#Auxilliary file for bitmap
bitmap.o: ./Bitmap/bitmap.h ./Bitmap/bitmap.cc
	$(CC) ./Bitmap/bitmap.cc -c $(OPTIONS)

skybox.o: ./Skybox/skybox.h ./Skybox/skybox.cc
	$(CC) ./Skybox/skybox.cc -c $(OPTIONS)


# pattern to clean the directory
clean: 
	rm -f *.o
