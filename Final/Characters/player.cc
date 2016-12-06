

#include "player.h"

const vec3 swordoff = vec3(0.075, 0.1, -0.25);


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
	mySword = new Sword(nCube, 10);
	level = nLevel;
	health = 100;
	defense = level*5;
	alive = true;
	attack = 100;
	exp = 0;
}


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

void Player::pdraw()
{
	mySword->sdraw();
}

void Player::move_player(vec3 movement)
{
	trans(movement);
	vec3 curr = get_location();
	mySword->set_pos(curr + swordoff);
}

void Player::rotate_player(float angle)
{

	vec3 translation = get_location();
	vec3 protation = get_rotation();
	vec3 srotation = mySword->get_rotation();
	vec3 change = translation;

	if(protation.y >= 360.0 || protation.y <= 360.0)
	{
		set_rot(vec3(protation.x, 0.0, protation.z));
		mySword->set_rot(vec3(srotation.x, 0.0, srotation.z));
	}

	trans(-change);
	rot(vec3(0.0, -angle, 0.0));
	trans(change);

	mySword->trans(-change);
	mySword->rot(vec3(0.0, -angle, 0.0));
	mySword->trans(change);
}

void Player::swing_sword()
{
	mySword->swing();
}

void Player::enemy_killed(int elev, int addexp)
{
	exp += (elev*addexp);
}

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

int Player::get_health()
{
	return health;
}

int Player::get_next_lev()
{
	int to_next = NEXT_LEVEL - exp;
	return to_next;
}

int Player::get_lev()
{
	return level;
}

int Player::get_attack()
{
	int tot_attack = mySword->get_attack() + attack;
	return tot_attack;
}

vec3 Player::get_sword_loc()
{
	return mySword->get_location();
}