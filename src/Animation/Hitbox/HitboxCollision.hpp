#ifndef HITBOXCOLLISION_H
#define HITBOXCOLLISION_H

class HitRect;
class HitEllipse;
class HitCone;

//Rect-base collision
bool collisionRectRect(HitRect* rect1, HitRect* rect2);
bool collisionRectEllipse(HitRect* rect, HitEllipse* ellipse);
bool collisionRectCone(HitRect* rect, HitCone* cone);

//Ellipse-base collision
bool collisionEllipseEllipse(HitEllipse* ellipse1, HitEllipse* ellipse2);
bool collisionEllipseCone(HitEllipse* ellipse, HitCone* cone);

//Cone-base collision
bool collisionConeCone(HitCone* cone1, HitCone* cone2);

#endif