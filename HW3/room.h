#ifndef ROOM_H
#define ROOM_H
// Class definition of room object, based on cube object
//
// Written by Brent Gruber, based on rectangle class from David M. Chelberg
//
// Cube must be defined in VAO at index

#include "cube.h"

class Room: public Cube {
	public:
		//default constructor
		Room();

		//second constructor
		Room(vec3 noffset, Cube *ncube, Room *Prev = NULL, Room *Next = NULL);

		//build a room at the current offset
		void build_Room();

		//draw the room
		void draw();

		//check if player hits wall, if true wall was hit
		bool collision(vec3 point, vec3 movement);

		//check if room was entered
		bool Enter_Door(vec3 point, vec3 movement);
		
		//connect rooms
		void Connect_Rooms(Room *Next, Room *Prev);

		//open and close the door
		void Toggle_Door(vec3 position);

		//see if the door is open
		bool Get_Open();

		//pointer to the next room, default to null
		Room *nextRoom;

		//pointer to previous room, default to null
		Room *prevRoom;

				//location of room in relation to origin
		vec3 offset;

	private:

		//moving the door
		void Move_Door();



		//cube to build room out of
		Cube *myCube;

		//Cube that is the floor of the room
		Cube *floor;

		//left wall
		Cube *left;
		
		//right wall
		Cube *right;

		//back wall
		Cube *back;
		
		//ceiling
		Cube *ceiling;

		//left portion of front wall
		Cube *frontleft;
		
		//right portion of front wall
		Cube *frontright;

		//top portion of front wall
		Cube *fronttop;

		//Cube that is the door of the room
		Cube *door;

		//angle the door is currently at
		GLfloat doorAngle;
		
		//how fast to rotate the door
		GLfloat doorAngleIncr;

		//contains whether the door is open or not
		bool open;
		
};

#endif
