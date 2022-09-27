#include <gtest/gtest.h>
#include "Maverick/Camera/Camera.hpp"
#include "Maverick/Engine/Engine.hpp"
#include "Maverick/Object/CameraReference/CameraReference.hpp"

TEST(CameraBehavior, TestFixedFollow){
    Engine* engine = Engine::getInstance();
    Camera* camera = engine->getCamera();
    int width, height;
    SDL_GetRendererOutputSize(engine->getRenderer(), &width, &height);

    //Test fixed follow first (the easy case)
    camera->setFollowMode(CAMERA_FOLLOW_MODE::FIXED_FOLLOW);

    CameraReference camera_reference("reference", 0, 0);
    camera->setReference((Object*)&camera_reference);
    camera->_draw((ObjectList*)nullptr, 6);
    int camera_x_offset = (width / 2) * -1;
    int camera_y_offset = (height / 2) * -1;
    ASSERT_EQ(camera->getX(), camera_x_offset);
    ASSERT_EQ(camera->getY(), camera_y_offset);

    int x_mod = 500;
    int y_mod = 600;
    camera_reference.setX(x_mod);
    camera_reference.setY(y_mod);
    camera->_draw((ObjectList*)nullptr, 6);
    ASSERT_EQ(camera->getX(), camera_x_offset + x_mod);
    ASSERT_EQ(camera->getY(), camera_y_offset + y_mod);

    x_mod = -1898;
    y_mod = -893;
    camera_reference.setX(x_mod);
    camera_reference.setY(y_mod);
    camera->_draw((ObjectList*)nullptr, 6);
    ASSERT_EQ(camera->getX(), camera_x_offset + x_mod);
    ASSERT_EQ(camera->getY(), camera_y_offset + y_mod);

    camera->setReference(nullptr);
    camera->_draw((ObjectList*)nullptr, 6);
    ASSERT_EQ(camera->getX(), camera_x_offset + x_mod);
    ASSERT_EQ(camera->getY(), camera_y_offset + y_mod);
}

TEST(CameraBehavior, TestGradualFollow){
    Engine* engine = Engine::getInstance();
    Camera* camera = engine->getCamera();
    int width, height;
    SDL_GetRendererOutputSize(engine->getRenderer(), &width, &height);

    double follow_rate = 0.08;
    camera->setFollowRate(follow_rate);

    //Test fixed follow first (the easy case)
    camera->setFollowMode(CAMERA_FOLLOW_MODE::FIXED_FOLLOW);
    CameraReference camera_reference("reference", 0, 0);
    camera->setReference((Object*)&camera_reference);
    camera->_draw((ObjectList*)nullptr, 6);
    double camera_x_pos = (width / 2) * -1;
    double camera_y_pos = (height / 2) * -1;
    ASSERT_EQ(camera->getX(), camera_x_pos);
    ASSERT_EQ(camera->getY(), camera_y_pos);

    camera->setFollowMode(CAMERA_FOLLOW_MODE::GRADUAL_FOLLOW);
    int x_mod = 500;
    int y_mod = 600;
    camera_reference.setX(x_mod);
    camera_reference.setY(y_mod);
    camera->_draw((ObjectList*)nullptr, 6);
    camera_x_pos += (((x_mod - (width / 2)) - camera_x_pos) * follow_rate);
    camera_y_pos += (((y_mod - (height / 2)) - camera_y_pos) * follow_rate);
    ASSERT_EQ(camera->getX(), camera_x_pos);
    ASSERT_EQ(camera->getY(), camera_y_pos);

    x_mod = -1898;
    y_mod = -893;
    camera_reference.setX(x_mod);
    camera_reference.setY(y_mod);
    camera->_draw((ObjectList*)nullptr, 6);
    camera_x_pos += (((x_mod - (width / 2)) - camera_x_pos) * follow_rate);
    camera_y_pos += (((y_mod - (height / 2)) - camera_y_pos) * follow_rate);
    ASSERT_EQ(camera->getX(), camera_x_pos);
    ASSERT_EQ(camera->getY(), camera_y_pos);

    camera->setReference(nullptr);
    camera->_draw((ObjectList*)nullptr, 6);
    ASSERT_EQ(camera->getX(), camera_x_pos);
    ASSERT_EQ(camera->getY(), camera_y_pos);
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}