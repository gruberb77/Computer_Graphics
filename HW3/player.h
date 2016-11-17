#ifndef PLAYER_H
#define PLAYER_H
// Class definition of player object
//
// Written by Brent Gruber, based on cube class
//
// Cube must be defined in VAO at index

#include "cube.h"
#include "room.h"

class Player: public Cube {
	public:
		//default constructor
		Player();

		//draw the player
		void draw();

	private:
		//pointer to cube that represents player
		Cube *myPlayer;

		//pointer to room current player is in
		Room *current_room;
};

#endif

		


