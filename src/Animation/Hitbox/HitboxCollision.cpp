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

	float ellipse_top_bound = ellipse->getTopBound();
	float ellipse_bot_bound = ellipse->getBotBound();
	float ellipse_left_bound = ellipse->getLeftBound();
	float ellipse_right_bound = ellipse->getRightBound();
	
	if(!(((rect_left_bound < ellipse_left_bound && rect_right_bound > ellipse_left_bound) || (ellipse_left_bound < rect_left_bound && ellipse_right_bound > rect_left_bound))
	&& ((rect_top_bound < ellipse_top_bound && rect_bot_bound > ellipse_top_bound) || (ellipse_top_bound < rect_top_bound && ellipse_bot_bound > rect_top_bound)))){
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

	float rect2_top_bound = rect2->getTopBound();
	float rect2_bot_bound = rect2->getBotBound();
	float rect2_left_bound = rect2->getLeftBound();
	float rect2_right_bound = rect2->getRightBound();
	
	if(((rect1_left_bound < rect2_left_bound && rect1_right_bound > rect2_left_bound) || (rect2_left_bound < rect1_left_bound && rect2_right_bound > rect1_left_bound))
	&& ((rect1_top_bound < rect2_top_bound && rect1_bot_bound > rect2_top_bound) || (rect2_top_bound < rect1_top_bound && rect2_bot_bound > rect1_top_bound))){
		return true;
	}

	return false;
}

bool collisionRectCone(HitRect* rect, HitCone* cone){
	return false;
}

/** Ellipse-ellipse-based collision
 * @param ellipse1 The first ellipse
 * @param ellipse2 The second ellipse
 * @return If the two ellipses are colliding
 */
bool collisionEllipseEllipse(HitEllipse* ellipse1, HitEllipse* ellipse2){
	float ellipse1_top_bound = ellipse1->getTopBound();
	float ellipse1_bot_bound = ellipse1->getBotBound();
	float ellipse1_left_bound = ellipse1->getLeftBound();
	float ellipse1_right_bound = ellipse1->getRightBound();

	float ellipse2_top_bound = ellipse2->getTopBound();
	float ellipse2_bot_bound = ellipse2->getBotBound();
	float ellipse2_left_bound = ellipse2->getLeftBound();
	float ellipse2_right_bound = ellipse2->getRightBound();
	
	if(!(((ellipse1_left_bound < ellipse2_left_bound && ellipse1_right_bound > ellipse2_left_bound) || (ellipse2_left_bound < ellipse1_left_bound && ellipse2_right_bound > ellipse1_left_bound))
	&& ((ellipse1_top_bound < ellipse2_top_bound && ellipse1_bot_bound > ellipse2_top_bound) || (ellipse2_top_bound < ellipse1_top_bound && ellipse2_bot_bound > ellipse1_top_bound)))){
		return false;
	}

	//At this point, we know that the rect is at least partially inside of the bounding box
	float ellipse1_x = ellipse1->getX();
	float ellipse1_y = ellipse1->getY();
	float ellipse1_x_radius = ellipse1->getXRadius();
	float ellipse1_y_radius = ellipse1->getYRadius();

	float ellipse2_x = ellipse2->getX();
	float ellipse2_y = ellipse2->getY();
	float ellipse2_x_radius = ellipse2->getXRadius();
	float ellipse2_y_radius = ellipse2->getYRadius();

	//Quicker to look up than to compute
	float ellipse_45 = 0.70710678118;

	float ellipse1_x_45 = ellipse1_x_radius * ellipse_45;
	float ellipse1_y_45 = ellipse1_y_radius * ellipse_45;

	float ellipse2_x_45 = ellipse2_x_radius * ellipse_45;
	float ellipse2_y_45 = ellipse2_y_radius * ellipse_45;

	//We calc if the first ellipse's points of interest are inside of the other
	if(ellipse2->isPointInside(ellipse1_left_bound, ellipse1_y)
			|| ellipse2->isPointInside(ellipse1_right_bound, ellipse1_y)
			|| ellipse2->isPointInside(ellipse1_x, ellipse1_bot_bound)
			|| ellipse2->isPointInside(ellipse1_x, ellipse1_top_bound)
			|| ellipse2->isPointInside(ellipse1_x + ellipse1_x_45, ellipse1_y + ellipse1_y_45)
			|| ellipse2->isPointInside(ellipse1_x + ellipse1_x_45, ellipse1_y - ellipse1_y_45)
			|| ellipse2->isPointInside(ellipse1_x - ellipse1_x_45, ellipse1_y + ellipse1_y_45)
			|| ellipse2->isPointInside(ellipse1_x - ellipse1_x_45, ellipse1_y - ellipse1_y_45)){
		return true;
	}
	//We test if the ellipse's points of interest are inside of the other ellipse
	if(ellipse1->isPointInside(ellipse2_left_bound, ellipse2_y)
			|| ellipse1->isPointInside(ellipse2_right_bound, ellipse2_y)
			|| ellipse1->isPointInside(ellipse2_x, ellipse2_bot_bound)
			|| ellipse1->isPointInside(ellipse2_x, ellipse2_top_bound)
			|| ellipse1->isPointInside(ellipse2_x + ellipse2_x_45, ellipse2_y + ellipse2_y_45)
			|| ellipse1->isPointInside(ellipse2_x + ellipse2_x_45, ellipse2_y - ellipse2_y_45)
			|| ellipse1->isPointInside(ellipse2_x - ellipse2_x_45, ellipse2_y + ellipse2_y_45)
			|| ellipse1->isPointInside(ellipse2_x - ellipse2_x_45, ellipse2_y - ellipse2_y_45)){
		return true;
	}

	return false;
}

bool collisionEllipseCone(HitEllipse* ellipse, HitCone* cone){
	return false;
}

//Cone-base collision
bool collisionConeCone(HitCone* cone1, HitCone* cone2){
	return false;
}