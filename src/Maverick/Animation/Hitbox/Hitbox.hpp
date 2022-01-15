#ifndef HITBOX_H
#define HITBOX_H

#include <inttypes.h>

//The 
enum HITBOX_SHAPE{
    RECT,
    ELLIPSE,
    CONE
};

enum HITBOX_TYPE{
    ENVIRONMENT = 1,  //Environmental element (can skip check against other environmental collisions)
    COLLISION = 2,    //A more general collision hitbox
    MOVABLE = 4,      //Bumping into this collision hitbox will cause the object to move
    DAMAGEBOX = 8,    //A damaging hitbox
    HURTBOX = 16,      //A hitbox that you can get damaged
    PROJECTILE = 32   //A projectile hitbox
};

class Hitbox{
    public:
        Hitbox(double* x_base, double* y_base, double x_offset, double y_offset, uint64_t type, int32_t hitbox_group = -1, int32_t hitbox_disable_timer = 0);
        virtual ~Hitbox();

        float getX();
        float getY();

        virtual float getRightBound();
        virtual float getLeftBound();
        virtual float getTopBound();
        virtual float getBotBound();
        virtual float getDrawAxis();

        HITBOX_SHAPE getShape();
        unsigned int getType();

        int32_t getHitboxGroup();
        int32_t getHitboxDisableTimer();

        virtual void setScale(double x_scale, double y_scale);

        virtual bool isPointInside(double x_coord, double y_coord);
        virtual bool checkCollision(Hitbox* other);
        
    protected:
		//Pointers to the X and Y base coords
		double* x_base;
		double* y_base;

        double x_base_offset;
        double y_base_offset;

        double x_curr_offset;
        double y_curr_offset;

        HITBOX_SHAPE shape;
        uint64_t type;

        int32_t hitbox_group;
        int32_t hitbox_disable_timer;
};

struct HitboxList{
    Hitbox* hitbox;
    struct HitboxList* next;
};
#endif