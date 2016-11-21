
#include "sword.h"

Sword::Sword() : Cube()
{
	attack = 10;
}


Sword::Sword(Cube *nCube, int nAttack) : Cube(nCube)
{
	attack = nAttack;
	speed = 0.5;
	scl(vec3(0.01, 0.5, 0.05));
	rot(vec3(-45.0, 0.0, 0.0));
	color(1.0, 0.0, 0.0);
	down = false;
}

void Sword::swing()
{
		swng = !swng;
		//if(down)
		//{
			//speed = -0.003;
		//}
		//else
		//{
			//speed = 0.003;
		//}
}


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
	vec3 scale = vec3(get_scale().x/2, get_scale().y/2, 0.0);
	vec3 change = translation - scale;

	//move the door accordingly
	trans(-change);
	rot(vec3(angle, 0.0, 0.0));
	trans(change);
}
	draw();
}


void Sword::sdraw()
{
	move_sword();
}