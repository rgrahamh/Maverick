#ifndef WEAPON_H
#define WEAPON_H

#include "Maverick/Object/Object.hpp"

enum WEAPON_TYPE{
	axe,
	blaster,
	claws,
	knuckles,
	lance,
	sword
};

class Weapon : public Object{
	public:
		Weapon();
		~Weapon();
	protected:
};

#endif