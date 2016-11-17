// Two-Dimensional Simple Triangle
// Written by dmc 
// last-modified: Wed Sep 19 11:20:44 2012
//
// Designed to demonstrate smooth shading of vertex colors 
// by using a varying parameter in the vertex shader

#include <iostream>
#include <stdlib.h>
#include <Angel.h>

// How many points are needed to specify the triangle
// Why 6?
const int NumPoints = 28;

//Offset Location
GLint p1Loc = 0;
GLint p2Loc = 0;
GLint flagLoc = 0;
GLint time_per_frame=500;

float p1ypos = 0.0;
float p1xpos = 20.0;
float p2ypos = 0.0;
float p2xpos = 100.0;

// Data storage for our geometry for the triangles
vec3 points[NumPoints];

//returns a random float between -1 and 1
float getrand()
{
  srand(time(NULL));
  int random = rand() % 10;
  return ((2.0/random)-1.0);
} 


extern "C" void check_collision()
{
  if(p1xpos == p2xpos && p1ypos == p2ypos)
  {
    p1xpos = 20.0;
    p1ypos = 0.0;
    p2xpos = 100.0;
    p2ypos = 0.0;
  }

  glUniform3f(p1Loc, p1xpos, p1ypos, 0.1);
  glUniform3f(p2Loc, p2xpos, p2ypos, 0.1);

  glutPostRedisplay();
}

// The only required callback, it must draw everything when called.
extern "C" void display()
{
  glUniform1f(flagLoc, 0.0);
  glClear(GL_COLOR_BUFFER_BIT);          // clear the window
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);  // draw left endzone
  glDrawArrays(GL_LINES, 4, 22);      // draw yard markers
  glDrawArrays(GL_TRIANGLE_STRIP, 22, 4);
  glPointSize(20);
  glUniform1f(flagLoc, 1.0);
  glDrawArrays(GL_POINTS, 26, 1);
  glUniform1f(flagLoc, 2.0);
  glDrawArrays(GL_POINTS, 27, 1);
  glutSwapBuffers();                             // clear buffers
}




extern "C" void keys(unsigned char key, int x, int y)
{

  switch(key)
  {
    case 's':
      if(p1ypos > -1.0)
      {  
         p1ypos -= .1;
      }
      break;
    case 'w':
      if(p1ypos < 1.0)
      {  
        p1ypos += .1;
      }
      break;
    case 'a':
      if(p1xpos > 0)
      {
        p1xpos -= 10.0;
      }
      break;
    case 'd':
      if(p1xpos < 110)
      {
        p1xpos += 10.0;
      }
      else
      {
        p1xpos = 20;
      }
      break;
  }

  glUniform3f(p1Loc, p1xpos, p1ypos, 0.1);
  glutPostRedisplay();


  std::cout << "Key Press Detected, new x: " << p1xpos << std::endl;
  std::cout << "new y: " << p1ypos << std::endl;



  check_collision();
}

extern "C" void timerFunc(int value)
{
  if(p1xpos > p2xpos)
  {
    p2xpos += 10.0;
  }
  else if(p1xpos < p2xpos)
  {
    p2xpos -= 10.0;
  }

  if(p1ypos > p2ypos)
  {
    p2ypos += .1;
  }
  else if(p1ypos < p2ypos)
  {
    p2ypos -= .1;
  }


  std::cout << "P2x = " << p2xpos << std::endl;
  std::cout << "p2y = " << p2ypos << std::endl;

  glUniform3f(p2Loc, p2xpos, p2ypos, 0.1);
  glutPostRedisplay();
  glutTimerFunc(time_per_frame, timerFunc, 0);


  check_collision();
} 


