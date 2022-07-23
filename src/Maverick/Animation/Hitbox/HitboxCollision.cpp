#include "./HitboxCollision.hpp"

#include "./HitRect/HitRect.hpp"
#include "./HitEllipse/HitEllipse.hpp"
#include "./HitEllipse/HitCone/HitCone.hpp"

/** Rect-Ellipse-based collision detection
 * @param rect The rectangle
 * @param ellipse The ellipse
 * @return If the rect and ellipse are colliding
 */
bool collisionRectEllipse(HitRect* rect, HitEllipse* ellipse){
	float rect_top_bound = rect->getTopBound();
	float rect_bot_bound = rect->getBotBound();
	float rect_left_bound = rect->getLeftBound();
	float rect_right_bound = rect->getRightBound();
	float rect_max_height = rect->getZMax();
	float rect_min_height = rect->getZMin();

	float ellipse_top_bound = ellipse->getTopBound();
	float ellipse_bot_bound = ellipse->getBotBound();
	float ellipse_left_bound = ellipse->getLeftBound();
	float ellipse_right_bound = ellipse->getRightBound();
	float ellipse_max_height = ellipse->getZMax();
	float ellipse_min_height = ellipse->getZMin();
	
	if(!(((rect_left_bound < ellipse_left_bound && rect_right_bound > ellipse_left_bound) || (ellipse_left_bound < rect_left_bound && ellipse_right_bound > rect_left_bound))
	&& ((rect_top_bound < ellipse_top_bound && rect_bot_bound > ellipse_top_bound) || (ellipse_top_bound < rect_top_bound && ellipse_bot_bound > rect_top_bound)))
	|| (rect_max_height <= ellipse_min_height || ellipse_max_height <= rect_min_height)){
		return false;
	}

	//At this point, we know that the rect is at least partially inside of the bounding box
	float ellipse_x = ellipse->getX();
	float ellipse_y = ellipse->getY();
	float ellipse_x_radius = ellipse->getXRadius();
	float ellipse_y_radius = ellipse->getYRadius();

	//Quicker to look up than to compute
	//float ellipse_60 = 0.86602540378;
	float ellipse_45 = 0.70710678118;
	//float ellipse_30 = 0.5;

	float ellipse_x_45 = ellipse_x_radius * ellipse_45;
	float ellipse_y_45 = ellipse_y_radius * ellipse_45;

	//We calc if the square's corners are inside of the ellipse
	if(rect->isPointInside(ellipse_left_bound, ellipse_y)
			|| rect->isPointInside(ellipse_right_bound, ellipse_y)
			|| rect->isPointInside(ellipse_x, ellipse_bot_bound)
			|| rect->isPointInside(ellipse_x, ellipse_top_bound)
			|| rect->isPointInside(ellipse_x + ellipse_x_45, ellipse_y + ellipse_y_45)
			|| rect->isPointInside(ellipse_x + ellipse_x_45, ellipse_y - ellipse_y_45)
			|| rect->isPointInside(ellipse_x - ellipse_x_45, ellipse_y + ellipse_y_45)
			|| rect->isPointInside(ellipse_x - ellipse_x_45, ellipse_y - ellipse_y_45)
			/*|| rect->isPointInside(ellipse_x + (ellipse_x_radius * ellipse_30), ellipse_y + (ellipse_y_radius * ellipse_60))
			|| rect->isPointInside(ellipse_x + (ellipse_x_radius * ellipse_30), ellipse_y - (ellipse_y_radius * ellipse_60))
			|| rect->isPointInside(ellipse_x - (ellipse_x_radius * ellipse_30), ellipse_y + (ellipse_y_radius * ellipse_60))
			|| rect->isPointInside(ellipse_x - (ellipse_x_radius * ellipse_30), ellipse_y - (ellipse_y_radius * ellipse_60))
			|| rect->isPointInside(ellipse_x + (ellipse_x_radius * ellipse_60), ellipse_y + (ellipse_y_radius * ellipse_30))
			|| rect->isPointInside(ellipse_x + (ellipse_x_radius * ellipse_60), ellipse_y - (ellipse_y_radius * ellipse_30))
			|| rect->isPointInside(ellipse_x - (ellipse_x_radius * ellipse_60), ellipse_y + (ellipse_y_radius * ellipse_30))
			|| rect->isPointInside(ellipse_x - (ellipse_x_radius * ellipse_60), ellipse_y - (ellipse_y_radius * ellipse_30))*/){
		return true;
	}
	//We test if the ellipse's points of interest are inside of the square 
	else if(ellipse->isPointInside(rect_left_bound, rect_top_bound)
	     || ellipse->isPointInside(rect_right_bound, rect_top_bound)
		 || ellipse->isPointInside(rect_left_bound, rect_bot_bound)
		 || ellipse->isPointInside(rect_right_bound, rect_bot_bound)){
		return true;
	}

	return false;
}

