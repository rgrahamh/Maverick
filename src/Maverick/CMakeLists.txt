cmake_minimum_required(VERSION 3.22)

project(maverick VERSION 0.1.0 DESCRIPTION "Maverick Game Engine, Ryan Houck 2020-2022")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -pthread -Wno-write-strings -Wno-int-to-pointer-cast -fPIC")

#Engine source
set(AnimationSrc
    Animation/Animation.cpp
)

set(AudioSrc
    Audio/Audio.cpp
    Audio/Music/Music.cpp
    Audio/SoundBoard/SoundBoard.cpp
)

set(FontSrc
    Font/Font.cpp
)

set(HashSrc
    HashTable/HashTable.cpp
    HashTable/AttributeHash/AttributeHash.cpp
    HashTable/SpriteHash/SpriteHash.cpp
    HashTable/MusicHash/MusicHash.cpp
    HashTable/SoundHash/SoundHash.cpp
    HashTable/FontHash/FontHash.cpp
)

set(HitboxSrc
    Animation/Hitbox/Hitbox.cpp
    Animation/Hitbox/HitEllipse/HitEllipse.cpp
    Animation/Hitbox/HitEllipse/HitCone/HitCone.cpp
    Animation/Hitbox/HitRect/HitRect.cpp
    Animation/Hitbox/HitboxCollision.cpp
)

set(CameraSrc
    Camera/Camera.cpp
)

set(ControlSrc
    Control/Control.cpp
)

set(UISrc
    Entity/UI/UIElement.cpp
    Entity/UI/UIBorders/UIBorders.cpp
    Entity/UI/UIText/UIText.cpp
    Entity/UI/UITextBox/UITextBox.cpp
    Entity/UI/UIObjectFrame/UIObjectFrame.cpp
    ${FontSrc}
)

set(ObjectSrc
    Entity/Object/Object.cpp
    Entity/Object/CameraReference/CameraReference.cpp
    Entity/Object/LoadingZone/LoadingZone.cpp
)

set(EntitySrc
    Entity/Entity.cpp
    ${ObjectSrc}
    ${UISrc}
)

set(ZoneSrc
    Zone/Zone.cpp
)

set(ZoneFactorySrc
    Zone/ZoneFactory/ZoneFactory.cpp
)

set(FactorySrc
    ../Cyberena/Factory/Factory.cpp
)

set(FileHandlerSrc
    FileHandler/Saver/Saver.cpp
    FileHandler/Loader/Loader.cpp
)

set(EngineSrc
    Engine/Engine.cpp
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
    ${EngineSrc}
)

add_library(maverick SHARED
    ${MotorSrc}
)

target_link_libraries(maverick
    SDL2
    SDL2_image
    SDL2_mixer
    GL
)

set_target_properties(maverick PROPERTIES VERSION ${PROJECT_VERSION})
