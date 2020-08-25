#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

int main(int argc, char** argv){
	//sf::RenderWindow window(sf::VideoMode(800, 600), "SFML Window");
	sf::RenderWindow window(sf::VideoMode(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height), "SFML Window");

	sf::Texture texture;
	if(!texture.loadFromFile("./Tails.png")){
		printf("Cannot find image!\n");
		return 1;
	}
	sf::Sprite sprite(texture);
	sprite.setScale(0.1, 0.1);
	sf::Event event;
	while(window.isOpen()){
		while(window.pollEvent(event)){
			// Close window: exit
			if(event.type == sf::Event::Closed){
				window.close();
			}
			window.clear();
			window.draw(sprite);
			window.display();
		}
	}

	return 0;
}
