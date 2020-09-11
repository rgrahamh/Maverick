#ifndef HITBOX_H
#define HITBOX_H

enum HITBOX_TYPE{
    RECT,
    CIRCLE,
    CONE
};

class Hitbox{
    public:
        Hitbox(float x_offset, float y_offset);
        ~Hitbox();

        float getX();
        float getY();

        virtual void setScale(float x_scale, float y_scale);
        
    protected:
        float base_x_offset;
        float base_y_offset;

        float curr_x_offset;
        float curr_y_offset;

        HITBOX_TYPE type;
};

typedef struct HitboxList{
    Hitbox* hitbox;
    struct HitboxList* next;
} HitboxLst;

#endif