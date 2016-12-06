//******************************************************************* 
//                                                                    
//  File:        Final                                           
//                                                                     
//  Author:      Brent Gruber
//  Email:       bg240711@ohio.edu
//  Class:       CS 4250 - Final Project
//                                                                    
//
//                                                                    
//  Description: This is the implementation of my final project, it is a simple 3d game that
//                will allow the user to move around the world and fight enemies by swinging a sword               
//                                                                    
//  Date:        12/6/2016
//                                                                    
//*******************************************************************

#include "./Room/room.h"
#include "./Camera/camera.h"
#include "./Bitmap/bitmap.h"
#include "./Skybox/skybox.h"
#include "./Characters/player.h"
#include "./irrklang/include/irrKlang.h"
#include <vector>
#include <pthread.h>
#include <time.h>
#include <string>
#include <fstream>
#include <list>

using std::cout;
using std::endl;
using std::vector;
using std::string;
using std::ifstream;
using std::ofstream;
using std::list;

  const int NumVertices = 36;

  //global vars
  #define NUM_TEXTURES 7
  #define TOT_TEXTURES 7
  #define M_PI  3.14159265358979323846
  #define SPEED 0.25

  //arrays to hold texture info
  BITMAPINFO *TexInfo[NUM_TEXTURES]; //texture bitmatp information
  GLubyte	 *TexBits[NUM_TEXTURES];

  //irrklang library for playing sound
  irrklang::ISoundEngine* engine;


  typedef Angel::vec4  color4;
  typedef Angel::vec4  point4;


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
  //number of enemies in house
  int numenemies;

  //list of rooms
  vector<Room> rooms;
  //list of enemies
  list<Player> enemies;

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

  //list<Room>::iterator current_room;
  int current;

//******************************************************************
//                                                                  
//  Function:   load_texture
//                                                                  
//  Purpose:    move enemies within house                                
//                                                                  
//  Parameters: n/a
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: reads an image file and creates a texture from it
//                                                                  
//******************************************************************
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
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, TexInfo[index]->bmiHeader.biWidth,
         TexInfo[index]->bmiHeader.biHeight, 0, GL_RGB,
         GL_UNSIGNED_BYTE, TexBits[index]);
  // Set up texture parameters
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

}

