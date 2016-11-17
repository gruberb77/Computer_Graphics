//Class implementation of rectangle object 
// 
// Written by Prof. David M. Chelberg as square class
//	implementation changed to rectangle class by Brent Gruber
// 
// Assumes that the rectangle object was defined in the VAO at location
// index

// last-modified: Mon Oct 10 2016

#include "rectangle.h"

GLint Rectangle::NumPoints=4;
bool Rectangle::inited=false;

// Initialize the points used to draw any rectangle
void Rectangle::init_points()
{
  if (!inited) {
    points[0+index]= vec2(-1, -1);
    points[1+index]= vec2( 1, -1);
    points[3+index]= vec2( 1,  1);
    points[2+index]= vec2(-1,  1);
    inited=true;
    // Actually send the data we've created to the GPU.
    //    glBufferSubData(GL_ARRAY_BUFFER, index*sizeof(vec2), NumPoints*sizeof(vec2), points);
  }
}

// Default constructor
Rectangle::Rectangle()
{
  std::cout << "in Rectangle constructor\n\n";
  // Default index is the start (0).
  index = 0;
  // Default color = white.
  // Default Size = 30.0
  size.x = 30.0;
  size.y = 30.0;

  // Default color = white.
  r=g=b=1.0;

  // Default position is (0, 0)
  x=y=0.0;
  goal_x=goal_y=0.0;

  init_points();
  last_time=glutGet(GLUT_ELAPSED_TIME);
}

// Constructor if start of square vertices aren't at 0.
Rectangle::Rectangle(GLuint nindex, vec2 *npoints, GLint noffsetLoc, GLint nsizeLoc, GLint ncolorLoc)
{

  std::cout << "offset Location = " << noffsetLoc << std::endl;

  // Default index is the start (0).
  index = nindex;

  // Update the location of the points array in the main program.
  points = npoints;

  // Update the values of where the variables are in the shaders.
  sizeLoc = nsizeLoc;
  colorLoc=ncolorLoc;

  // Default Size = 30.0
  size.x = 30.0;
  size.y = 30.0;

  // Default color = white.
  r=g=b=1.0;

  // Default position is (0, 0)
  x=y=0.0;
  goal_x=goal_y=0.0;

  init_points();
  last_time=glutGet(GLUT_ELAPSED_TIME);
}

// Code to call to draw a Rectangle
void Rectangle::draw()
{
  // Pass the current size of the Rectangle
  glUniform2f(sizeLoc, size.x, size.y);
  //p
  glUniform2i(offsetLoc, int(x), int(y));
  // Set color on GPU
  glUniform4f(colorLoc, r, g, b, 1.0);
  glDrawArrays(GL_TRIANGLE_STRIP, index, NumPoints);
}

// Move the square to (x, y) in screen coordinates
void Rectangle::move(GLfloat nx, GLfloat ny)
{
  std::cout << "In move, nx = " << nx << " ny = " << ny << std::endl;

  x = nx;
  y = ny;
}

// Move the square to where in screen coordinates
void Rectangle::move(vec2 where)
{
  std::cout << "In move 2, nx = " << where.x << " ny = " << where.y << std::endl;

  x = where.x;
  y = where.y;
}

// Get the current object's position
vec2 Rectangle::get_pos()
{
  return(vec2(x, y));
}

// Get the current object's size
vec2 Rectangle::get_size()
{
  return(size);
}

// Change the square's color to r, g, b value
void Rectangle::color(GLfloat nr, GLfloat ng, GLfloat nb)
{
  r = nr; g = ng; b = nb;
}  

// Change the size of the square.
void Rectangle::change_size(vec2 nsize)
{
  size = nsize;
}

// Update the position of the square from time
void Rectangle::update()
{
  GLint time = glutGet(GLUT_ELAPSED_TIME);
  const GLfloat max_speed= 200.0/1000.0; // pixels/msec max speed
  
  vec2 dir = vec2(goal_x -x, goal_y- y);
  if (length(dir) > 2.0) {
    dir = (time - last_time)*max_speed*normalize(dir);
    // Update location
    x+=dir.x;
    y+=dir.y;
  } else {
    x = goal_x;
    y = goal_y;
  }
  last_time = time;
}


// Update the position of the square from time
void Rectangle::update_ellipse()
{
  GLint time = glutGet(GLUT_ELAPSED_TIME);

  // What are 300.0 and 200.0?  What do they affect?
  x = goal_x + 300.0*sin(time/300.0);
  y = goal_y + 200.0*cos(time/300.0);
  move(x, y);
  last_time = time;
}

 
// Change goal location for square
void Rectangle::change_goal(GLint nx, GLint ny)
{
  goal_x = nx;
  goal_y = ny;
}

// Change goal location for square
void Rectangle::change_goal(vec2 npos)
{
  goal_x = npos.x;
  goal_y = npos.y;
}

// Change the x position of the rectangle
void Rectangle::change_x(float dist)
{
  x = x+dist;
}
