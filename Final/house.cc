//******************************************************************* 
//                                                                    
//  File:        House                                           
//                                                                     
//  Author:      Brent Gruber
//  Email:       bg240711@ohio.edu
//                                                                    
//
//                                                                    
//  Description: This is the implementation of a (haunted) house, with some interaction and glut 
//               callbacks                
//                                                                    
//  Date:        11/1/2016
//                                                                    
//*******************************************************************

#include "./Room/room.h"
#include "./Camera/camera.h"
#include "./Bitmap/bitmap.h"
#include "./Skybox/skybox.h"
#include "./Characters/player.h"
#include <vector>
#include <time.h>
#include <string>
#include <fstream>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::ifstream;
using std::ofstream;

  const int NumVertices = 36;

  #define NUM_TEXTURES 7
  #define TOT_TEXTURES 7
  BITMAPINFO *TexInfo[NUM_TEXTURES]; //texture bitmatp information
  GLubyte	 *TexBits[NUM_TEXTURES];

  typedef Angel::vec4  color4;
  typedef Angel::vec4  point4;

  //ceiling fan
  struct Fan
  {
    Cube    *blade1;
    Cube    *blade2;
    bool    spin;
  };

  //points array used as VAO
  point4 points[NumVertices];
  vec3 normals[NumVertices];
  color4 colors[36];
  GLuint textures[TOT_TEXTURES];

  vec2 tex_coords[NumVertices];
  float *z_vals;

  const int TextureSize = 64;
  GLubyte image[TextureSize][TextureSize][3];
  GLubyte image2[TextureSize][TextureSize][3];

  	// Size of input heightmap
	size_t Width;
	size_t Height;

  //number of rooms in house
  int numrooms;
  //number of ghosts in house
  int numghosts;

  //list of rooms
  vector<Room> rooms;
  //list of ghosts
  vector<Player> ghosts;

  // Adjust this value for your taste (to speed up, make bigger, to slow
  // down rotation, make smaller
  GLfloat incr =-0.06;
  GLint lasttime = 0;
  GLint new_time = 0;
  GLint delta = 0;
  GLint program;

  //width and height of window
  float	winh;
  float	winw;

  // Model-view and projection matrices uniform location
  GLuint  Projection, vColor;

  //perspective values
  GLfloat  fovy = 45.0;
  GLfloat  aspect;
  GLfloat  zNear = 0.5, zFar = 25.0;

  //pointers for objects used
  Cube	   *myCube;
  Player   *myPlayer;
  Skybox   *skyBox;
  Room	   *myRoom;
  Camera   *myCamera;
  Fan      *myFan;

  //list<Room>::iterator current_room;
  int current;

void load_texture(int index, string filename)
{
  // Set which texture subsequent commands will apply to.
  glBindTexture(GL_TEXTURE_2D, textures[index]);
  // Read the image file.
  TexBits[index] = LoadDIBitmap(filename.c_str(), &TexInfo[index]);
  // Check for failure (null pointer returned).
  if (TexBits[index] == 0) {
    std::cout << "Error reading in texture file. Filename =" << filename << std::endl;
    exit(EXIT_FAILURE);
  }
  // Send the texture image to the graphics card.
  glTexImage2D(GL_TEXTURE_2D, 0, 3, TexInfo[index]->bmiHeader.biWidth,
         TexInfo[index]->bmiHeader.biHeight, 0, GL_RGB,
         GL_UNSIGNED_BYTE, TexBits[index]);
  // Set up texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

}


void set_textures()
{
  string folder = "./textures/skycube_tga/";
  string files[TOT_TEXTURES] = {"sky1.bmp", "sky2.bmp", "sky3.bmp", "sky4.bmp", "sky5.bmp", "sky6.bmp", "horizon.bmp"};

  glGenTextures(6, textures);

  for(int i=0; i<TOT_TEXTURES; i++)
  {
    load_texture(i, folder+files[i]);
  }

  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, textures[0]);
}

