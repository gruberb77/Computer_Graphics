# This sample Makefile allows you to make an OpenGL application
#   whose source is in many .cc files.
#   check the location of the X libraries on your implementation.
#
# To use this Makefile, you type:
#
#        make 
#                  
# Libraries are assumed to be in the default search paths as are any
# required include files

# Modified by dmc to allow use of multiple files for a project.
# last-modified: Fri Sep 26 15:35:29 2014

# Assumes the following file exists in the proper place.
include ./Makefile.txt

# The first item in the file is what will be made if you just type
# make (i.e. with no arguments).
animate: animate.cc rectangle.o circle.o triangle.o
	$(CC) animate.cc  $(InitShader) rectangle.o circle.o triangle.o $(OPTIONS) $(LDLIBS) -o animate

# Auxilliary file for rectangle.
rectangle.o: rectangle.h rectangle.cc
	$(CC) rectangle.cc -c $(OPTIONS)

# Auxilliary file for circle.
circle.o: circle.h circle.cc
	$(CC) circle.cc -c $(OPTIONS)

# Auxilliary file for triangle.
triangle.o: triangle.h triangle.cc
	$(CC) triangle.cc -c $(OPTIONS)

# pattern to clean the directory
clean: 
	rm -f animate rectanlge.o circle.o
