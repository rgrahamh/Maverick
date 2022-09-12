#ifndef ARENA_H
#define ARENA_H

#include "../Object/Character/Character.hpp"

typedef struct TeamList{
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
