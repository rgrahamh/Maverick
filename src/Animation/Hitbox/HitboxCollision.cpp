#include "./HitboxCollision.hpp"

#include "./HitRect/HitRect.hpp"
#include "./HitEllipse/HitEllipse.hpp"
#include "./HitEllipse/HitCone/HitCone.hpp"

bool collisionRectEllipse(HitRect* rect, HitEllipse* ellipse){
	float rect_x = rect->getX();
	float rect_y = rect->getY();
	float rect_width = rect->getWidth();
	float rect_height = rect->getHeight();

	float ellipse_x = ellipse->getX();
	float ellipse_y = ellipse->getY();
	float ellipse_x_radius = ellipse->getXRadius();
	float ellipse_y_radius = ellipse->getYRadius();

	//Quicker to look up than to compute
	float ellipse_60 = 0.86602540378;
	float ellipse_45 = 0.70710678118;
	float ellipse_30 = 0.5;

	//At this point, we know that the rect is at least partially inside of the bounding box. We calc if the square's corners are inside of the ellipse
	if(rect->isPointInside(ellipse_x - ellipse_x_radius, ellipse_y)
			|| rect->isPointInside(ellipse_x + ellipse_x_radius, ellipse_y)
			|| rect->isPointInside(ellipse_x, ellipse_y + ellipse_y_radius)
			|| rect->isPointInside(ellipse_x, ellipse_y - ellipse_y_radius)
			/*|| rect->isPointInside(ellipse_x + (ellipse_x_radius * ellipse_45), ellipse_y + (ellipse_y_radius * ellipse_45))
			|| rect->isPointInside(ellipse_x + (ellipse_x_radius * ellipse_45), ellipse_y - (ellipse_y_radius * ellipse_45))
			|| rect->isPointInside(ellipse_x - (ellipse_x_radius * ellipse_45), ellipse_y + (ellipse_y_radius * ellipse_45))
			|| rect->isPointInside(ellipse_x - (ellipse_x_radius * ellipse_45), ellipse_y - (ellipse_y_radius * ellipse_45))
			|| rect->isPointInside(ellipse_x + (ellipse_x_radius * ellipse_30), ellipse_y + (ellipse_y_radius * ellipse_60))
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
	else if(ellipse->isPointInside(rect_x, rect_y)
	     || ellipse->isPointInside(rect_x + rect_width, rect_y)
		 || ellipse->isPointInside(rect_x, rect_y + rect_height)
		 || ellipse->isPointInside(rect_x + rect_width, rect_y + rect_height)){
		return true;
	}

	return false;
}

bool collisionRectRect(HitRect* rect1, HitRect* rect2){
	float rect1_x = rect1->getX();
	float rect1_y = rect1->getX();
	float rect1_width = rect1->getWidth();
	float rect1_height = rect1->getHeight();

	float rect2_x = rect2->getX();
	float rect2_y = rect2->getX();
	float rect2_width = rect2->getWidth();
	float rect2_height = rect2->getHeight();
	
	if(rect1_x + rect1_width < rect2_x
	|| rect1_x > rect2_x + rect2_y
	|| rect1_y + rect1_height < rect2_y
	|| rect1_y > rect2_y + rect2_height){
		return true;
	}

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