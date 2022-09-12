#include <gtest/gtest.h>
#include "Maverick/Animation/Animation.hpp"
#include "Maverick/Engine/Engine.hpp"

TEST(AnimationProperties, TestSpriteSet){
    Engine* engine = Engine::getInstance();
    double x_base, y_base;
    Animation* animation = new Animation("Test Animation", &x_base, &y_base, 2);

    animation->addFrame(60);
    animation->addSpriteSet("day");
    ASSERT_EQ(animation->hasSpriteSet("day"), true);
    animation->addSpriteSet("night");
    ASSERT_EQ(animation->hasSpriteSet("night"), true);

    ASSERT_EQ(animation->addSprite("day", "test_sprite1", 58, 68), 0);
    ASSERT_EQ(animation->addSprite("night", "test_sprite2", 58, 68), 0);

    animation->setSpriteSet("day");
    ASSERT_NE(animation->getSprite(), nullptr);
    ASSERT_EQ(animation->getSprite()->surface, engine->getSurface("test_sprite1"));
    animation->setSpriteSet("night");
    ASSERT_NE(animation->getSprite(), nullptr);
    ASSERT_EQ(animation->getSprite()->surface, engine->getSurface("test_sprite2"));

    delete animation;
}

TEST(AnimationBehavior, TestAdvance){
    Engine* engine = Engine::getInstance();
    double x_base, y_base, z_base;
    Animation* animation = new Animation("Test Animation", &x_base, &y_base, 1);
    animation->addFrame(60);
    animation->addSpriteSet("day");
    animation->addSprite("day", "test_sprite1", 58, 68);
    animation->addSound("slice", 0);
    HitRect* rect = new HitRect(&x_base, &y_base, &z_base, 0, 0, 0, 1, 1, 1, HITBOX_TYPE::COLLISION);
    animation->addHitbox((Hitbox*)rect, 0);

    animation->addFrame(84);
    animation->addSprite("day", "test_sprite2", 58, 68);
    animation->addSound("step", 1);
    HitEllipse* ellipse = new HitEllipse(&x_base, &y_base, &z_base, 0, 0, 0, 1, 1, 1, HITBOX_TYPE::COLLISION);
    animation->addHitbox((Hitbox*)ellipse, 0);

    animation->addFrame(104);
    ASSERT_EQ(animation->getTimeLeft(), 248);

    //Looping
    animation->setNextAnimation(animation);
    animation->start();

    //At the first frame still
    animation->advance(59);
    ASSERT_EQ(animation->getTimeLeft(), 189);
    ASSERT_EQ(animation->getSprite()->surface, engine->getSurface("test_sprite1"));
    ASSERT_EQ(animation->getSound(), engine->getSound("slice"));

    //Partway through the second frame
    animation->advance(60);
    ASSERT_EQ(animation->getTimeLeft(), 129);
    ASSERT_EQ(animation->getSprite()->surface, engine->getSurface("test_sprite2"));
    ASSERT_EQ(animation->getSound(), engine->getSound("step"));

    //Partway through the third frame (intentionally empty)
    animation->advance(60);
    ASSERT_EQ(animation->getTimeLeft(), 69);
    ASSERT_EQ(animation->getHitboxes(), nullptr);
    ASSERT_EQ(animation->getSound(), nullptr);
    ASSERT_EQ(animation->getSprite(), nullptr);

    //Testing pausing
    animation->setPaused(true);
    ASSERT_EQ(animation->getPaused(), true);
    animation->advance(60);
    ASSERT_EQ(animation->getTimeLeft(), 69);
    animation->setPaused(false);
    animation->advance(69);

    //Should be back at the first sprite
    ASSERT_NE(animation->getSprite(), nullptr);
    ASSERT_EQ(animation->getSprite()->surface, engine->getSurface("test_sprite1"));
    ASSERT_EQ(animation->getSound(), engine->getSound("slice"));
    ASSERT_EQ(animation->getTimeLeft(), 248);

    //Remove the loop; should stay on the last frame
    animation->setNextAnimation(nullptr);
    animation->advance(260);
    ASSERT_EQ(animation->getTimeLeft(), 0);
    ASSERT_EQ(animation->getHitboxes(), nullptr);
    ASSERT_EQ(animation->getSound(), nullptr);
    ASSERT_EQ(animation->getSprite(), nullptr);

    delete animation;
}

//Add serialization/deserialzation

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);

    Engine* engine = Engine::getInstance();

    //Adding sounds that we'll use
    Sound slice;
    slice.name = "slice";
    slice.sample = Mix_LoadWAV("../../assets/audio/effects/slice.wav");
    engine->addSound("slice", &slice);

    Sound step;
    step.name = "step";
    step.sample = Mix_LoadWAV("../../assets/audio/effects/step.wav");
    engine->addSound("step", &step);

    engine->addSurface("test_sprite1", SDL_LoadBMP("../../assets/sprites/test_assets/GlassBloodBall/GlassBloodBallNeutral.bmp"));
    engine->addSurface("test_sprite2", SDL_LoadBMP("../../assets/sprites/test_assets/GlassBloodBall/GlassBloodBallRolling1.bmp"));

    return RUN_ALL_TESTS();
}