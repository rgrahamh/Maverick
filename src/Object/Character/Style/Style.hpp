#ifndef STYLE_H
#define STYLE_H

#include <string>

class Style{
	public:
		Style(std::string style);
		~Style();
	protected:
		std::string style_name;
};

#endif