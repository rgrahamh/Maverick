#ifndef ENGINE_H
#define ENGINE_H

#include "../Zone/Zone.hpp"
#include "../Camera/Camera.hpp"

class Engine{
	public:
		Engine(ZoneLst* zones);
		~Engine();

		void draw();

	private:
		ZoneLst* zones;
		ZoneLst* active_zones;
		Camera* camera;
};

#endif