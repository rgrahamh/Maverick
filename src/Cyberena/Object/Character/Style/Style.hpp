#ifndef STYLE_H
#define STYLE_H

#include <string>

enum STYLE{
	SUPPORT,
	ATTACKER,
	DEFENDER
};

class Style{
	public:
		Style(STYLE style);
	protected:
		STYLE style;
};

#endif