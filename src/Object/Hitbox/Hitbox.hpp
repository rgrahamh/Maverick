#ifndef HITBOX_H
#define HITBOX_H

class Hitbox{
    public:
        Hitbox();
        ~Hitbox();

        bool isActive();
        float getXOffset();
        float getYOffset();
        float getHeight();
        float getWidgth();
        
    private:
        bool active;
        float x_offset;
        float y_offset;
        float height;
        float width;
};

#endif