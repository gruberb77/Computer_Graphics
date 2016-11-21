#ifndef PLAYER_H
#define PLAYER_H

#include "../Cube/cube.h"
#include "sword.h"

class Player: public Cube {
	public:
		Player();

		Player(Cube *nCube, int nLevel);

		bool take_damage(int damage);

		void pdraw();

		void move_player(vec3 movement);

		void rotate_player(float angle);

		void swing_sword();

		int get_health();

	private:
		Cube *myCube;
		Sword *mySword;
		vec3 direction;
		bool alive;
		int health;
		int defense;
		int level;
};

#endif