#ifndef ENGINE_H
#define ENGINE_H

#include "../Zone/Zone.hpp"
#include "../Camera/Camera.hpp"

class Engine{
	public:
		Engine(Zone** zones);
		~Engine();


	private:
		Zone** zones;
		Zone** active_zones;
		Camera* camera;
};

#endif