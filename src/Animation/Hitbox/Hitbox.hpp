#ifndef HITBOX_H
#define HITBOX_H

//The 
enum HITBOX_SHAPE{
    RECT,
    ELLIPSE,
    CONE
};

enum HITBOX_TYPE{
    ENVIRONMENT = 1,  //Environmental element (can skip check against other environmental collisions)
    COLLISION = 2,    //A more general collision hitbox
    DAMAGEBOX = 4,    //A damaging hitbox
    HURTBOX = 8,      //A hitbox that you can get damaged
    PROJECTILE = 16   //A projectile hitbox
};

class Hitbox{
    public:
        Hitbox(float* x_base, float* y_base, float x_offset, float y_offset, unsigned int type);
        virtual ~Hitbox();

        float getX();
        float getY();

        virtual float getRightBound();
        virtual float getLeftBound();
        virtual float getTopBound();
        virtual float getBotBound();

        HITBOX_SHAPE getShape();
        unsigned int getType();

        virtual void setScale(float x_scale, float y_scale);

        virtual bool isPointInside(float x_coord, float y_coord);
        virtual bool checkCollision(Hitbox* other);
        
    protected:
		//Pointers to the X and Y base coords
		float* x_base;
		float* y_base;

        float x_base_offset;
        float y_base_offset;

        float x_curr_offset;
        float y_curr_offset;

        HITBOX_SHAPE shape;
        unsigned int type;
};

typedef struct HitboxList{
    Hitbox* hitbox;
    struct HitboxList* next;
} HitboxLst;
#endif