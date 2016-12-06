//******************************************************************* 
//                                                                    
//  File:        Player                                         
//                                                                     
//  Author:      Brent Gruber
//  Email:       bg240711@ohio.edu
//  Class:       CS 4250 - Final Project
//                                                                    
//
//                                                                    
//  Description: This is the class used to contain a player character, 
//				            
//                                                                    
//  Date:        12/6/2016
//                                                                    
//*******************************************************************
#ifndef PLAYER_H
#define PLAYER_H

//how much experience it takes to reach next level
#define NEXT_LEVEL 500
//experience scalar gained from a kill
#define KILL_EXP 50


#include "../Cube/cube.h"
#include "sword.h"

class Player: public Cube {
	public:
		//Default Constructor
		Player();

		//Constructor to be used, defines a level when created
		Player(Cube *nCube, int nLevel);

		//take damage if player was hit
		bool take_damage(int damage);

		//draw the player, also draws sword
		void pdraw();

		//move the player in the world
		void move_player(vec3 movement);

		//rotate the player by a certain angle, in degrees
		void rotate_player(float angle);

		//swing the sword the player is holding
		void swing_sword();

		//add experience if an enemy was killed
		void enemy_killed(int elev, int addexp=KILL_EXP);

		//check if the player has leveled up and if they have reset exp
		int level_up();

		//return the current health
		int get_health();

		//return how much experience player needs to level up
		int get_next_lev();

		//return the player's current level
		int get_lev();

		//return the attack strenght of the player plus sword
		int get_attack();

		//return the location of this player's sword
		vec3 get_sword_loc();

	private:
		Cube *myCube;
		//sword the player is holding
		Sword *mySword;
		//direction the player is facing
		vec3 direction;
		//if player is alive
		bool alive;
		//player's health
		int health;
		//player's defense
		int defense;
		//player's attack strenght
		int attack;
		//player's current level
		int level;
		//how much experience player currenlty has
		int exp;
};

#endif