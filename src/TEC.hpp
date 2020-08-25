void gameLoop();

void inputStep();
void drawStep(sf::RenderWindow* window);
void collisionStep();

void saveGame();

enum GAME_STATE{
	TITLE;
	OVERWORLD;
	BATTLE;
	DISCUSSION;
	PAUSE;
}

Zone** zones;

float X_SCALE = sf::VideoMode::getDesktopMode().width;
float Y_SCALE = sf::VideoMode::getDesktopMode().height;
