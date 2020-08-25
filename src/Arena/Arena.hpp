#ifndef ARENA_H
#define ARENA_H

#include "../Character/Character.hpp"

typedef struct TeamLst{
	Character* fighter;
	Character* next;
} Team;

class Arena{
	public:
		Arena();
		~Arena();
	private:
		Team** teams;
}

#endif
