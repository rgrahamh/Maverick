#include <gtest/gtest.h>
#include "Maverick/Camera/Camera.hpp"
#include "Maverick/Engine/Engine.hpp"
#include "Maverick/Object/CameraReference/CameraReference.hpp"

TEST(CameraBehavior, TestFixedFollow){
    Engine* engine = Engine::getInstance();
    Camera* camera = engine->getCamera();
    int win_width, win_height;
    SDL_GetRendererOutputSize(engine->getRenderer(), &win_width, &win_height);
    double width = win_width;
    double height = win_height;
    double width_scale = width / DESIGN_SCREEN_WIDTH;
    double height_scale = height / DESIGN_SCREEN_HEIGHT;

    //Test fixed follow first (the easy case)
    camera->setFollowMode(CAMERA_FOLLOW_MODE::FIXED_FOLLOW);

    CameraReference camera_reference("reference", 0, 0);
    camera->setReference((Object*)&camera_reference);
    camera->_draw((ObjectList*)nullptr, 6);
    double camera_x_offset = (width / 2) * -1;
    double camera_y_offset = (height / 2) * -1;
    ASSERT_EQ(camera->getX(), camera_x_offset / width_scale);
    ASSERT_EQ(camera->getY(), camera_y_offset / height_scale);

    double x_mod = 500;
    double y_mod = 600;
    camera_reference.setX(x_mod);
    camera_reference.setY(y_mod);
    camera->_draw((ObjectList*)nullptr, 6);
    ASSERT_EQ(camera->getX(), camera_x_offset / width_scale + x_mod);
    ASSERT_EQ(camera->getY(), camera_y_offset / height_scale + y_mod);

    x_mod = -1898;
    y_mod = -893;
    camera_reference.setX(x_mod);
    camera_reference.setY(y_mod);
    camera->_draw((ObjectList*)nullptr, 6);
    ASSERT_EQ(camera->getX(), camera_x_offset / width_scale + x_mod);
    ASSERT_EQ(camera->getY(), camera_y_offset / height_scale + y_mod);

    camera->setReference(nullptr);
    camera->_draw((ObjectList*)nullptr, 6);
    ASSERT_EQ(camera->getX(), camera_x_offset / width_scale + x_mod);
    ASSERT_EQ(camera->getY(), camera_y_offset / height_scale + y_mod);
}

TEST(CameraBehavior, TestGradualFollow){
    Engine* engine = Engine::getInstance();
    Camera* camera = engine->getCamera();
    int win_width, win_height;
    SDL_GetRendererOutputSize(engine->getRenderer(), &win_width, &win_height);
    double width = win_width;
    double height = win_height;
    double width_scale = win_width / DESIGN_SCREEN_WIDTH;
    double height_scale = win_height / DESIGN_SCREEN_HEIGHT;

    double follow_rate = 0.08;
    camera->setFollowRate(follow_rate);

    //Test fixed follow first (the easy case)
    camera->setFollowMode(CAMERA_FOLLOW_MODE::FIXED_FOLLOW);
    CameraReference camera_reference("reference", 0, 0);
    camera->setReference((Object*)&camera_reference);
    camera->_draw((ObjectList*)nullptr, 6);
    double camera_x_pos = (width / width_scale / 2) * -1;
    double camera_y_pos = (height / height_scale / 2) * -1;
    ASSERT_EQ(camera->getX(), camera_x_pos);
    ASSERT_EQ(camera->getY(), camera_y_pos);

    camera->setFollowMode(CAMERA_FOLLOW_MODE::GRADUAL_FOLLOW);
    double x_mod = 500;
    double y_mod = 600;
    camera_reference.setX(x_mod);
    camera_reference.setY(y_mod);
    camera->_draw((ObjectList*)nullptr, 6);
    camera_x_pos += (((x_mod - (width / width_scale / 2)) - camera_x_pos) * follow_rate);
    camera_y_pos += (((y_mod - (height / height_scale / 2)) - camera_y_pos) * follow_rate);
    ASSERT_EQ(camera->getX(), camera_x_pos);
    ASSERT_EQ(camera->getY(), camera_y_pos);

    x_mod = -1898;
    y_mod = -893;
    camera_reference.setX(x_mod);
    camera_reference.setY(y_mod);
    camera->_draw((ObjectList*)nullptr, 6);
    camera_x_pos += (((x_mod - (width / width_scale / 2)) - camera_x_pos) * follow_rate);
    camera_y_pos += (((y_mod - (height / height_scale / 2)) - camera_y_pos) * follow_rate);
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