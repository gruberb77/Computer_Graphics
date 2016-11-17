#ifndef RECTANGLE_H
#define RECTANGLE_H
//Class definition of square object 
// 
// Written by Prof. David M. Chelberg edited by Brent Gruber
// 
// Assumes that the square object was defined in the VAO at location
// index

// last-modified: Mon Oct  8 13:06:54 2012

#include <Angel.h>
#include <iostream>

class Rectangle
{
public:
  // How many points are needed to specify the square
  static GLint NumPoints;

  // Default constructor
  Rectangle();

  // Constructor if start of square vertices aren't at 0.
  Rectangle(GLuint nindex, vec2 *npoints, GLint noffsetLoc, GLint nsizeLoc, GLint ncolorLoc);

  // Initialize the points in the points array for square.
  void init_points();

  // Code to call to draw a square.
  void draw();

  // Move the square to (x, y) in screen coordinates
  void move(GLfloat nx, GLfloat ny);

  // Move the square to where in screen coordinates
  void move(vec2 where);

  // Change the square's color to r, g, b value
  void color(GLfloat nr, GLfloat ng, GLfloat nb);

  // Change the size of the square.
  void change_size(vec2 size);

  // Update the position of the square from time
  void update();
  // Update square's position based on ellipse code.
  void update_ellipse();

  // Change goal location for square
  void change_goal(GLint nx, GLint ny);

  // Change goal location for square
  void change_goal(vec2 npos);

  void change_x(float dist);
  
  // Get the current square's position
  vec2 get_pos();

  vec2 get_size();

  GLfloat x;      
  GLfloat y; 

private:
  static bool inited;

  vec2 size; // contains width and height
  // location of square (x, y)
  //GLfloat x;      
  //GLfloat y; 
  // location of where square wants to go
  GLint goal_x;
  GLint goal_y;
  // Color of square
  GLfloat r;
  GLfloat g;
  GLfloat b;
  // Start of vertices to draw in VAO
  GLuint index;
  // Pointer to the points array
  vec2 *points;

  // Locations of variables in shaders.
  // Offset of square
  GLint offsetLoc;
  // Size of square
  GLint sizeLoc;
  // Color of square 
  GLint colorLoc;
  
  GLint last_time;
};

#endif
