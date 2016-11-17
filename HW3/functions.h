#ifndef FUNCTIONS_H
#define FUNCTIONS_H


#include "room.h"
#include "camera.h"
#include <list>
#include <vector>
#include <time.h>

using std::cout;
using std::endl;
using std::list;
using std::vector;

  typedef Angel::vec4  color4;
  typedef Angel::vec4  point4;

  const int NumVertices = 36; //(6 faces)(2 triangles/face)(3 vertices/triangle)
  const vec3 Offset3rd = vec3(0.0, 0.25, 1.5); //how far the camera will be from the player in 3rd person

  point4 points[NumVertices];
  color4 colors[NumVertices];

  int numrooms;
  vector<Room> rooms;
  list<Cube> ghosts;

  // Adjust this value for your taste (to speed up, make bigger, to slow
  // down rotation, make smaller
  GLfloat incr =-0.06;
  GLint lasttime = 0;
  GLint new_time = 0;
  GLint delta = 0;

  float	winh;
  float	winw;

  // Model-view and projection matrices uniform location
  GLuint  Projection, vColor;

  GLfloat  fovy = 45.0;
  GLfloat  aspect;
  GLfloat  zNear = 0.5, zFar = 3.0;

  Cube	   *myCube;
  Cube     *myPlayer;
  Room	   *myRoom;
  Camera   *myCamera;

  //list<Room>::iterator current_room;
  int current;

void make_ghosts(Cube *myCube, vec3 offset);
vec3 get_mov();
void move_ghosts();
void create_rooms();
void open_doors();
void move(vec3 location, vec3 movement);


#endif