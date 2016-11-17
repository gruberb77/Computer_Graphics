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
proj: house.o
	$(CC) house.o $(OPTIONS) $(LDLIBS) -o house

house.o: house.cc functions.o
	$(CC) house.cc $(InitShader) functions.o $(OPTIONS) $(LDLIBS)

functions.o: functions.h functions.cc room.o cube.o camera.o
	$(CC) functions.cc cube.o room.o camera.o -c $(OPTIONS)

# Auxilliary file for rooms.
room.o: room.h room.cc cube.o
	$(CC) room.cc -c $(OPTIONS)

# Auxilliary file for cubes.
cube.o: cube.h cube.cc
	$(CC) cube.cc -c $(OPTIONS)

#Auxilliary file for cameras
camera.o: camera.h camera.cc
	$(CC) camera.cc -c $(OPTIONS)



# pattern to clean the directory
clean: 
	rm -f house *.o