void create_player()
{
    //move player to the position of the eye
  vec4 eye = vec4(0.0, 0.5, 3.0, 1.0);
  myCamera->Set_Eye(eye);
  myPlayer = new Player(myCube, 1);
  myPlayer->transform(vec3(0.0 , 0.0, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.15, 0.3, 0.15));
  myPlayer->move_player(vec3(eye.x, 0.25, eye.z+0.25));
  myPlayer->color(0.0, 1.0, 0.0);
  skyBox->set_pos(vec3(0.0, 7.5, 0.0));
}


//******************************************************************
//                                                                  
//  Function:   make_ghosts
//                                                                  
//  Purpose:    initializes some ghosts within the house                                 
//                                                                  
//  Parameters: myCube - cube to build the ghosts from
//              offset - offset of room to put ghosts in
//                                                                  
//
// Pre Conditions: no ghosts
//
// Post Conditions: ghosts
//                                                                  
//******************************************************************
void make_ghosts(Cube *myCube, vec3 offset)
{
  //produce a random number of ghosts between 0 and 10
  numghosts = rand() % 10;

  //create ghosts
  for(int i=0; i<numghosts; i++)
  {
    float random1 = (2.0/(rand()%20))-1;
    float random2 = (2.0/(rand()%20))-1;

    int level = (rand() % 3) + 1;
    Player *player = new Player(myCube, level);
    player->transform(vec3(random1, 0.25, random2 + offset.z), vec3(0.0, 0.0, 0.0), vec3(0.15, 0.3, 0.15));
    if(level == 1)
      player->color(1.0, 0.0, 0.0);
    else if(level == 2)
      player->color(0.0, 1.0, 0.0);
    else if(level == 3)
      player->color(0.0, 0.0, 1.0);
    ghosts.push_back(*player);
  }
}

//******************************************************************
//                                                                  
//  Function:   move_ghosts
//                                                                  
//  Purpose:    move ghosts within house                                
//                                                                  
//  Parameters: n/a
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: ghosts will move towards player
//                                                                  
//******************************************************************
void move_ghosts()
{
  //used for hack of collision detection
  vec3 noffset = rooms[numrooms-1].Get_Offset();
  vec3 scale = vec3(1.0, 1.0, 1.0*numrooms);
  vec3 ploc = myPlayer->get_location();
  GLfloat dist = 0.01;

  //move every ghost
  for(int i=0; i<numghosts; i++)
  {
    vec3 movement;
    vec3 location = ghosts[i].get_location();
    bool coll = false;

    //quick collision hack, allow ghosts to walk through internal walls
    if(location.x <= (noffset.x-scale.x) || location.x > (noffset.x+scale.x) || location.z < (noffset.z-scale.z) || location.z > (noffset.z+scale.z))
      coll = true;

      if(ploc.z > location.z)
        movement.z = dist;
      else
        movement.z = -dist;

    //no collision, continue with movement
    if(!coll)
    {
      ghosts[i].trans(movement);
    }
    else //collision move somewhere else
    {
      movement.x *= -1;
      movement.z *= -1;
      ghosts[i].trans(movement*-1);
    }
  }
}

//******************************************************************
//                                                                  
//  Function:   create_rooms
//                                                                  
//  Purpose:    initializes rooms within a house                              
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: a vector of rooms will be created
//                                                                  
//******************************************************************
void create_rooms()
{
  vec3 offset = vec3(0.0, 0.0, 0.0);

  //create first room explicitly
  myRoom = new Room(offset, myCube);
  rooms.push_back(*myRoom);

  //create the rest of the rooms
  for(int i=1; i<numrooms; i++)
  {
    offset.z -= 2.0;
    myRoom = new Room(offset, myCube);
    rooms.push_back(*myRoom);

    rooms[i-1].nextRoom = &rooms[i];
  }


  current = 0;
}

