#include <gtest/gtest.h>
#include "../../Maverick/Animation/Hitbox/HitboxCollision.hpp"
#include "../../Maverick/Engine/Engine.hpp"

extern Engine* engine;

TEST(Collision, TestRectRect){
	double x_base = 0;
	double y_base = 0;
	double z_base = 0;

	HitRect rect1(&x_base, &y_base, &z_base, 0, 0, 0, 20, 40, 80, COLLISION);
	HitRect rect2(&x_base, &y_base, &z_base, 0, 0, 0, 20, 40, 80, COLLISION);

	ASSERT_EQ(collisionRectRect(&rect1, &rect2), true);
	ASSERT_EQ(collisionRectRect(&rect2, &rect1), true);

	rect2.setXOffset(25);
	ASSERT_EQ(collisionRectRect(&rect1, &rect2), false);
	ASSERT_EQ(collisionRectRect(&rect2, &rect1), false);

	rect2.setXOffset(-25);
	ASSERT_EQ(collisionRectRect(&rect1, &rect2), false);
	ASSERT_EQ(collisionRectRect(&rect2, &rect1), false);

	rect2.setXOffset(0);
	rect2.setYOffset(45);
	ASSERT_EQ(collisionRectRect(&rect1, &rect2), false);
	ASSERT_EQ(collisionRectRect(&rect2, &rect1), false);

	rect2.setYOffset(-45);
	ASSERT_EQ(collisionRectRect(&rect1, &rect2), false);
	ASSERT_EQ(collisionRectRect(&rect2, &rect1), false);

	rect2.setYOffset(0);
	rect2.setZOffset(85);
	ASSERT_EQ(collisionRectRect(&rect1, &rect2), false);
	ASSERT_EQ(collisionRectRect(&rect2, &rect1), false);

	rect2.setZOffset(-85);
	ASSERT_EQ(collisionRectRect(&rect1, &rect2), false);
	ASSERT_EQ(collisionRectRect(&rect2, &rect1), false);

	rect2.setZOffset(75);
	ASSERT_EQ(collisionRectRect(&rect1, &rect2), true);
	ASSERT_EQ(collisionRectRect(&rect2, &rect1), true);

	rect2.setYOffset(35);
	ASSERT_EQ(collisionRectRect(&rect1, &rect2), true);
	ASSERT_EQ(collisionRectRect(&rect2, &rect1), true);

	rect2.setXOffset(15);
	ASSERT_EQ(collisionRectRect(&rect1, &rect2), true);
	ASSERT_EQ(collisionRectRect(&rect2, &rect1), true);

	rect2.setZOffset(-75);
	ASSERT_EQ(collisionRectRect(&rect1, &rect2), true);
	ASSERT_EQ(collisionRectRect(&rect2, &rect1), true);

	rect2.setYOffset(-35);
	ASSERT_EQ(collisionRectRect(&rect1, &rect2), true);
	ASSERT_EQ(collisionRectRect(&rect2, &rect1), true);

	rect2.setXOffset(-15);
	ASSERT_EQ(collisionRectRect(&rect1, &rect2), true);
	ASSERT_EQ(collisionRectRect(&rect2, &rect1), true);
}

TEST(Collision, TestRectEllipse){
	
}

TEST(Collision, TestEllipseEllipse){
	
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);

    engine = new Engine();

    return RUN_ALL_TESTS();
}