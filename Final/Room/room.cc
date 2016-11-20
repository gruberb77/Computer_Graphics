//******************************************************************* 
//                                                                    
//  File:        Room implementation                                           
//                                                                     
//  Author:      Brent Gruber
//  Email:       bg240711@ohio.edu
//                                                                    
//
//                                                                    
//  Description: This is the implementation a class that contains a room in a house                  
//                                                                    
//  Date:        11/1/2016
//                                                                    
//*******************************************************************


#include "room.h"

//******************************************************************
//                                                                  
//  Function:   Room
//                                                                  
//  Purpose:    default constructor                          
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: new Room instance is created
//                                                                  
//******************************************************************
Room::Room()
{
	//set all values
	offset = vec3(0.0, 0.0, 0.0);
	myCube = new Cube();
	nextRoom = NULL;
	prevRoom = NULL;
	doorAngle = 0.0;
	doorAngleIncr = 0.0;
	open = false;
	//build the room
	build_Room();
}

//******************************************************************
//                                                                  
//  Function:   Room
//                                                                  
//  Purpose:    constructor                           
//                                                                  
//  Parameters: inputs
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: new Room instance is created with given inputs
//                                                                  
//******************************************************************
Room::Room(vec3 noffset, Cube *ncube, Room *Prev, Room *Next)
{
	//set all the values
	offset = noffset;
	myCube = ncube;
	nextRoom = Next;
	prevRoom = Prev;
	doorAngle = 0.0;
	doorAngleIncr = 0.0;
	open = false;
	//build the room
	build_Room();
}

//******************************************************************
//                                                                  
//  Function:   build_Room
//                                                                  
//  Purpose:    used to create a room                           
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: a room exists at the class's offset vector
//                                                                  
//******************************************************************
void Room::build_Room()
{
  //TODO: remove hard coding

  //set values used in building of room
  GLfloat thickness = 0.06;
  GLfloat normal = 2.0;
  GLfloat translation = normal/2.0;
  GLfloat sidew = 0.35*normal;
  GLfloat doorw = 0.375*normal;
  GLfloat topw = 0.875*normal;

  //used for scaling of walls
  vec3 thinx = vec3(thickness, normal, normal);
  vec3 thiny = vec3(normal, thickness, normal);
  vec3 thinz = vec3(normal, normal, thickness);


  //floor
  myCube->transform((vec3(0.0, 0.0, 0.0)+offset), vec3(0.0, 0.0, 0.0), thiny);
  floor = new Cube((*myCube));
  floor->color(0.0, 0.0, 1.0);
  myCube->Identity();

  //right wall
  myCube->transform((vec3(translation, translation, 0.0)+offset), vec3(0.0, 0.0, 0.0), thinx);
  right = new Cube((*myCube));
  right->color(0.6, 0.0, 0.3);
  myCube->Identity();

  //left wall
  myCube->transform((vec3(-translation, translation, 0.0)+offset), vec3(0.0, 0.0, 0.0), thinx);
  left = new Cube((*myCube));
  left->color(0.6, 0.0, 0.3);
  myCube->Identity();

  //back wall
  myCube->transform((vec3(0.0, translation, -translation)+offset), vec3(0.0, 0.0, 0.0), thinz);
  back = new Cube((*myCube));
  back->color(0.6, 0.0, 0.3);
  myCube->Identity();

  //ceiling
  myCube->transform((vec3(0.0, normal, 0.0)+offset), vec3(0.0, 0.0, 0.0), thiny);
  ceiling = new Cube((*myCube));
  ceiling->color(0.6, 0.6, 0.6);
  myCube->Identity();

  //left of front wall
  myCube->transform((vec3(-sidew, translation, translation)+offset), vec3(0.0, 0.0, 0.0), vec3(0.6, normal, thickness));
  frontleft = new Cube((*myCube));
  frontleft->color(0.0, 0.3, 0.3);
  myCube->Identity();

  //right of front wall
  myCube->transform((vec3(sidew, translation, translation)+offset), vec3(0.0, 0.0, 0.0), vec3(0.6, normal, thickness));
  frontright = new Cube((*myCube));
  frontright->color(0.0, 0.3, 0.3);
  myCube->Identity();

  //top of front wall
  myCube->transform((vec3(0.0, topw, translation)+offset), vec3(0.0, 0.0, 0.0), vec3(0.8, 0.5, thickness));
  fronttop = new Cube((*myCube));
  fronttop->color(0.0, 0.3, 0.3);
  myCube->Identity();

  //Door
  myCube->transform((vec3(0.0, doorw, translation)+offset), vec3(0.0, 0.0, 0.0), vec3(0.8, 1.5, thickness));
  door = new Cube((*myCube));
  door->color(0.5, 0.5, 0.0);
  myCube->Identity();

  draw();
}

