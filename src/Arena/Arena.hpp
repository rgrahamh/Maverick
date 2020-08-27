#ifndef ARENA_H
#define ARENA_H

#include "../Object/Character/Character.hpp"

typedef struct TeamLst{
	Character* character;
	Character* next;
} Team;

class Arena{
	public:
		Arena();
		~Arena();
	private:
		Team** teams;
};

#endif