//******************************************************************
//                                                                  
//  Function:   open_doors
//                                                                  
//  Purpose:    open the door to the next room if applicable                             
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: player exists in a room with a door to the next room available
//
// Post Conditions: door will open if it is in the line of sight
//                                                                  
//******************************************************************
void open_doors()
{
  //get what the player is looking at, only open the door if it is being pointed at
  vec4 sight = myCamera->Get_At();
  vec4 player = myCamera->Get_Eye();

  //make sure it is not the last room which has no door
  if(current != (numrooms-1))
    rooms[current+1].Toggle_Door(vec3(sight.x, sight.y, player.z));  //open the door
}


//******************************************************************
//                                                                  
//  Function:   move
//                                                                  
//  Purpose:    moves the player                            
//                                                                  
//  Parameters: location - location of player
//              movement - intended movement vector
//                                                                  
//
// Pre Conditions: user has create a movement using inpu
//
// Post Conditions: player will move if no collisions
//                                                                  
//******************************************************************
void move(vec3 location, vec3 movement)
{
  //list<Room>::iterator next = current_room;


  //check for collisions before moving

    myPlayer->move_player(movement);
    skyBox->trans(movement);
    myCamera->Move_Eye(movement);
    myCamera->Move_At(movement);


    for(vector<Player>::iterator it = ghosts.begin(); it != ghosts.end(); ++it)
    {
      if((*it).Check_Collision(location, movement))
      {
        bool alive = myPlayer->take_damage(20);
        cout << "Health = " << myPlayer->get_health() << endl;
        if(!alive)
        {
          delete myPlayer;
          create_player();
        }
      }
    }
  

}

//******************************************************************
//                                                                  
//  Function:   interact
//                                                                  
//  Purpose:    interact with the world                             
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: player has attempted to interact with something
//
// Post Conditions: if possible interaction found, execute it
//                                                                  
//******************************************************************
void interact()
{
  vec4 sight = myCamera->Get_At();
  vec4 player = myCamera->Get_Eye();

  cout << "player =  " << sight << endl;
  cout << "sight = " << sight*5 << endl;

  if(myFan->blade1->Check_Collision(vec3(player.x, player.y, player.z), vec3(sight.x*5, sight.y*5, sight.z*5)) || 
    myFan->blade2->Check_Collision(vec3(player.x, player.y, player.z), vec3(sight.x*5, sight.y*5, sight.z*5)))
  {
    myFan->spin = !myFan->spin;
  }
}

//******************************************************************
//                                                                  
//  Function:   spin_fan
//                                                                  
//  Purpose:    spin a ceiling fan                              
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: if a ceiling fan is set to spin, it will spin
//                                                                  
//******************************************************************
void spin_fan()
{
  GLfloat angle = 10.0;

  if(myFan->spin)
  {
    myFan->blade1->trans(vec3(0.0, 0.0, 2.0));
    myFan->blade1->rot(vec3(0.0, angle, 0.0));
    myFan->blade1->trans(vec3(0.0, 0.0, -2.0));
    myFan->blade2->trans(vec3(0.0, 0.0, 2.0));
    myFan->blade2->rot(vec3(0.0, angle, 0.0));
    myFan->blade2->trans(vec3(0.0, 0.0, -2.0));
  }
}



