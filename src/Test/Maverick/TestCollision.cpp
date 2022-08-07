#include <gtest/gtest.h>
#include "../../Maverick/Animation/Hitbox/HitboxCollision.hpp"
#include "../../Maverick/Engine/Engine.hpp"

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
	double x_base = 0;
	double y_base = 0;
	double z_base = 0;

	HitRect rect(&x_base, &y_base, &z_base, 0, 0, 0, 20, 40, 80, COLLISION);
	HitEllipse ellipse(&x_base, &y_base, &z_base, 0, 0, 0, 20, 40, 80, COLLISION);

	ASSERT_EQ(collisionRectEllipse(&rect, &ellipse), true);

	rect.setXOffset(25);
	ASSERT_EQ(collisionRectEllipse(&rect, &ellipse), false);

	rect.setXOffset(-45);
	ASSERT_EQ(collisionRectEllipse(&rect, &ellipse), false);

	rect.setXOffset(0);
	rect.setYOffset(45);
	ASSERT_EQ(collisionRectEllipse(&rect, &ellipse), false);

	rect.setYOffset(-85);
	ASSERT_EQ(collisionRectEllipse(&rect, &ellipse), false);

	rect.setYOffset(0);
	rect.setZOffset(85);
	ASSERT_EQ(collisionRectEllipse(&rect, &ellipse), false);

	rect.setZOffset(-85);
	ASSERT_EQ(collisionRectEllipse(&rect, &ellipse), false);

	rect.setZOffset(75);
	ASSERT_EQ(collisionRectEllipse(&rect, &ellipse), true);

	rect.setYOffset(35);
	ASSERT_EQ(collisionRectEllipse(&rect, &ellipse), true);

	rect.setXOffset(15);
	ASSERT_EQ(collisionRectEllipse(&rect, &ellipse), true);

	rect.setZOffset(-75);
	ASSERT_EQ(collisionRectEllipse(&rect, &ellipse), true);

	rect.setYOffset(-35);
	ASSERT_EQ(collisionRectEllipse(&rect, &ellipse), true);

	rect.setXOffset(-15);
	ASSERT_EQ(collisionRectEllipse(&rect, &ellipse), true);
}

TEST(Collision, TestEllipseEllipse){
	double x_base = 0;
	double y_base = 0;
	double z_base = 0;

	HitEllipse ellipse1(&x_base, &y_base, &z_base, 0, 0, 0, 20, 40, 80, COLLISION);
	HitEllipse ellipse2(&x_base, &y_base, &z_base, 0, 0, 0, 20, 40, 80, COLLISION);

	ASSERT_EQ(collisionEllipseEllipse(&ellipse1, &ellipse2), true);
	ASSERT_EQ(collisionEllipseEllipse(&ellipse2, &ellipse1), true);

	ellipse2.setXOffset(40);
	ASSERT_EQ(collisionEllipseEllipse(&ellipse1, &ellipse2), false);
	ASSERT_EQ(collisionEllipseEllipse(&ellipse2, &ellipse1), false);

	ellipse2.setXOffset(-40);
	ASSERT_EQ(collisionEllipseEllipse(&ellipse1, &ellipse2), false);
	ASSERT_EQ(collisionEllipseEllipse(&ellipse2, &ellipse1), false);

	ellipse2.setXOffset(0);
	ellipse2.setYOffset(80);
	ASSERT_EQ(collisionEllipseEllipse(&ellipse1, &ellipse2), false);
	ASSERT_EQ(collisionEllipseEllipse(&ellipse2, &ellipse1), false);

	ellipse2.setYOffset(-80);
	ASSERT_EQ(collisionEllipseEllipse(&ellipse1, &ellipse2), false);
	ASSERT_EQ(collisionEllipseEllipse(&ellipse2, &ellipse1), false);

	ellipse2.setYOffset(0);
	ellipse2.setZOffset(85);
	ASSERT_EQ(collisionEllipseEllipse(&ellipse1, &ellipse2), false);
	ASSERT_EQ(collisionEllipseEllipse(&ellipse2, &ellipse1), false);

	ellipse2.setZOffset(-85);
	ASSERT_EQ(collisionEllipseEllipse(&ellipse1, &ellipse2), false);
	ASSERT_EQ(collisionEllipseEllipse(&ellipse2, &ellipse1), false);

	ellipse2.setZOffset(75);
	ASSERT_EQ(collisionEllipseEllipse(&ellipse1, &ellipse2), true);
	ASSERT_EQ(collisionEllipseEllipse(&ellipse2, &ellipse1), true);

	ellipse2.setYOffset(35);
	ASSERT_EQ(collisionEllipseEllipse(&ellipse1, &ellipse2), true);
	ASSERT_EQ(collisionEllipseEllipse(&ellipse2, &ellipse1), true);

	ellipse2.setXOffset(15);
	ASSERT_EQ(collisionEllipseEllipse(&ellipse1, &ellipse2), true);
	ASSERT_EQ(collisionEllipseEllipse(&ellipse2, &ellipse1), true);

	ellipse2.setZOffset(-75);
	ASSERT_EQ(collisionEllipseEllipse(&ellipse1, &ellipse2), true);
	ASSERT_EQ(collisionEllipseEllipse(&ellipse2, &ellipse1), true);

	ellipse2.setYOffset(-35);
	ASSERT_EQ(collisionEllipseEllipse(&ellipse1, &ellipse2), true);
	ASSERT_EQ(collisionEllipseEllipse(&ellipse2, &ellipse1), true);

	ellipse2.setXOffset(-15);
	ASSERT_EQ(collisionEllipseEllipse(&ellipse1, &ellipse2), true);
	ASSERT_EQ(collisionEllipseEllipse(&ellipse2, &ellipse1), true);
}

int main(int argc, char** argv){
    testing::InitGoogleTest(&argc, argv);

    Engine* engine = Engine::getInstance();

    return RUN_ALL_TESTS();
}