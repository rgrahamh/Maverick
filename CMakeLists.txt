cmake_minimum_required(VERSION 2.2)

project(Cyberena)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++11 -pthread -Wno-write-strings -Wno-int-to-pointer-cast")

#Engine source
set(AnimationSrc
    ./src/Maverick/Animation/Animation.cpp
)

set(AudioSrc
    ./src/Maverick/Audio/Audio.cpp
    ./src/Maverick/Audio/Music/Music.cpp
    ./src/Maverick/Audio/SoundBoard/SoundBoard.cpp
)

set(FortSrc
    ./src/Maverick/Font/Font.cpp
)

set(HashSrc
    ./src/Maverick/HashTable/HashTable.cpp
    ./src/Maverick/HashTable/AttributeHash/AttributeHash.cpp
    ./src/Maverick/HashTable/SpriteHash/SpriteHash.cpp
    ./src/Maverick/HashTable/MusicHash/MusicHash.cpp
    ./src/Maverick/HashTable/SoundHash/SoundHash.cpp
    ./src/Maverick/HashTable/FontHash/FontHash.cpp
)

set(HitboxSrc
    ./src/Maverick/Animation/Hitbox/Hitbox.cpp
    ./src/Maverick/Animation/Hitbox/HitEllipse/HitEllipse.cpp
    ./src/Maverick/Animation/Hitbox/HitEllipse/HitCone/HitCone.cpp
    ./src/Maverick/Animation/Hitbox/HitRect/HitRect.cpp
    ./src/Maverick/Animation/Hitbox/HitboxCollision.cpp
)

set(CameraSrc
    ./src/Maverick/Camera/Camera.cpp
)

set(ControlSrc
    ./src/Maverick/Control/Control.cpp
)

set(UISrc
    ./src/Maverick/Entity/UI/UIElement.cpp
    ./src/Maverick/Entity/UI/UIBorders/UIBorders.cpp
    ./src/Maverick/Entity/UI/UIText/UIText.cpp
    ./src/Maverick/Entity/UI/UITextBox/UITextBox.cpp
    ./src/Maverick/Entity/UI/UIObjectFrame/UIObjectFrame.cpp
)

set(ObjectSrc
    ./src/Maverick/Entity/Object/Object.cpp
    ./src/Maverick/Entity/Object/CameraReference/CameraReference.cpp
    ./src/Maverick/Entity/Object/LoadingZone/LoadingZone.cpp
)

set(EntitySrc
    ./src/Maverick/Entity/Entity.cpp
    ${ObjectSrc}
    ${UISrc}
)

set(ZoneSrc
    ./src/Maverick/Zone/Zone.cpp
)

set(ZoneFactorySrc
    ./src/Maverick/Zone/ZoneFactory/ZoneFactory.cpp
)

set(FactorySrc
    ./src/Cyberena/Factory/Factory.cpp
)

set(FileHandlerSrc
    ./src/Maverick/FileHandler/Saver/Saver.cpp
    ./src/Maverick/FileHandler/Loader/Loader.cpp
)

set(FilterSrc
    ./src/Maverick/Filter/Filter.cpp
    ./src/Maverick/Filter/ColorFilter/ColorFilter.cpp
)

set(EngineSrc
    ./src/Maverick/Engine/Engine.cpp
)

set(MotorSrc
    ${AnimationSrc}
    ${AudioSrc}
    ${HashSrc}
    ${HitboxSrc}
    ${CameraSrc}
    ${ControlSrc}
    ${EntitySrc}
    ${ZoneSrc}
    ${ZoneFactorySrc}
    ${FactorySrc}
    ${FileHandlerSrc}
    ${FilterSrc}
    ${EngineSrc}
)

set(MaverickSrc
    ./src/Maverick/Maverick.cpp
    ${MotorSrc}
)

#Game source
set(CyberenaObjectSrc
    ./src/Cyberena/Entity/Object/Character/Character.cpp
    ./src/Cyberena/Entity/Object/Character/Race/Race.cpp
    ./src/Cyberena/Entity/Object/Character/Style/Style.cpp
    ./src/Cyberena/Entity/Object/GlassBall/GlassBall.cpp
)

set(CyberenaUISrc
)

set(CyberenaEntitySrc
    ${CyberenaObjectSrc}
    ${CyberenaUISrc}
)

set(CyberenaZonesSrc
    ./src/Cyberena/Zones/TestZone/TestZone.cpp
    ./src/Cyberena/Zones/global/global.cpp
    ./src/Cyberena/Zones/FilterTest/FilterTest.cpp
)

set(CyberenaSrc
    ${CyberenaEntitySrc}
    ${CyberenaZonesSrc}
)

#Editor source

set(LedZonesSrc
    ./src/led/Zones/led/led.cpp
)

set(LedUISrc
    ./src/led/Entity/UI/UILevelPort/UILevelPort.cpp
)

set(LedSrc
    ${LedZonesSrc}
    ${LedUISrc}
)

add_executable(editor
    ${MaverickSrc}
    ${CyberenaSrc}
    ${LedSrc}
)

target_link_libraries(editor
    SDL2
    SDL2_image
    SDL2_ttf
    SDL2_mixer
    GL
)
