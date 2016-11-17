// 
// Written by Prof. David M. Chelberg as Circle Class, 
// edited to Triangle class by Brent Gruber
// 
// Assumes that the triangle object was defined in the VAO at location
// index
// 
// this class will draw an equilateral triangle of any size or location

// last-modified: Mon Oct  8 13:06:54 2012

#include "triangle.h"

bool Triangle::inited=false;
GLint Triangle::NumPoints=3;

//initialize points used to draw any triangle
void Triangle::init_points()
{
  if (!inited && points!=NULL) {
    points[0+index]= vec2(-0.5, 0);
    points[1+index]= vec2(0.5, 0);
    points[2+index]= vec2(0, 1.0);
    inited=true;
    // Actually send the data we've created to the GPU.
    //    glBufferSubData(GL_ARRAY_BUFFER, index*sizeof(vec2), NumPoints*sizeof(vec2), points);
  }
}

// Default constructor
Triangle::Triangle()
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
}

// Setter to modify the parameters of the ellipse orbit
void Triangle::update_ellipse()
{
  GLint time = glutGet(GLUT_ELAPSED_TIME);

  // What are 300.0 and 200.0?  What do they affect?
  x = goal_x + 300.0*sin(time/300.0);
  y = goal_y + 200.0*cos(time/300.0);
  move(x, y);
  last_time = time;
}

// Constructor if start of triangle vertices aren't at 0.
Triangle::Triangle(GLuint nindex, vec2 *npoints, GLint noffsetLoc, GLint nsizeLoc, GLint ncolorLoc)
{
  // Default index is the start (0).
  index = nindex;
  std::cout << "index = " << index << std::endl;

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
}

// Code to call to draw a triangle.
void Triangle::draw()
{
  // Pass the current size of the triangle
  glUniform2f(sizeLoc, size, size);
  glUniform2i(offsetLoc, int(x), int(y));
  // Set color on GPU
  glUniform4f(colorLoc, r, g, b, 1.0);
  //std::cout << "drawing triangle at (" << x << " " << y << ")" << std::endl;
  glDrawArrays(GL_TRIANGLES, index, NumPoints);

}

// Move the triangle to (x, y) in screen coordinates
void Triangle::move(GLfloat nx, GLfloat ny)
{
  std::cout << "In move, nx = " << nx << " ny = " << ny << std::endl;
  x = nx;
  y = ny;
}

// Move the circle to where in screen coordinates
void Triangle::move(vec2 where)
{
  x = where.x;
  y = where.y;
}


// Get the current object's position
vec2 Triangle::get_pos()
{
  return(vec2(x, y));
}

// Get the current object's size
vec2 Triangle::get_size()
{
  return(size);
}

// Change the triangle's color to r, g, b value
void Triangle::color(GLfloat nr, GLfloat ng, GLfloat nb)
{
  r = nr; g = ng; b = nb;
}  

// Change the size of the triangle
void Triangle::change_size(GLfloat nsize)
{
  size = nsize;
}

// Update the posiiton of the triangle from time
void Triangle::update()
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
  move(x, y);
  last_time = time;
}



 
// Change goal location for triangle
void Triangle::change_goal(GLint nx, GLint ny)
{
  goal_x = nx;
  goal_y = ny;
}

// Change goal location for triangle
void Triangle::change_goal(vec2 npos)
{
  goal_x = npos.x;
  goal_y = npos.y;
}
