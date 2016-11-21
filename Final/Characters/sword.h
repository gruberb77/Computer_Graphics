#ifndef SWORD_H
#define SWORD_H

#include "../Cube/cube.h"

class Sword: public Cube {
	public:
		Sword();

		Sword(Cube *nCube, int nAttack);

		void swing();

		void sdraw();

	private:
		void move_sword();

		Cube* myCube;
		int attack;
		float angle;
		float speed;
		bool swng;
		bool down;
};

#endif