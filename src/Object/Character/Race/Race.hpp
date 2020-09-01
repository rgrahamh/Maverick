#ifndef RACE_H
#define RACE_H

#include <string>

enum RACE{
	HUMAN,
	ROBOT,
	LATIMUS
};

class Race{
	public:
		Race(RACE race);
		~Race();
	protected:
		RACE race;
};

#endif