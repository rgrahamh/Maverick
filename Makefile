INCLUDE_SFML= -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -I/usr/include/SFML
CC=g++
CFLAGS= -Wall -g -std=c++11

ABILITY= ./src/Ability/Ability.cpp
ANIMATION= ./src/Animation/Animation.cpp ./src/Animation/SpriteHash/SpriteHash.cpp
CAMERA= ./src/Camera/Camera.cpp
ENGINE= ./src/Engine/Engine.cpp
OBJECT= ./src/Object/Object.cpp ./src/Object/Character/Character.cpp ./src/Object/Character/Player/Player.cpp ./src/Object/Character/Race/Race.cpp ./src/Object/Character/Style/Style.cpp
ZONE= ./src/Zone/Zone.cpp
CYBERENA= ./src/Cyberena.cpp

ALL_OBJECTS= $(ABILITY) $(ANIMATION) $(CAMERA) $(ENGINE) $(OBJECT) $(ZONE) $(CYBERENA)

cyberena:
	$(CC) $(ALL_OBJECTS) -o Cyberena $(CFLAGS) $(INCLUDE_SFML)

basics:
	${CC} sfml_basics.cpp -o sfmlb ${CFLAGS} ${INCLUDE_SFML}