//******************************************************************
//                                                                  
//  Function:   init
//                                                                  
//  Purpose:    initializes openGL features, and objects to be drawn                           
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: openGL will be configured for use, objects in world exist
//                                                                  
//******************************************************************
void init()
{

  set_textures();

  // Load shaders and use the resulting shader program
  program = InitShader("./shaders/vshader72.glsl", "./shaders/fshader72.glsl");
  glUseProgram(program);
		 
  // Retrieve transformation uniform variable locations
  GLuint CameraView = glGetUniformLocation(program, "camera_view");
  GLuint ModelView = glGetUniformLocation(program, "model_view");
  Projection = glGetUniformLocation(program, "projection");
  GLuint vPosition = glGetAttribLocation(program, "vPosition");
  vColor = glGetUniformLocation(program, "vColor"); 
  GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");

  //create the camera
  myCamera = new Camera(CameraView);
  myCube = new Cube(0, points, normals, ModelView, vColor);
  skyBox= new Skybox(0, points, tex_coords, ModelView, vColor, textures);
  skyBox->transform(vec3(0.0, 7.5, 0.0), vec3(0.0, 0.0, 0.0), vec3(15.0, 15.0, 15.0));

  create_player();

  // Create a vertex array object
  GLuint vao;
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  // Create and initialize a buffer object
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(points) + sizeof(tex_coords),
	       NULL, GL_STATIC_DRAW);
  glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(points), points);
  glBufferSubData(GL_ARRAY_BUFFER, sizeof(points),
		  sizeof(tex_coords), tex_coords);

  // set up vertex arrays
  glEnableVertexAttribArray(vPosition);
  glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(0));

  glEnableVertexAttribArray(vTexCoord);
  glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, 0,
			BUFFER_OFFSET(sizeof(points)));

  glEnable(GL_DEPTH_TEST);
  glClearColor(1.0, 1.0, 1.0, 1.0); 

  GLfloat aspect = GLfloat(winw)/winh;
  mat4  projection = Perspective(fovy, aspect, zNear, zFar);
  //mat4  projection = Ortho(-1, 1, -1, 1, -2, 2);

  glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);

  //create the rooms
  //create_rooms();

  //make the ghosts
  make_ghosts(myCube, vec3(0.0, 0.0, 0.0));

  myCube->Identity();

  myFan = new Fan();
  myFan->blade1 = new Cube(*myCube);
  myFan->blade1->transform(vec3(0.0, 1.9, -2.0), vec3(0.0, 0.0, 0.0), vec3(0.1, 0.1, 0.5));

  myFan->blade2 = new Cube(*myCube);
  myFan->blade2->transform(vec3(0.0, 1.9, -2.0), vec3(0.0, 0.0, 0.0), vec3(0.5, 0.1, 0.1));
}

//******************************************************************
//                                                                  
//  Function:   display
//                                                                  
//  Purpose:    draw the world                             
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: user will be able to see all objects to be displayed
//                                                                  
//******************************************************************
extern "C" void display()
{
  glUniform1i(glGetUniformLocation(program, "text"), 1);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //move camera
  myCamera->Update_Camera();

  skyBox->draw();

  glUniform1i(glGetUniformLocation(program, "text"), 0);
  

  //draw ghosts
  for (vector<Player>::iterator it=ghosts.begin(); it != ghosts.end(); it++)
  {
    (*it).draw();
  }

  myFan->blade1->draw();
  myFan->blade2->draw();

  //draw player
  myPlayer->color(0.0, 0.5, 0.5);
  myPlayer->pdraw();
  glutSwapBuffers();
}


void attack()
{
  myPlayer->swing_sword();
  vec3 location = myPlayer->get_location();
  vec4 movement = myCamera->Get_At();
  for (vector<Player>::iterator it=ghosts.begin(); it != ghosts.end(); it++)
  {
    if((*it).Check_Collision(location, vec3(movement.x, movement.y, movement.z)))
    {
      bool alive = (*it).take_damage(500);
      cout << "hit, health = " << (*it).get_health() << endl;
    }
  }
}

//******************************************************************
//                                                                  
//  Function:   mouse
//                                                                  
//  Purpose:    to take input from user using the mouse buttons and position                         
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: user presses a mouse button
//
// Post Conditions: resulting functionality for button will occur
//                                                                  
//******************************************************************
extern "C" void mouse(int button, int state, int x, int y)
{
  if (state == GLUT_DOWN) {
    switch(button) {
    case GLUT_LEFT_BUTTON:  
      //open door in front of player
      attack();
			break;
    case GLUT_MIDDLE_BUTTON:  
			break;
    case GLUT_RIGHT_BUTTON: 
      //toggle point of view
			myCamera->Toggle_Pov(); 
			break;
    }
  }

  glutPostRedisplay();
}

