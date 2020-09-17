#include "./HitboxCollision.hpp"

#include "./HitRect/HitRect.hpp"
#include "./HitEllipse/HitEllipse.hpp"
#include "./HitEllipse/HitCone/HitCone.hpp"

bool collisionRectEllipse(HitRect* rect, HitEllipse* ellipse){
	float rect_x = rect->getX();
	float rect_y = rect->getX();
	float rect_width = rect->getWidth();
	float rect_height = rect->getHeight();

	float ellipse_x = ellipse->getX();
	float ellipse_y = ellipse->getY();
	float ellipse_x_radius = ellipse->getXRadius();
	float ellipse_y_radius = ellipse->getYRadius();

	//Check of rect is outside of the bounding box (common case, so it's faster than going through the rest of the floating point operations)
	if(rect_x + rect_width < ellipse_x - ellipse_x_radius
	|| rect_x > ellipse_x + ellipse_x_radius
	|| rect->getY() + rect->getHeight() < ellipse_y - ellipse_y_radius
	|| rect->getY() > ellipse_y + ellipse_y_radius){
		return false;
	}
	//At this point, we know that the rect is at least partially inside of the bounding box. We calc if the square's corners are inside of the ellipse
	else if(rect->isPointInside(ellipse_x - ellipse_x_radius, ellipse_y)
			|| rect->isPointInside(ellipse_x + ellipse_x_radius, ellipse_y)
			|| rect->isPointInside(ellipse_x, ellipse_y + ellipse_y_radius)
			|| rect->isPointInside(ellipse_x, ellipse_y - ellipse_y_radius)){
		return true;
	}
	//We test if the ellipse's points of interest are inside of the square 
	else if(ellipse->isPointInside(rect_x, rect_y)
	     || ellipse->isPointInside(rect_x + rect_width, rect_y)
		 || ellipse->isPointInside(rect_x, rect_y + rect_height)
		 || ellipse->isPointInside(rect_x + rect_width, rect_y + rect_height)){
		return true;
	}

	return false;
}

bool collisionRectRect(HitRect* rect1, HitRect* rect2){
	return false;
}

bool collisionRectCone(HitRect* rect, HitCone* cone){
	return false;
}

//Ellipse-base collision
bool collisionEllipseEllipse(HitEllipse* ellipse1, HitEllipse* ellipse2){
	return false;
}

bool collisionEllipseCone(HitEllipse* ellipse, HitCone* cone){
	return false;
}

//Cone-base collision
bool collisionConeCone(HitCone* cone1, HitCone* cone2){
	return false;
}