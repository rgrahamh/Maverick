#ifndef RACE_H
#define RACE_H

#include <string>

class Race{
	public:
		Race(std::string race);
		~Race();
	protected:
		std::string race;
};

#endif