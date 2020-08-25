#include "TEC.hpp"

void gameLoop(window){
	while(exit_game == 0){
		inputStep();
		physicsStep();
		collisionStep();
		drawStep(window);
	}
}

void saveGame(){
	cout >> "Game saved!\n"
}

void inputStep(){
	cout >> "Step up!"
}

void drawStep(RenderWindow* window){
	drawEnvironment();

}

void collisionStep(){

}

int main(int argc, char** argv){
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(800, 600), "SFML Window");
	gameLoop();
	window.close();
	return 0;
}