// This function initializes the particular things for this program
void myinit()
{
  // First points defined:
  points[0]=vec3(  0.0,  -1, 0.0);
  points[1]=vec3(  0.0,  1,  0.0);
  points[2]=vec3(  10,   -1, 0.0);
  points[3]=vec3(  10,   1, 0.0);
  points[4]=vec3(  20,   -1, 0.0);
  points[5]=vec3(  20,   1, 0.0);
  points[6]=vec3(  30,   -1, 0.0);
  points[7]=vec3(  30,   1, 0.0);
  points[8]=vec3(  40,   -1, 0.0);
  points[9]=vec3(  40,   1, 0.0);
  points[10]=vec3(  50,   -1, 0.0);
  points[11]=vec3(  50,   1, 0.0);
  points[12]=vec3(  60,   -1, 0.0);
  points[13]=vec3(  60,   1, 0.0);
  points[14]=vec3(  70,   -1, 0.0);
  points[15]=vec3(  70,   1, 0.0);
  points[16]=vec3(  80,   -1, 0.0);
  points[17]=vec3(  80,   1, 0.0);
  points[18]=vec3(  90,   -1, 0.0);
  points[19]=vec3(  90,   1, 0.0);
  points[20]=vec3(  100,   -1, 0.0);
  points[21]=vec3(  100,   1, 0.0);
  points[22]=vec3(  110,   -1, 0.0);
  points[23]=vec3(  110,   1, 0.0);
  points[24]=vec3(  120,   -1, 0.0);
  points[25]=vec3(  120,   1, 0.0);
  points[26]=vec3( p1xpos, p1ypos, 0.1);
  points[27]=vec3( p2xpos, p2ypos, 0.1);


 
  // attributes
  glClearColor(0.2, 1.0, 0.2, 1.0); // green */
  //glClearColor(0.8, 0.8, 0.8, 0.5); // light grey background


  glutDisplayFunc(display);                      // display callback
             // invoked when
             // window opened 
  glutKeyboardFunc(keys);
  glutTimerFunc(time_per_frame, timerFunc, 0);

}

// This function initializes the buffers and shaders
void init()
{
  // Create a vertex array object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);
    
  // Create and initialize a buffer object
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points), points, GL_STATIC_DRAW);

  // Load shaders and use the resulting shader program
  GLuint program = InitShader("vshaderFootball.glsl", "fshaderSimple.glsl");
  glUseProgram(program);

  // Initialize the vertex position attribute from the vertex shader
  GLuint loc = glGetAttribLocation(program, "vPosition");
  glEnableVertexAttribArray(loc);
  glVertexAttribPointer(loc, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));

  // Initialize the colors for the vertices for the fragment shader.
  GLuint col=  glGetAttribLocation(program, "vColor");
  glEnableVertexAttribArray(col);

  // 3 points, each is 3 floating point coordinates after which is the
  // color data.
  glVertexAttribPointer(col, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(3*3*sizeof(float)));


  p1Loc = glGetUniformLocation(program, "player1");
  if (p1Loc == -1)
  {
    std::cerr << "Unable to find p1Loc parameter" << std::endl;
  }

  p2Loc = glGetUniformLocation(program, "player2");
  if (p2Loc == -1)
  {
    std::cerr << "Unable to find p2Loc param" << std::endl;
  }

  flagLoc = glGetUniformLocation(program, "flag");
  if (flagLoc == -1)
  {
    std::cerr << "Unable to find flagLoc parameter" << std::endl;
  }

  glUniform3f(p1Loc, 20, 0.0, 0.1);
  glUniform3f(p2Loc, 110, 0.0, 0.1);
  glUniform1f(flagLoc, 0.0);

  // Or you could use the following (since the color data is halfway through the vector):
  /*
  glVertexAttribPointer(col, 3, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(sizeof(points)/2));
  */
} 


int main(int argc, char** argv)
{
  // Standard GLUT initialization
  glutInit(&argc,argv);        // window title

  glutInitDisplayMode (GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize (1000, 600);
  glutCreateWindow("Football");

  // Add callbacks here, only required is displayFunc

  // Initialize the "magic" that glues all the code together.
  glewInit();

  myinit();             // set attributes
  init();               // set up shaders and display environment
  glutMainLoop();       // enter event loop

  return(EXIT_SUCCESS); // return successful exit code
}
