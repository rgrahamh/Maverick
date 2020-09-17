#ifndef HITBOX_H
#define HITBOX_H

enum HITBOX_TYPE{
    RECT,
    ELLIPSE,
    CONE
};

class Hitbox{
    public:
        Hitbox(float* x_base, float* y_base, float x_offset, float y_offset);
        ~Hitbox();

        float getX();
        float getY();
        HITBOX_TYPE getType();

        virtual bool isPointInside(float x_coord, float y_coord);

        virtual void setScale(float x_scale, float y_scale);
        
    protected:
		//Pointers to the X and Y base coords
		float* x_base;
		float* y_base;

        float x_base_offset;
        float y_base_offset;

        float x_curr_offset;
        float y_curr_offset;

        HITBOX_TYPE type;
};

typedef struct HitboxList{
    Hitbox* hitbox;
    struct HitboxList* next;
} HitboxLst;
#endif