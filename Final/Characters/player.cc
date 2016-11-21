

#include "player.h"

Player::Player() : Cube()
{

  health = 100;
  level = 1;
  defense = 20;
  alive = true;
}


Player::Player(Cube *nCube, int nLevel) : Cube(nCube)
{
	myCube = nCube;
	mySword = new Sword(nCube, 800);
	level = nLevel;
	health = 100;
	defense = level*20;
	alive = true;
}


bool Player::take_damage(int damage)
{
	health -= damage/defense;
	if (health <= 0)
	{
		alive = false;
	}
	return alive;
}

void Player::pdraw()
{
	draw();
	mySword->sdraw();
}

void Player::move_player(vec3 movement)
{
	trans(movement);
	vec3 curr = get_location();
	mySword->set_pos(vec3(curr.x+0.075, curr.y+0.2, curr.z-0.25));
}

void Player::rotate_player(float angle)
{
	std::cout << "angle = " << angle << std::endl;

	vec3 translation = get_location();
	vec3 rotation = mySword->get_rotation();
	vec3 change = translation;

	std::cout << "location = " << translation << " scale = " << scale << std::endl;

	//move the door accordingly
	trans(-change);
	set_rot(vec3(0.0, -angle, 0.0));
	trans(change);

	mySword->trans(-change);
	mySword->set_rot(vec3(-45.0, -angle, 0.0));
	mySword->trans(change);
}

void Player::swing_sword()
{
	mySword->swing();
}


int Player::get_health()
{
	return health;
}