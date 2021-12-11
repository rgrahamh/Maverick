cmake_minimum_required(VERSION 2.2)

project(Cyberena)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -std=c++17 -pthread -Wno-write-strings")

set(AnimationSrc
    ./src/Maverick/Animation/Animation.cpp
)

set(HashSrc
    ./src/Maverick/HashTable/HashTable.cpp
    ./src/Maverick/HashTable/TextureHash/TextureHash.cpp
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
    ./src/Maverick/Entity/UI/UIScreenBlit/UIScreenBlit.cpp
    ./src/Maverick/Entity/UI/UIObjectFrame/UIObjectFrame.cpp
)

set(ObjectSrc
    ./src/Maverick/Entity/Object/Object.cpp
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

set(EngineSrc
    ./src/Maverick/Engine/Engine.cpp
)

set(MotorSrc
    ${AnimationSrc}
    ${HashSrc}
    ${HitboxSrc}
    ${CameraSrc}
    ${ControlSrc}
    ${EntitySrc}
    ${ZoneSrc}
    ${ZoneFactorySrc}
    ${FactorySrc}
    ${EngineSrc}
)

set(MaverickSrc
    ./src/Maverick/Maverick.cpp
    ${MotorSrc}
)

#Cyberena source
set(CyberenaObjectSrc
    ./src/Cyberena/Entity/Object/Character/Character.cpp
    ./src/Cyberena/Entity/Object/Character/Race/Race.cpp
    ./src/Cyberena/Entity/Object/Character/Style/Style.cpp
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
)

set(CyberenaSrc
    ${CyberenaEntitySrc}
    ${CyberenaZonesSrc}
)

#Editor source

set(LedZonesSrc
    ./src/led/Zones/led/led.cpp
)

set(LedSrc
    ${LedZonesSrc}
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
    GL
)