//******************************************************************
//                                                                  
//  Function:   idle
//                                                                  
//  Purpose:    run while nothing else is happening                           
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: no other events happening
//
// Post Conditions: animations
//                                                                  
//******************************************************************
extern "C" void idle()
{
  new_time = glutGet(GLUT_ELAPSED_TIME);
  delta = new_time - lasttime;
  lasttime = new_time;

  //move the ghosts constantly
  //move_ghosts();
  spin_fan();

  for(vector<Player>::iterator it = ghosts.begin(); it != ghosts.end(); ++it)
  {
    if((*it).get_health() <= 0)
    {
      ghosts.erase(it);
    }
  }

  if(ghosts.empty())
  {
    make_ghosts(myCube, vec3(0.0, 0.0, 0.0));
  }

  glutPostRedisplay();
}

//******************************************************************
//                                                                  
//  Function:   keyboard
//                                                                  
//  Purpose:    take input from keyboard                             
//                                                                  
//  Parameters: key - key pressed
//              x - position of cursor
//              y - position of cursor                               
//
// Pre Conditions: key was pressed
//
// Post Conditions: functionality for key is executed
//                                                                  
//******************************************************************
extern "C" void keyboard(unsigned char key, int x, int y)
{
  //distance to move
  GLfloat dist = 0.1;

  //movement vector
  vec3 movement = vec3(0.0, 0.0, 0.0);
  //location of player
  vec3 location = myPlayer->get_location();

  //edit the movement vector
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
   case '1':
   	glBindTexture(GL_TEXTURE_2D, textures[0]);
   	break;
   case '2':
   	glBindTexture(GL_TEXTURE_2D, textures[1]);
   	break;
   case '3':
   	glBindTexture(GL_TEXTURE_2D, textures[2]);
   	break;
   case '4':
   	glBindTexture(GL_TEXTURE_2D, textures[3]);
   	break;
  case ' ':
    //reverse the camera TODO: not working
    myCamera->Reverse();
    break;
  case 'v':
    //change the color of the player, TODO: can't change back
    myPlayer->color(1.0, 1.0, 1.0);
    break;
  }

  //move the player based on the movement vector
  move(location, movement);
 
  glutPostRedisplay();
}

//******************************************************************
//                                                                  
//  Function:   special
//                                                                  
//  Purpose:    used for special keys that don't have ascii mappings                             
//                                                                  
//  Parameters: k - key pressed
//              xx - cursor
//              yy - cursor
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: key functionality executed
//                                                                  
//******************************************************************
extern "C" void special(int k, int xx, int yy)
{
  int speed = 10;

  //move where teh player is looking, use mouse, works better
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

//******************************************************************
//                                                                  
//  Function:   passivemotion
//                                                                  
//  Purpose:    used for mouse when no button pressed                             
//                                                                  
//  Parameters: x - cursor
//              y - cursor
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: move camera
//                                                                  
//******************************************************************
extern "C" void passivemotion(int x, int y)
{

  //angle to rotate by
  GLfloat angle = 360.0;
  //middle of screen
  float midx = winw/2.0;
  float midy = winh/2.0;
  //distance of mouse from cetner
  float dx = (x-midx)/winw;
  float dy = -(y-midy)/winh;

  //angles used to emulate trackball
  float theta = angle * dx * DegreesToRadians;
  float phi = angle * dy * DegreesToRadians;

  //update the at vector
  myCamera->Move_At(theta, phi);
  myPlayer->rotate_player(angle * dx);

  glutPostRedisplay();
}

//******************************************************************
//                                                                  
//  Function:   reshape
//                                                                  
//  Purpose:    used when user reshapes the window                            
//                                                                  
//  Parameters: width - new width
//              height - new height
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: window is reshaped
//                                                                  
//******************************************************************
extern "C" void reshape(int width, int height)
{
  winw = width;
  winh = height;

  glViewport(0, 0, width, height);

  GLfloat aspect = GLfloat(width)/height;
  mat4  projection = Perspective(45.0, aspect, 0.1, 20.0);

  glUniformMatrix4fv(Projection, 1, GL_TRUE, projection);
}

//******************************************************************
//                                                                  
//  Function:   main
//                                                                  
//  Purpose:    main                            
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: 
//                                                                  
//******************************************************************
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
