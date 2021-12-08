INCLUDE_SDL= -lSDL2 -lSDL2_image -lSDL2_ttf -lGL
CC=g++
CFLAGS= -Wall -std=c++17 -pthread -Wno-write-strings

#Engine components
ANIMATION= ./src/Maverick/Animation/Animation.cpp
HASH= ./src/Maverick/HashTable/HashTable.cpp ./src/Maverick/HashTable/TextureHash/TextureHash.cpp
HITBOXES= ./src/Maverick/Animation/Hitbox/Hitbox.cpp ./src/Maverick/Animation/Hitbox/HitEllipse/HitEllipse.cpp ./src/Maverick/Animation/Hitbox/HitEllipse/HitCone/HitCone.cpp ./src/Maverick/Animation/Hitbox/HitRect/HitRect.cpp ./src/Maverick/Animation/Hitbox/HitboxCollision.cpp
CAMERA= ./src/Maverick/Camera/Camera.cpp
CONTROL= ./src/Maverick/Control/Control.cpp
UI= ./src/Maverick/Entity/UI/UIElement.cpp ./src/Maverick/Entity/UI/UIText/UIText.cpp ./src/Maverick/Entity/UI/UITextBox/UITextBox.cpp ./src/Maverick/Entity/UI/UIScreenBlit/UIScreenBlit.cpp ./src/Maverick/Entity/UI/UIObjectFrame/UIObjectFrame.cpp
OBJECT= ./src/Maverick/Entity/Object/Object.cpp
ENTITY= ./src/Maverick/Entity/Entity.cpp $(OBJECT) $(UI)
ZONE= ./src/Maverick/Zone/Zone.cpp
ZONE_FACTORY= ./src/Maverick/Zone/ZoneFactory/ZoneFactory.cpp
FACTORY= ./src/Cyberena/Factory/Factory.cpp
ENGINE= ./src/Maverick/Engine/Engine.cpp
MOTOR= $(ANIMATION) $(HASH) $(HITBOXES) $(CAMERA) $(CONTROL) $(ENTITY) $(ZONE) $(ZONE_FACTORY) $(FACTORY) $(ENGINE)

#Game components
CYBERENA_OBJECT= ./src/Cyberena/Entity/Object/Character/Character.cpp ./src/Cyberena/Entity/Object/Character/Race/Race.cpp ./src/Cyberena/Entity/Object/Character/Style/Style.cpp
CYBERENA_UI=
CYBERENA_ENTITY= $(CYBERENA_OBJECT) $(CYBERENA_UI)
CYBERENA_ZONES= ./src/Cyberena/Zones/TestZone/TestZone.cpp ./src/Cyberena/Zones/global/global.cpp
CYBERENA = $(CYBERENA_ENTITY) $(CYBERENA_FACTORY) $(CYBERENA_ZONES)

#Editor components
LED_ZONES= ./src/led/Zones/led/led.cpp
LED_FACTORY= 
LED= $(LED_FACTORY) $(LED_ZONES)

#RPG Components
#ABILITY= ./src/Maverick/Cyberena/Ability/Ability.cpp
#ITEM= ./src/Maverick/Cyberena/Entity/Object/Item/Item.cpp
#RPG = $(ABILITY) $(ITEM)

#Master component
MAVERICK= ./src/Maverick/Maverick.cpp $(MOTOR)

led:
	$(CC) $(MAVERICK) $(CYBERENA) $(LED) -o led $(CFLAGS) -g $(INCLUDE_SDL)

cyberena_test:
	$(CC) -O0 $(MAVERICK) $(CYBERENA) -o cyberena $(CFLAGS) -g $(INCLUDE_SDL)

cyberena_release:
	$(CC) -O3 $(MAVERICK) $(CYBERENA) -o cyberena $(CFLAGS) $(INCLUDE_SDL)
