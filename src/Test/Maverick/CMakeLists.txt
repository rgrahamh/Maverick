cmake_minimum_required(VERSION 3.19)

project(maverick_unit_tests VERSION 0.1.0 DESCRIPTION "Maverick Game Engine Unit Testing, Ryan Houck 2020-2022")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -pipe -pthread -Wno-write-strings -Wno-int-to-pointer-cast -fPIC")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../../../bin/Test")
set(CMAKE_BUILD_PARALLEL_LEVEL "16")

set(LIBS_DIR "../../../libs")

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    link_directories(
        ${LIBS_DIR}/Linux/SDL2/bin
        ${LIBS_DIR}/Linux/SDL2_image/bin
        ${LIBS_DIR}/Linux/SDL2_mixer/bin
        ../../../bin
    )
    include_directories(
        ${LIBS_DIR}/Linux/SDL2/include
        ${LIBS_DIR}/Linux/SDL2/include/SDL2
        ${LIBS_DIR}/Linux/SDL2_image/include
        ${LIBS_DIR}/Linux/SDL2_mixer/include
        ../../../src
    )
else()
    link_directories(
        ${LIBS_DIR}/Windows/SDL2/bin
        ${LIBS_DIR}/Windows/SDL2_image/bin
        ${LIBS_DIR}/Windows/SDL2_mixer/bin
        ../../../bin
    )
    include_directories(
        ${LIBS_DIR}/Windows/SDL2/include
        ${LIBS_DIR}/Windows/SDL2/include/SDL2
        ${LIBS_DIR}/Windows/SDL2_image/include
        ${LIBS_DIR}/Windows/SDL2_mixer/include
        ../../../src
    )
endif()

set(LinkLibs
    SDL2
    SDL2_image
    SDL2_mixer
    maverick
    gtest
)
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set(LinkLibs
        ${LinkLibs}
        GL
    )
endif()

#Animation test
add_executable(TestAnimation
    TestAnimation.cpp
)

target_link_libraries(TestAnimation
    ${LinkLibs}
)

##Audio test
#add_executable(TestAudio
#    ${MotorSrc}
#    TestAudio.cpp
#)
#
#target_link_libraries(TestAudio
#    ${LinkLibs}
#)

#Camera test
add_executable(TestCamera
    ${MotorSrc}
    TestCamera.cpp
)

target_link_libraries(TestCamera
    ${LinkLibs}
)

#Collision test
add_executable(TestCollision
    TestCollision.cpp
)

target_link_libraries(TestCollision
    ${LinkLibs}
)

##Control test
#add_executable(TestControl
#    ${MotorSrc}
#    TestControl.cpp
#)
#
#target_link_libraries(TestControl
#    ${LinkLibs}
#)
#
##Engine test
#add_executable(TestEngine
#    ${MotorSrc}
#    TestEngine.cpp
#)
#
#target_link_libraries(TestEngine
#    ${LinkLibs}
#)
#
##File handler test
#add_executable(TestFileHandler
#    ${MotorSrc}
#    TestFileHandler.cpp
#)
#
#target_link_libraries(TestFileHandler
#    ${LinkLibs}
#)

#Globals test
add_executable(TestGlobal
    TestGlobal.cpp
)

target_link_libraries(TestGlobal
    ${LinkLibs}
)

#Object test
#add_executable(TestObject
#    ${MotorSrc}
#    TestObject.cpp
#)
#
#target_link_libraries(TestObject
#    ${LinkLibs}
#)
#
##UIElement test
#add_executable(TestUIElement
#    ${MotorSrc}
#    TestUIElement.cpp
#)
#
#target_link_libraries(TestUIElement
#    ${LinkLibs}
#)