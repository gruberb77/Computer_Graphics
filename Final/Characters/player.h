#ifndef PLAYER_H
#define PLAYER_H

#define NEXT_LEVEL 500
#define KILL_EXP 50


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

		void enemy_killed(int elev, int addexp=KILL_EXP);

		int level_up();

		int get_health();

		int get_next_lev();

		int get_lev();

		int get_attack();

		vec3 get_sword_loc();

	private:
		Cube *myCube;
		Sword *mySword;
		vec3 direction;
		bool alive;
		int health;
		int defense;
		int attack;
		int level;
		int exp;
};

#endif