INCLUDE_SDL= -lSDL2 -lSDL2_image -lSDL2_ttf -lGL
CC=g++
CFLAGS= -Wall -Wno-write-strings -std=c++17 -pthread

#Engine components
ANIMATION= ./src/Animation/Animation.cpp
HASH= ./src/HashTable/HashTable.cpp ./src/HashTable/TextureHash/TextureHash.cpp
HITBOXES= ./src/Animation/Hitbox/Hitbox.cpp ./src/Animation/Hitbox/HitEllipse/HitEllipse.cpp ./src/Animation/Hitbox/HitEllipse/HitCone/HitCone.cpp ./src/Animation/Hitbox/HitRect/HitRect.cpp ./src/Animation/Hitbox/HitboxCollision.cpp
CAMERA= ./src/Camera/Camera.cpp
CONTROL= ./src/Control/Control.cpp
UI= ./src/Entity/UI/UIElement.cpp ./src/Entity/UI/UIText/UIText.cpp ./src/Entity/UI/UITextBox/UITextBox.cpp
OBJECT= ./src/Entity/Object/Object.cpp ./src/Entity/Object/Character/Character.cpp ./src/Entity/Object/Character/Race/Race.cpp ./src/Entity/Object/Character/Style/Style.cpp
ENTITY= ./src/Entity/Entity.cpp $(OBJECT) $(UI)
ZONE= ./src/Zone/Zone.cpp
ZONE_FACTORY= ./src/Zone/ZoneFactory/ZoneFactory.cpp ./src/Zone/ZoneFactory/TestZone/TestZone.cpp ./src/Zone/ZoneFactory/led/led.cpp
FACTORY= ./src/Factory/Factory.cpp
ENGINE= ./src/Engine/Engine.cpp
MOTOR= $(ANIMATION) $(HASH) $(HITBOXES) $(CAMERA) $(CONTROL) $(ENTITY) $(ZONE) $(ZONE_FACTORY) $(FACTORY) $(ENGINE)

#RPG Components
ABILITY= ./src/Ability/Ability.cpp
ITEM= ./src/Item/Item.cpp
RPG = $(ABILITY) $(ITEM)

#Master component
CYBERENA= ./src/Cyberena.cpp

ALL_OBJECTS= $(RPG) $(MOTOR) $(CYBERENA)

cyberena_test:
	$(CC) -O0 $(ALL_OBJECTS) -o Cyberena $(CFLAGS) -g $(INCLUDE_SDL)

cyberena_release:
	$(CC) -O3 $(ALL_OBJECTS) -o Cyberena $(CFLAGS) $(INCLUDE_SDL)

basics:
	${CC} sfml_basics.cpp -o sfmlb ${CFLAGS} ${INCLUDE_SDL}
