//Class implementation of circle object 
// 
// Written by Prof. David M. Chelberg, modified by Brent Gruber
// 
// Assumes that the circle object was defined in the VAO at location
// index

// last-modified: Mon Oct  8 13:06:54 2012

#include "circle.h"

bool Circle::inited=false;
GLuint Circle::NumPoints=18;

//initialize the points used to create any circle
void Circle::init_points()
{
  if (!inited && points!=NULL) {
    points[index]= vec2( 0,  0);
    GLfloat delta=(2.0*M_PI)/float(NumPoints-2);
    GLfloat angle=0.0;
    for (size_t ind = 1; ind < NumPoints; ++ind) {
      points[index+ind]=vec2(cos(angle), sin(angle));
      angle+=delta;
    }
    inited=true;
    // Actually send the data we've created to the GPU.
    //    glBufferSubData(GL_ARRAY_BUFFER, index*sizeof(vec2), NumPoints*sizeof(vec2), points);
  }
}

// Default constructor
Circle::Circle()
{
  // Default index is the start (0).
  index = 0;

  // Default Size = 30.0
  size = 30.0;

  // Default color = white.
  r=g=b=1.0;

  // Default position is (0, 0)
  x=y=0.0;
  goal_x=goal_y=0.0;

  init_points();
  last_time=glutGet(GLUT_ELAPSED_TIME);
  points=NULL;

  // Defaults
  minor_axis=100.0;
  major_axis=200.0;
  angle_offset=3.14;
  angular_velocity=1.0/230.0;
}

// Setter to modify the parameters of the ellipse orbit
void Circle::set_ellipse_parameters(GLfloat nminor_axis, GLfloat nmajor_axis,
				    GLfloat nangle_offset, 
				    GLfloat nangular_velocity)
{
  minor_axis=nminor_axis;
  major_axis=nmajor_axis;
  angle_offset=nangle_offset;
  angular_velocity=nangular_velocity;
}

// Constructor if start of circle vertices aren't at 0.
Circle::Circle(GLuint nindex, vec2 *npoints, GLint noffsetLoc, GLint nsizeLoc, GLint ncolorLoc)
{
  // Default index is the start (0).
  index = nindex;

  // Update the location of the points array in the main program.
  points = npoints;

  // Update the values of where the variables are in the shaders.
  offsetLoc=noffsetLoc;
  sizeLoc=nsizeLoc;
  colorLoc=ncolorLoc;

  // Default Size = 30.0
  size = 30.0;

  // Default color = white.
  r=g=b=1.0;

  // Default position is (0, 0)
  x=y=0.0;
  goal_x=goal_y=0.0;

  init_points();
  last_time=glutGet(GLUT_ELAPSED_TIME);
  // Defaults
  minor_axis=100.0;
  major_axis=200.0;
  angle_offset=3.14;
  angular_velocity=1.0/330.0;
}

// Code to call to draw a circle.
void Circle::draw()
{
  // Pass the current size of the circle
  glUniform2f(sizeLoc, size.x, size.y);
  glUniform2i(offsetLoc, int(x), int(y));
  // Set color on GPU
  glUniform4f(colorLoc, r, g, b, 1.0);
  //std::cout << "Drawing circle at (" << x << " " << y << ") color = (" << r << ", " << g << ", " << b << ")" << std::endl;
  glDrawArrays(GL_TRIANGLE_FAN, index, NumPoints);
}

// Move the circle to (x, y) in screen coordinates
void Circle::move(GLfloat nx, GLfloat ny)
{
  x = nx;
  y = ny;
}

// Move the circle to where in screen coordinates
void Circle::move(vec2 where)
{
  x = where.x;
  y = where.y;
}

// change the size of the circle in both the x and y direction
// make x and y same for perfect circle
void Circle::change_size(vec2 nsize)
{
  size.x = nsize.x;
  size.y = nsize.y;
}


// Get the current object's position
vec2 Circle::get_pos()
{
  return(vec2(x, y));
}

// Get the current object's size
vec2 Circle::get_size()
{
  return(size);
}

// Change the circle's color to r, g, b value
void Circle::color(GLfloat nr, GLfloat ng, GLfloat nb)
{
  r = nr; g = ng; b = nb;
}  

// Update the posiiton of the circle from time
void Circle::update()
{
  GLint time = glutGet(GLUT_ELAPSED_TIME);
  const GLfloat max_speed= 200.0/1000.0; // pixels/msec max speed
  
  vec2 dir = vec2(goal_x -x, goal_y- y);
  if (length(dir) > 20.0) {
    dir = (time - last_time)*max_speed*normalize(dir);
    // Update location
    x+=dir.x;
    y+=dir.y;
  } else {
    x = goal_x;
    y = goal_y;
  }

  move(vec2(x,y));
  last_time = time;
}


// Update the posiiton of the circle from time
void Circle::update_ellipse()
{
  GLint time = glutGet(GLUT_ELAPSED_TIME);
  x = goal_x + major_axis*sin(time*angular_velocity+angle_offset);
  y = goal_y + minor_axis*cos(time*angular_velocity+angle_offset);
  move(x, y);
  last_time = time;
}

 
// Change goal location for circle
void Circle::change_goal(GLint nx, GLint ny)
{
  goal_x = nx;
  goal_y = ny;
}

// Change goal location for circle
void Circle::change_goal(vec2 npos)
{
  goal_x = npos.x;
  goal_y = npos.y;
}

//change the x parameter of the circle by some distance, easy way to move objects
void Circle::change_x(float dist)
{
  x = x+dist;
}

//change the y parameter of the circle by some distance, easy way to move objects
void Circle::change_y(float dist)
{
  y = y+dist;
}
