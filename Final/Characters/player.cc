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
//  Description: This is a player class to create a character in the game
//				 Implementation file             
//                                                                    
//  Date:        12/6/2016
//                                                                    
//*******************************************************************

#include "player.h"

//offset of sword to player
const vec3 swordoff = vec3(0.075, 0.1, -0.25);

//******************************************************************
//                                                                  
//  Function:   Player
//                                                                  
//  Purpose:    Default constructor                      
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: creates a new character
//                                                                  
//******************************************************************
Player::Player() : Cube()
{

  health = 100;
  level = 1;
  defense = 20;
  alive = true;
}

//******************************************************************
//                                                                  
//  Function:   Player
//                                                                  
//  Purpose:    constructor                      
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: creates a new character
//                                                                  
//******************************************************************
Player::Player(Cube *nCube, int nLevel) : Cube(nCube)
{
	myCube = nCube;
	mySword = new Sword(nCube, 10);
	level = nLevel;
	health = 100;
	defense = level*5;
	alive = true;
	attack = 100;
	exp = 0;
}

//******************************************************************
//                                                                  
//  Function:   take_damage
//                                                                  
//  Purpose:    take damage if player was hit                    
//                                                                  
//  Parameters: damage - how much damage was received
//                                                                  
//
// Pre Conditions: player is alive
//
// Post Conditions: reduces health and returns if player is still alive
//                                                                  
//******************************************************************
bool Player::take_damage(int damage)
{
	health -= damage/defense;
	std::cout << "health = " << health << std::endl;
	if (health <= 0)
	{
		alive = false;
	}
	return alive;
}

//******************************************************************
//                                                                  
//  Function:   pdraw
//                                                                  
//  Purpose:    draw the player's sword on the screen                     
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: sword will be drawn to screen
//                                                                  
//******************************************************************
void Player::pdraw()
{
	mySword->sdraw();
}

//******************************************************************
//                                                                  
//  Function:   move_player
//                                                                  
//  Purpose:    move the player in the world                    
//                                                                  
//  Parameters: movement - vector to move player along
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: player will be in new postion
//                                                                  
//******************************************************************
void Player::move_player(vec3 movement)
{
	trans(movement);
	vec3 curr = get_location();
	mySword->set_pos(curr + swordoff);
}

//******************************************************************
//                                                                  
//  Function:   rotate_player
//                                                                  
//  Purpose:    rotate the player by the given angle                   
//                                                                  
//  Parameters: angle - angle to rotate player by in degrees
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: player + sword will be rotated by the input angle
//                                                                  
//******************************************************************
void Player::rotate_player(float angle)
{
	//where the player is on the screen
	vec3 translation = get_location();
	vec3 protation = get_rotation();
	vec3 srotation = mySword->get_rotation();
	vec3 change = translation;

	if(protation.y >= 360.0 || protation.y <= 360.0)
	{
		set_rot(vec3(protation.x, 0.0, protation.z));
		mySword->set_rot(vec3(srotation.x, 0.0, srotation.z));
	}

	//move to origin, rotate, then move back
	trans(-change);
	rot(vec3(0.0, -angle, 0.0));
	trans(change);

	//move sword to player's origin, rotate, then move back
	mySword->trans(-change);
	mySword->rot(vec3(0.0, -angle, 0.0));
	mySword->trans(change);
}

//******************************************************************
//                                                                  
//  Function:   swing_sword
//                                                                  
//  Purpose:    swing the sword the player is holding                      
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: sword is swung
//                                                                  
//******************************************************************
void Player::swing_sword()
{
	mySword->swing();
}

//******************************************************************
//                                                                  
//  Function:   enemy_killed
//                                                                  
//  Purpose:    update player's experience on a kill                     
//                                                                  
//  Parameters: elev - level of enemy killed
//				addexp -experience scalar to add to player's exp, default is 50
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: experience variable is updated
//                                                                  
//******************************************************************
void Player::enemy_killed(int elev, int addexp)
{
	exp += (elev*addexp);
}

//******************************************************************
//                                                                  
//  Function:   level_up
//                                                                  
//  Purpose:    level up the player if they have enough experience                     
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: if player's experience is greater than experience needed
//					then level up.  returns current level
//                                                                  
//******************************************************************
int Player::level_up()
{
	if(exp >= NEXT_LEVEL)
	{
		exp = 0;
		level++;
		defense += (level*10);
		attack += (level*10);
	}

	return level;
}

//******************************************************************
//                                                                  
//  Function:   get_health
//                                                                  
//  Purpose:    retrieve the player's health                     
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: returns player's health
//                                                                  
//******************************************************************
int Player::get_health()
{
	return health;
}

//******************************************************************
//                                                                  
//  Function:   get_next_lev
//                                                                  
//  Purpose:    return the amount of experience needed for next level                      
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: returns the experience needed for a player to level up
//                                                                  
//******************************************************************
int Player::get_next_lev()
{
	int to_next = NEXT_LEVEL - exp;
	return to_next;
}

//******************************************************************
//                                                                  
//  Function:   get_lev
//                                                                  
//  Purpose:    retrieve the player's current level                     
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: returns the player's current level
//                                                                  
//******************************************************************
int Player::get_lev()
{
	return level;
}


//******************************************************************
//                                                                  
//  Function:   Player
//                                                                  
//  Purpose:    retrieve the player+sword's attack strength                    
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: returns the sum of player attack and sword attack
//                                                                  
//******************************************************************
int Player::get_attack()
{
	int tot_attack = mySword->get_attack() + attack;
	return tot_attack;
}

//******************************************************************
//                                                                  
//  Function:   Player
//                                                                  
//  Purpose:    retrieve location of sword player is holding                     
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: sword location is returned
//                                                                  
//******************************************************************
vec3 Player::get_sword_loc()
{
	return mySword->get_location();
}