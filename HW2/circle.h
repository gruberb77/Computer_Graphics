#ifndef CIRCLE_H
#define CIRCLE_H
//Class definition of circle object 
// 
// Written by Prof. David M. Chelberg
// 
// Assumes that the circle object was defined in the VAO at location
// index

// last-modified: Mon Oct  8 13:06:54 2012

#include <Angel.h>
#include <iostream>

class Circle
{
public:
  // How many points are needed to specify the circle
  static GLuint NumPoints;

  // Default constructor
  Circle();

  // Constructor if start of circle vertices aren't at 0.
  Circle(GLuint nindex, vec2 *npoints, GLint noffsetLoc, GLint nsizeLoc, GLint ncolorLoc);

  // Initialize the points in the points array for circle.
  void init_points();

  // Code to call to draw a circle.
  void draw();

  // Move the circle to (x, y) in screen coordinates
  void move(GLfloat nx, GLfloat ny);

  // Move the circle to where in screen coordinates
  void move(vec2 where);

  // Change the circle's color to r, g, b value
  void color(GLfloat nr, GLfloat ng, GLfloat nb);

  // Change the size of the circle.
  void change_size(vec2 size);

  // Update the position of the circle from time
  void update();
  // Update circle's position based on ellipse code.
  void update_ellipse();

  // Change goal location for circle
  void change_goal(GLint nx, GLint ny);

  // Change goal location for circle
  void change_goal(vec2 npos);

  void change_x(float dist);

  void change_y(float dist);

  // Get the current circle's position
  vec2 get_pos();

  vec2 get_size();

  void set_ellipse_parameters(GLfloat minor_axis, GLfloat major_axis,
			      GLfloat angle_offset, GLfloat angular_velocity);

private:
  static bool inited;

  vec2 size; // half side length of circle
  // location of circle (x, y)
  GLfloat x;      
  GLfloat y; 
  // location of where circle wants to go
  GLint goal_x;
  GLint goal_y;
  // Color of circle
  GLfloat r;
  GLfloat g;
  GLfloat b;
  // Start of vertices to draw in VAO
  GLuint index;
  // Pointer to the points array
  vec2 *points;

  // Locations of variables in shaders.
  // Offset of circle
  GLint offsetLoc;
  // Size of circle
  GLint sizeLoc;
  // Color of circle 
  GLint colorLoc;
  
  GLint last_time;
  // Parameters of the ellipse movement
  GLfloat minor_axis;
  GLfloat major_axis;
  GLfloat angle_offset;
  GLfloat angular_velocity;
};

#endif
