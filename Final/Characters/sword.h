//******************************************************************* 
//                                                                    
//  File:        Sword                                          
//                                                                     
//  Author:      Brent Gruber
//  Email:       bg240711@ohio.edu
//  Class:       CS 4250 - Final Project
//                                                                    
//
//                                                                    
//  Description: This class will provide the user player with a sword they can swing to damage enemies              
//                                                                    
//  Date:        12/6/2016
//                                                                    
//*******************************************************************

#ifndef SWORD_H
#define SWORD_H

#include "../Cube/cube.h"

//sword class built on cube class
class Sword: public Cube {
	public:
		//constructor
		Sword();

		//better constructor
		Sword(Cube *nCube, int nAttack);

		//set or clear flag for swing animation
		void swing();

		//draw the sword, animate if necessary
		void sdraw();

		//return the attack strength of the sword
		int get_attack();

	private:
		//perform animation of sword
		void move_sword();

		Cube* myCube;
		//attack strength
		int attack;
		//angle for animation
		float angle;
		float speed;
		//flag to determine if sword was swung
		bool swng;
		//flag to determine if sword has reched bottom of swing
		bool down;
};

#endif