//
//  Display a rotating cube with lighting
//
//  Light and material properties are sent to the shader as uniform
//    variables.  Vertex positions and normals are sent after each
//    rotation.
//
// Modified by David M. Chelberg for style
// last-modified: Tue Oct 16 16:14:55 2012

#include "functions.h"

//----------------------------------------------------------------------------
// OpenGL initialization
void init()
{
  // Load shaders and use the resulting shader program
  GLuint program = InitShader("vshader41.glsl", "fshader41.glsl");
  glUseProgram(program);
		 
  // Retrieve transformation uniform variable locations
  GLuint CameraView = glGetUniformLocation(program, "camera_view");
  GLuint ModelView = glGetUniformLocation(program, "model_view");
  Projection = glGetUniformLocation(program, "projection");
  GLuint vPosition = glGetAttribLocation(program, "vPosition");
  vColor = glGetAttribLocation(program, "vColor"); 

  myCube = new Cube(0, points, ModelView);

  // Create a vertex array object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create and initialize a buffer object
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(colors),
	       NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(points),
		  sizeof(colors), colors);

  // set up vertex array
  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));

  glEnable(GL_DEPTH_TEST);
  glClearColor(1.0, 1.0, 1.0, 1.0); 


  GLfloat aspect = GLfloat(winw)/winh;
  mat4  projection = Perspective(fovy, aspect, zNear, zFar);

  glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);

  myCamera = new Camera(CameraView);
  myPlayer = new Cube((*myCube));

  create_rooms();

  vec4 eye = myCamera->Get_Eye();
  myPlayer->transform(vec3(eye.x , 0.25, eye.z+0.15), vec3(0.0, 0.0, 0.0), vec3(0.15, 0.3, 0.15));
  myPlayer->color(0.0, 0.0, 0.0);
}

//----------------------------------------------------------------------------
extern "C" void display()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  myCamera->Update_Camera();

  for (int i=0; i<numrooms; i++)
  {
	   rooms[i].draw();
  }
  for (list<Cube>::iterator it = ghosts.begin(); it != ghosts.end(); ++it)
  {
    it->draw();
  }

  glUniform4f(vColor, 0.0, 0.0, 0.0, 0.0);
  myPlayer->draw();
  glutSwapBuffers();
}

//----------------------------------------------------------------------------
extern "C" void mouse(int button, int state, int x, int y)
{
  if (state == GLUT_DOWN) {
    switch(button) {
    case GLUT_LEFT_BUTTON:  
			open_doors();  
			break;
    case GLUT_MIDDLE_BUTTON:  
			break;
    case GLUT_RIGHT_BUTTON: 
			myCamera->Toggle_Pov(); 

			break;
    }
  }

  glutPostRedisplay();
}

//----------------------------------------------------------------------------
extern "C" void idle()
{
  new_time = glutGet(GLUT_ELAPSED_TIME);
  delta = new_time - lasttime;
  lasttime = new_time;

  //move_ghosts();

  glutPostRedisplay();
}

//----------------------------------------------------------------------------
extern "C" void keyboard(unsigned char key, int x, int y)
{

  GLfloat dist = 0.1;

  vec3 movement = vec3(0.0, 0.0, 0.0);
  vec3 location = myPlayer->get_location();

  switch(key) {
  case 033: // Escape Key
  case 'q': case 'Q':
    exit(EXIT_SUCCESS);
    break;
  case 'w': case 'W':
    movement.z = -dist;
    location.z -= 0.075;
    break;
  case 's': case 'S':
    movement.z = dist;
    location.z += 0.075;
    break;
  case 'a': case 'A':
    movement.x = -dist;
    location.x -= 0.075;
    break;
  case 'd': case 'D':
    movement.x = dist;
    location.x += 0.075;
    break;
  case ' ':
    myCamera->Reverse();
    break;
  case 'v':
    myPlayer->color(0.1, 0.1, 0.1);
    break;
  }

  move(location, movement);
 
  glutPostRedisplay();
}


// Special Keys events.
// Left and Right arrows will move plane, down arrow drops food
extern "C" void special(int k, int xx, int yy)
{
  int speed = 10;

  if (k==GLUT_KEY_UP) {
    myCamera->Move_At(0.0, 0.2, 0.0);
  } else if (k==GLUT_KEY_DOWN) {
    myCamera->Move_At(0.0, -0.2, 0.0);
  } else if (k==GLUT_KEY_LEFT) {
    myCamera->Move_At(-0.2, 0.0, 0.0);
  } else if (k==GLUT_KEY_RIGHT) {
    myCamera->Move_At(0.2, 0.0, 0.0);
  }

  glutPostRedisplay();

}


extern "C" void passivemotion(int x, int y)
{

  //angle to rotate by
  GLfloat angle = 180.0;
  //middle of screen
  float midx = winw/2.0;
  float midy = winh/2.0;
  //distance of mouse from cetner
  float dx = (x-midx)/winw;
  float dy = -(y-midy)/winh;

  float theta = angle * dx * DegreesToRadians;
  float phi = angle * dy * DegreesToRadians;

  myCamera->Move_At(theta, phi);

  glutPostRedisplay();
}

//----------------------------------------------------------------------------
extern "C" void reshape(int width, int height)
{
  glViewport(0, 0, width, height);

  GLfloat aspect = GLfloat(width)/height;
  mat4  projection = Perspective(45.0, aspect, 0.1, 5.0);
  //mat4  projection = Ortho(-1, 1, -1, 1, -2, 2);

  glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);
}

//----------------------------------------------------------------------------
int main(int argc, char **argv)
{
  winw = 800.0;
  winh = 800.0;

  numrooms = 6;

  srand(time(NULL));

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(winw, winh);
  glutCreateWindow("Color Cube");

  glewInit();

  init();

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutIdleFunc(idle);
  glutPassiveMotionFunc(passivemotion);
  glutSpecialFunc(special);

  glutMainLoop();
  return(EXIT_SUCCESS);
}