//******************************************************************
//                                                                  
//  Function:   draw
//                                                                  
//  Purpose:    draw the room                            
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: room and all components exist
//
// Post Conditions: room is visible on screen
//                                                                  
//******************************************************************
void Room::draw()
{
	//draw each component
	left->draw();
	right->draw();
	ceiling->draw();
	frontleft->draw();
	frontright->draw();
	fronttop->draw(); 
	floor->draw();

	//open or close door if necessary
	Move_Door();

	//if last room draw back wall, if not then don't draw
	if(nextRoom == NULL)
	{
		back->draw();
	}
}

//******************************************************************
//                                                                  
//  Function:   collision
//                                                                  
//  Purpose:    check a point and movement vector for a collision with the walls                           
//                                                                  
//  Parameters: point - location of object to compare against walls
//				movement - movement vector of object
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: returns true if there was a collision
//                                                                  
//******************************************************************
bool Room::collision(vec3 point, vec3 movement)
{
	//check collision on each component
	return (left->Check_Collision(point, movement) ||
		right->Check_Collision(point, movement) ||
		frontleft->Check_Collision(point, movement) ||
		frontright->Check_Collision(point, movement) ||
		(!open && door->Check_Collision(point, movement)) ||
		(nextRoom == NULL && back->Check_Collision(point, movement)));
}

//******************************************************************
//                                                                  
//  Function:   Enter_Door
//                                                                  
//  Purpose:    to determine if an object passed through the doorway                          
//                                                                  
//  Parameters: point - object to check
//				movement - movement vector of object
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: true if doorway was passed through
//                                                                  
//******************************************************************
bool Room::Enter_Door(vec3 point, vec3 movement)
{
	//check if the room was entered throught the door
	bool entered =(open && door->Check_Collision(point, movement));
	return entered;
}

//******************************************************************
//                                                                  
//  Function:   Connect_Rooms
//                                                                  
//  Purpose:    set next and previous pointers for rooms                           
//                                                                  
//  Parameters: Next - next room
//				Prev - previous room
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: pointers are set
//                                                                  
//******************************************************************
void Room::Connect_Rooms(Room *Next = NULL, Room *Prev = NULL)
{
	//set pointers
	nextRoom = Next;
	prevRoom = Prev;
}

//******************************************************************
//                                                                  
//  Function:  Toggle_Door
//                                                                  
//  Purpose:    Toggle if the door is open or not                           
//                                                                  
//  Parameters: position->position of player's sight, only open if within sight aka collision
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: door is set to be opened/closed if within sight
//                                                                  
//******************************************************************
void Room::Toggle_Door(vec3 position)
{
	//if there was a collision with the door, open/close it
    if(door->Check_Collision(position, vec3(0.0, 0.0, -5.0)))
    {
		if(open)
		{
			doorAngleIncr = -0.35;
		}
		else
		{
			doorAngleIncr = 0.35;
		}
	}
}


//******************************************************************
//                                                                  
//  Function:   Move_Door
//                                                                  
//  Purpose:    physically moves the door                          
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: door is set to be opened/closed
//
// Post Conditions: door is moved applicably
//                                                                  
//******************************************************************
void Room::Move_Door()
{
  	static GLint lasttime = glutGet(GLUT_ELAPSED_TIME);
  	GLint time = glutGet(GLUT_ELAPSED_TIME);

	//door is fully open, so stop moving it
	if(doorAngle >= 90.0 &&  !open)
	{
		open = true;
		doorAngleIncr = 0.0;
	}
	else if(doorAngle <= 0.0 && open) //door is closed so stop moving
	{
		open = false;
		doorAngleIncr = 0.0;
	}

	//update the angle
  	float angle = doorAngleIncr;
	doorAngle += angle;

	//
	vec3 translation = door->get_translation();
	vec3 scale = vec3(door->get_scale().x/2, 0.0, 0.0);
	vec3 change = translation - scale;

	//move the door accordingly
	door->trans(-change);
	door->rot(vec3(0.0, -angle, 0.0));
	door->trans(change);
	door->draw();
}

//******************************************************************
//                                                                  
//  Function:   Get_Open
//                                                                  
//  Purpose:    determine if door is open or not                           
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: return true if door is open
//                                                                  
//******************************************************************
bool Room::Get_Open()
{
	return open;
}

//******************************************************************
//                                                                  
//  Function:   Get_Offset
//                                                                  
//  Purpose:    get the offset of the room                        
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: returns the offset(location) of the room
//                                                                  
//******************************************************************
vec3 Room::Get_Offset()
{
	return offset;
}