//******************************************************************
//                                                                  
//  Function:   set_textures
//                                                                  
//  Purpose:    generates a list of textures                              
//                                                                  
//  Parameters: n/a
//                                                                  
//
// Pre Conditions: the files used by this function exist
//
// Post Conditions: textures will be loaded and usable
//                                                                  
//******************************************************************
void set_textures()
{
  string folder = "./textures/skycube_tga/";
  string files[TOT_TEXTURES] = {"Daylight Box_Back.bmp", "Daylight Box_Bottom.bmp", "Daylight Box_Front.bmp", "Daylight Box_Left.bmp", "Daylight Box_Right.bmp", "Daylight Box_Top.bmp", "horizon.bmp"};

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

//******************************************************************
//                                                                  
//  Function:   update_title
//                                                                  
//  Purpose:    update the title of the window to inform the user of the current
//              state of the game                              
//                                                                  
//  Parameters: n/a
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: title will show information about game
//                                                                  
//******************************************************************
void update_title()
{
  int level = myPlayer->get_lev();
  int next_lev = myPlayer->get_next_lev();
  int rem_enem = enemies.size();

  char *title;
  sprintf(title, "Level: %d Experience to Next Level: %d Enemies Remaining: %d", level, next_lev, rem_enem);

  glutSetWindowTitle(title);
}


//******************************************************************
//                                                                  
//  Function:   create_player
//                                                                  
//  Purpose:    create the user player and move all the connected aspects
//              to the player's location                                
//                                                                  
//  Parameters: n/a
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: Player will be created and skybox and camera will be set based on
//                  the location
//                                                                  
//******************************************************************
void create_player()
{
    //move player to the position of the eye
  vec4 eye = vec4(0.0, 0.5, 3.0, 1.0);
  myCamera->Set_Eye(eye);
  myPlayer = new Player(myCube, 1);
  myPlayer->transform(vec3(0.0 , 0.0, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.15, 0.3, 0.15));
  myPlayer->move_player(vec3(eye.x, 0.25, eye.z+0.25));
  myPlayer->color(0.0, 1.0, 0.0);
  skyBox->set_pos(vec3(0.0, 0.0, 1.125));
}


//******************************************************************
//                                                                  
//  Function:   make_enemies
//                                                                  
//  Purpose:    initializes some enemies within the house                                 
//                                                                  
//  Parameters: myCube - cube to build the enemies from
//              offset - offset of room to put enemies in
//                                                                  
//
// Pre Conditions: no enemies
//
// Post Conditions: enemies
//                                                                  
//******************************************************************
void make_enemies(Cube *myCube, vec3 offset)
{
  //produce a random number of enemies between 0 and 10
  numenemies = (rand() % 10)+(myPlayer->get_lev() * 2);

  //create enemies
  for(int i=0; i<numenemies; i++)
  {
    float random1 = ((1.0/(rand()%20))-1)*(double)(i/2.0);
    float random2 = ((1.0/(rand()%20))-1)*(double)(i/2.0);

    int level = (rand() % 3) + myPlayer->get_lev();
    Player *player = new Player(myCube, level);
    player->transform(vec3(random1, 0.25, random2 + offset.z), vec3(0.0, 0.0, 0.0), vec3(0.15, 0.3, 0.15));
    if(level%3 == 1)
      player->color(1.0, 0.0, 0.0);
    else if(level%3 == 2)
      player->color(0.0, 1.0, 0.0);
    else if(level%3 == 3)
      player->color(0.0, 0.0, 1.0);
    enemies.push_back(*player);
  }
}

//******************************************************************
//                                                                  
//  Function:   move_enemies
//                                                                  
//  Purpose:    move enemies within house                                
//                                                                  
//  Parameters: n/a
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: enemies will move towards player
//                                                                  
//******************************************************************
void move_enemies()
{
  //used for hack of collision detection
  vec3 ploc = myPlayer->get_location();
  GLfloat dist = 0.0025;

  //move every ghost
  for(list<Player>::iterator it=enemies.begin(); it != enemies.end(); it++)
  {
    vec3 movement = vec3(0.0, 0.0, 0.0);
    vec3 location = (*it).get_location();
    bool coll = false;

    dist = 0.0025*(*it).get_lev();

    if((ploc.x - location.x) < 0.25 && (ploc.x - location.z) < 0.25)
    {

    if(location.x < ploc.x-0.075)
    {
      movement.x = dist;
    }
    else if(location.x > ploc.x+0.075)
    {
      movement.x = -dist;
    }
    else
    {
      movement.x = 0;
    }
    if(location.z < ploc.z-0.075)
    {
      movement.z = dist;
    }
    else if(location.z > ploc.z+0.075)
    {
      movement.z = -dist;
    }
    else
    {
      movement.z = 0;
    }

      if(myPlayer->Check_Collision(location, movement))
      {
        bool alive = myPlayer->take_damage((*it).get_lev()*20);
        cout << "Health = " << myPlayer->get_health() << endl;
        if(!alive)
        {
          delete myPlayer;
          create_player();
        }

        movement *= -150;
      }
    }

    (*it).trans(movement);
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
  float diroffset=0.0;

  if(movement.x < 0)
    diroffset = 90.0;
  else if(movement.x > 0)
    diroffset = -90.0;

  if(movement.z > 0)
    diroffset = 180.0;

    float rotation = myPlayer->get_rotation().y;
    rotation += diroffset;
    float direction = rotation*DegreesToRadians;

    float x = sin(direction);
    float z = cos(direction);

    movement.x = -SPEED*x;
    movement.z = -SPEED*z;

    cout << "direction = " << rotation << " movement = " << movement << endl;

    myPlayer->move_player(movement);
    skyBox->trans(movement);
    myCamera->Move_Eye(movement);
    myCamera->Move_At(movement);

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
  skyBox->transform(vec3(0.0, 0.0, 3.25), vec3(0.0, 0.0, 0.0), vec3(7.5, 7.5, 7.5));

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

  //make the enemies
  make_enemies(myCube, vec3(0.0, 0.0, 0.0));

  myCube->Identity();
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
  

  //draw enemies
  for (list<Player>::iterator it=enemies.begin(); it != enemies.end(); it++)
  {
    (*it).draw();
  }

  //draw player
  myPlayer->color(0.0, 0.5, 0.5);
  myPlayer->pdraw();
  glutSwapBuffers();
}

//******************************************************************
//                                                                  
//  Function:   playVictor
//                                                                  
//  Purpose:    play a sound file upon leveling up                                
//                                                                  
//  Parameters: n/a
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: level up sound is played then the default background music is restarted
//                                                                  
//******************************************************************
void *playVictory(void *arg1)
{
        engine->stopAllSounds();
        engine->play2D("./irrklang/media/victory.mp3", false);
        while(engine->isCurrentlyPlaying("./irrklang/media/victory.mp3"));
        engine->play2D("./irrklang/media/DarkKnight.ogg", true);
        return NULL;
}

//******************************************************************
//                                                                  
//  Function:   attack
//                                                                  
//  Purpose:    allow the user to swing the sword and attack enemies                               
//                                                                  
//  Parameters: n/a
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: if enemy is within range they will take damage
//                                                                  
//******************************************************************
void attack()
{
  myPlayer->swing_sword();
  vec4 eye = myCamera->Get_Eye();
  vec3 location = vec3(eye.x, eye.y/2.0, eye.z);
  vec3 rotation = myPlayer->get_rotation();
  vec3 movement;

  movement.y = location.y;
  movement.x = sin(rotation.y*DegreesToRadians)*-0.65;
  movement.z = cos(rotation.y*DegreesToRadians)*-0.65;

  cout << "movement = " << movement << endl;

  list<Player>::iterator it=enemies.begin();
  while(it != enemies.end())
  {
    bool alive = true;
    if((*it).Check_Collision(location, vec3(movement.x, movement.y, movement.z)))
    {
      cout << myPlayer->get_attack() << endl;
      alive = (*it).take_damage(myPlayer->get_attack());
      if(!alive)
      {
        myPlayer->enemy_killed((*it).get_lev());
        enemies.erase(it++);

      }
      else
      {
        it++;
      }
    }
    else
    {
      it++;
    }
  }

  if(enemies.empty())
  {
    make_enemies(myCube, myPlayer->get_location());
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
      //attack an enemy
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

  //see if user leveled up
  int oldlevel = myPlayer->get_lev();
  int newlevel = myPlayer->level_up();

  //update the title of the screen
  update_title();
  //move the enemies
  move_enemies();

  //if the user has leveled up then play the level up sound
  //used pthread so that execution isn't halted while waiting for sound to finish
  if(newlevel > oldlevel)
  {
        pthread_t sound;
        int x;
        pthread_create(&sound, NULL, playVictory, &x);
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
  case ' ':
    //delete all current enemies and regenerate
    enemies.clear();
    make_enemies(myCube, location);
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
  //rotate the player with the camera
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
  winw = 1200.0;
  winh = 1200.0;

  // start the sound engine with default parameters
  engine = irrklang::createIrrKlangDevice();

  if (!engine)
  {
    printf("Could not startup engine\n");
    return 0; // error starting up the engine
  }

    // play some sound stream, looped
  engine->play2D("./irrklang/media/DarkKnight.ogg", true);


  srand(time(NULL));

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutInitWindowSize(winw, winh);
  glutCreateWindow("Game");

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
