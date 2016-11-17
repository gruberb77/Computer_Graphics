#include "functions.h"

void make_ghosts(Cube *myCube, vec3 offset)
{

  for(int i=0; i<4; i++)
  {
    Cube *ghost = new Cube((*myCube));
    ghost->transform(vec3(0.0, 0.25, offset.z), vec3(0.0, 0.0, 0.0), vec3(0.15, 0.3, 0.15));
    ghost->color(1.0, 1.0, 1.0);
    ghosts.push_back(ghost);
  }
}

vec3 get_mov()
{
  int x = (rand() % 4)-2;
  int z = (rand() % 4)-2;
  float dist = 0.01;
  vec3 movement;

  if(x<0) movement.x = -dist;
  else if(x=0) movement.x = 0.0;
  else if(x>0) movement.x = dist;

  movement.y = 0.0;

  if(z<0) movement.z = -dist;
  else if(z=0) movement.z = 0.0;
  else if(z>0) movement.z = dist;

  return movement;
}

void move_ghosts()
{
  for(list<Cube>::iterator it = ghosts.begin(); it != ghosts.end(); ++it)
  {
    vec3 movement = get_mov();
    vec3 location = it->get_location();
    bool coll = false;


    for(int i = 0; i<5; i++)
    {
      coll = rooms[i].collision(location, movement);
      if(coll)
        break;
    }

    if(!coll)
    {
      it->trans(movement);
    }
    else
    {
      it->trans(movement*-1);
    }
  }
}

void create_rooms()
{
  float zoffset = 0.0;

  //create first room explicitly
  myRoom = new Room(vec3(0.0, 0.0, 0.0), myCube);
  rooms.push_back(*myRoom);

  //create the rest of the rooms
  for(int i=1; i<numrooms; i++)
  {
    zoffset -= 2;
    myRoom = new Room(vec3(0.0, 0.0, zoffset), myCube);
    rooms.push_back(*myRoom);

    rooms[i-1].nextRoom = &rooms[i];
  }

  current = 0;
}

void open_doors()
{
  vec4 sight = myCamera->Get_At();
  vec4 player = myCamera->Get_Eye();


  if(current != (numrooms-1))
    rooms[current+1].Toggle_Door(vec3(sight.x, sight.y, player.z));
}

void move(vec3 location, vec3 movement)
{
  //list<Room>::iterator next = current_room;
  int next = current;

  if(next < 5)  
    ++next;

  cout << "current = " << current << "next = " << next << endl;

  if(!rooms[current].collision(location, movement) && !rooms[next].collision(location, movement))
  {
    myPlayer->trans(movement);
    myCamera->Move_Eye(movement);
    myCamera->Move_At(movement);
  }

  bool entered_next = rooms[next].Enter_Door(location, movement);
  bool entered_prev = rooms[current].Enter_Door(location, movement);

  if(entered_prev)
  {
    --current;
  }
  else if(entered_next)
  {
    ++current;
  }
}