/** AABB collision detection
 * @param rect1 The first rectangle
 * @param rect2 The second rectangle
 * @return If the two rects are colliding
 */
bool collisionRectRect(HitRect* rect1, HitRect* rect2){
	float rect1_top_bound = rect1->getTopBound();
	float rect1_bot_bound = rect1->getBotBound();
	float rect1_left_bound = rect1->getLeftBound();
	float rect1_right_bound = rect1->getRightBound();
	float rect1_max_height = rect1->getZMax();
	float rect1_min_height = rect1->getZMin();

	float rect2_top_bound = rect2->getTopBound();
	float rect2_bot_bound = rect2->getBotBound();
	float rect2_left_bound = rect2->getLeftBound();
	float rect2_right_bound = rect2->getRightBound();
	float rect2_max_height = rect2->getZMax();
	float rect2_min_height = rect2->getZMin();
	
	if(((rect1_left_bound <= rect2_left_bound && rect1_right_bound >= rect2_left_bound) || (rect2_left_bound <= rect1_left_bound && rect2_right_bound >= rect1_left_bound))
	&& ((rect1_top_bound <= rect2_top_bound && rect1_bot_bound >= rect2_top_bound) || (rect2_top_bound <= rect1_top_bound && rect2_bot_bound >= rect1_top_bound))
	&& !(rect1_max_height <= rect2_min_height || rect2_max_height <= rect1_min_height)){
		return true;
	}

	return false;
}

//Hard-coded to false for now; will come back to this
bool collisionRectCone(HitRect* rect, HitCone* cone){
	return false;
}

/** Ellipse-ellipse-based collision
 * @param ellipse1 The first ellipse
 * @param ellipse2 The second ellipse
 * @return If the two ellipses are colliding
 */
bool collisionEllipseEllipse(HitEllipse* ellipse1, HitEllipse* ellipse2){
	double angle = atan2(ellipse2->getY() - ellipse1->getY(), ellipse2->getX() - ellipse1->getX());
	double sin_angle = sin(angle);
	double cos_angle = cos(angle);

	float ellipse1_max_height = ellipse1->getZMax();
	float ellipse1_min_height = ellipse1->getZMin();
	float ellipse2_max_height = ellipse2->getZMax();
	float ellipse2_min_height = ellipse2->getZMin();

	return (ellipse2->isPointInside(ellipse1->getX() + (ellipse1->getXRadius() * cos_angle), ellipse1->getY() + (ellipse1->getYRadius() * sin_angle)) ||
	       ellipse1->isPointInside(ellipse2->getX() + (ellipse2->getXRadius() * (cos_angle * -1)), ellipse2->getY() + (ellipse2->getYRadius() * (sin_angle * -1)))) &&
		   !(ellipse1_max_height <= ellipse2_min_height || ellipse2_max_height <= ellipse1_min_height);
}

bool collisionEllipseCone(HitEllipse* ellipse, HitCone* cone){
	return false;
}

//Cone-base collision
bool collisionConeCone(HitCone* cone1, HitCone* cone2){
	return false;
}