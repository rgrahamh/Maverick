INCLUDE_SFML= -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -I/usr/include/SFML
CC=g++
CFLAGS= -Wall -g -std=c++11

ABILITY= ./src/Ability/Ability.cpp
ANIMATION= ./src/Animation/Animation.cpp ./src/Animation/TextureHash/TextureHash.cpp
HITBOXES= ./src/Animation/Hitbox/Hitbox.cpp ./src/Animation/Hitbox/HitEllipse/HitEllipse.cpp ./src/Animation/Hitbox/HitEllipse/HitCone/HitCone.cpp ./src/Animation/Hitbox/HitRect/HitRect.cpp ./src/Animation/Hitbox/HitboxCollision.cpp
CAMERA= ./src/Camera/Camera.cpp
ENGINE= ./src/Engine/Engine.cpp
OBJECT= ./src/Object/Object.cpp ./src/Object/Character/Character.cpp ./src/Object/Character/Player/Player.cpp ./src/Object/Character/Race/Race.cpp ./src/Object/Character/Style/Style.cpp
ZONE= ./src/Zone/Zone.cpp
FACTORY= ./src/Factory/Factory.cpp
CYBERENA= ./src/Cyberena.cpp

ALL_OBJECTS= $(ABILITY) $(ANIMATION) $(HITBOXES) $(CAMERA) $(OBJECT) $(ZONE) $(FACTORY) $(ENGINE) $(CYBERENA)

cyberena:
	$(CC) $(ALL_OBJECTS) -o Cyberena $(CFLAGS) $(INCLUDE_SFML)

basics:
	${CC} sfml_basics.cpp -o sfmlb ${CFLAGS} ${INCLUDE_SFML}
