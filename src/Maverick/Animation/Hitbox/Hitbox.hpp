#ifndef HITBOX_H
#define HITBOX_H

#include <inttypes.h>
#include <stdio.h>

#include "../../Global/Global.hpp"

//The shape of the hitbox
enum HITBOX_SHAPE{
    HIT_RECT = 0,
    HIT_ELLIPSE,
    HIT_CONE
};

//The type of the hitbox (default flags that may be applied to a hitbox)
enum HITBOX_TYPE{
    ENVIRONMENT =     1, //Environmental element (can skip check against other environmental collisions)
    COLLISION =       2, //A more general collision hitbox
    MOVABLE =         4, //Bumping into this collision hitbox will cause the object to move
    DAMAGEBOX =       8, //A damaging hitbox
    HURTBOX =        16, //A hitbox that you can get damaged
    GROUNDING_ZONE = 32, //A hitbox that checks what the ground position should be (generally to the bottom of this hitbox)
    TRIGGER   = 64  //A hitbox that signifies that something should be loaded
};

class Hitbox{
    public:
        Hitbox(double* x_base, double* y_base, double* z_base, double x_offset, double y_offset, double z_offset, double depth, uint64_t type, int32_t hitbox_group = -1, int32_t immunity_timer = 0);
        virtual ~Hitbox();

        double getX();
        double getY();
        double getZ();

        double getZMax();
        double getZMin();

        double getXOffset();
        double getYOffset();
        double getZOffset();

        void setXOffset(double x_offset);
        void setYOffset(double y_offset);
        void setZOffset(double z_offset);

        double getDepth();

        virtual float getRightBound();
        virtual float getLeftBound();
        virtual float getTopBound();
        virtual float getBotBound();

        HITBOX_SHAPE getShape();
        uint64_t getType();

        int32_t getHitboxGroup();
        int32_t getImmunityTimer();

        virtual bool isPointInside(double x_coord, double y_coord);
        virtual bool checkCollision(Hitbox* other);

        virtual void serializeData(FILE* file) = 0;
        
    protected:
		//Pointers to the X and Y base coords
		double* x_base;
		double* y_base;
        double* z_base;

        double x_offset;
        double y_offset;
        double z_offset;

        double depth;

        HITBOX_SHAPE shape;
        uint64_t type;

        int32_t hitbox_group;
        int32_t immunity_timer;
};

struct HitboxList{
    Hitbox* hitbox;
    struct HitboxList* next;
};

class Object;
struct HitboxImmunityList{
    Object* obj;
    Hitbox* hitbox;
    uint32_t hitbox_group;
    uint32_t ignore_timer;
    struct HitboxImmunityList* next;
};
#endif