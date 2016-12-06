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
//				 Implementation file             
//                                                                    
//  Date:        12/6/2016
//                                                                    
//*******************************************************************
#include "sword.h"

//******************************************************************
//                                                                  
//  Function:   Sword
//                                                                  
//  Purpose:    Default sword constructor                   
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: sword is created with default attack strength of 10
//                                                                  
//******************************************************************
Sword::Sword() : Cube()
{
	attack = 10;
}

//******************************************************************
//                                                                  
//  Function:   Sword
//                                                                  
//  Purpose:    Sword constructor                    
//                                                                  
//  Parameters: nCube - cube to create sword from
//				nAttack - attack to give to the sword
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: Cube is rotated by amounts given in vector
//                                                                  
//******************************************************************
Sword::Sword(Cube *nCube, int nAttack) : Cube(nCube)
{
	attack = nAttack;
	speed = 5;
	scl(vec3(0.01, 0.5, 0.05));
	rot(vec3(-45.0, 0.0, 0.0));
	color(0.5, 0.5, 0.5);
	down = false;
}

//******************************************************************
//                                                                  
//  Function:   swing
//                                                                  
//  Purpose:    set the flag to either swing the sword or stop swinging the sword                   
//                                                                  
//  Parameters:
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: swng flag is negated
//                                                                  
//******************************************************************
void Sword::swing()
{
	swng = !swng;
}

//******************************************************************
//                                                                  
//  Function:   move_sword
//                                                                  
//  Purpose:    perform animation of swinging sword                   
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: sword is swung and returned to position if swng is true
//                                                                  
//******************************************************************
void Sword::move_sword()
{
  	static GLint lasttime = glutGet(GLUT_ELAPSED_TIME);
  	GLint time = glutGet(GLUT_ELAPSED_TIME);

	if(get_rotation().x <= -90 && down)
	{
		down = false;
		angle = speed;
	}
	else if(get_rotation().x >= -45 && !down)
	{
		down = true;
		swng = false;
		angle = -speed;
	}

if(swng)
{
	//
	vec3 translation = get_translation();
	vec3 rotation = get_rotation();
	vec3 scale = vec3(get_scale().x/2, get_scale().y/2, 0.0);
	vec3 change = translation - scale;

	//move the sword accordingly
	trans(-change);
	set_rot(vec3(0.0,0.0,0.0));
	rot(vec3(angle, -angle, 0.0));
	rot(rotation);
	trans(change);
}
	draw();
}


//******************************************************************
//                                                                  
//  Function:   sdraw
//                                                                  
//  Purpose:    draw the sword                
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: sword is drawn to screen, with swing animation if necessary
//                                                                  
//******************************************************************
void Sword::sdraw()
{
	move_sword();
}

//******************************************************************
//                                                                  
//  Function:   get_attack
//                                                                  
//  Purpose:    retrieve the attack strength of this sword                  
//                                                                  
//  Parameters: 
//                                                                  
//
// Pre Conditions: 
//
// Post Conditions: attack strength is returned
//                                                                  
//******************************************************************
int Sword::get_attack()
{
	return attack;
}