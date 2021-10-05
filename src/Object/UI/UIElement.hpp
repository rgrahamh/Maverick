#ifndef UI_H
#define UI_H

#include "../Object.hpp"

enum UI_OBJECT_TYPE{
    GENERIC,
    ELEMENT_GROUP,
    WINDOW,
    TEXT,
    SELECTOR,
    BOX,
    RADIAL
};

class UIElement;

typedef struct UIElementList{
    UIElement* element;
    struct UIElementList* next;
} UIElementLst;

class UIElement : public Object{
    public:
        UIElement(const char* name, float view_x_offest, float view_y_offset, float view_width, float view_height, unsigned int animation_num, int draw_layer, enum UI_OBJECT_TYPE obj_type, SDL_Window* window);
        virtual ~UIElement();

		virtual void action(SDL_Event* event);
        virtual void draw(SDL_Renderer* renderer, uint32_t delta, int camera_x, int camera_y);
		virtual void setScale(float x_scale, float y_scale);

        void addElement(UIElement* element);
        void addSprite(unsigned int animation_num, const char* sprite_path, unsigned int keytime, float x_offset, float y_offset, bool isRelative = false);
        UIElement* getElement(const char* name);
        UIElementLst* getSubelements();

    protected:
        //The UI object type
        enum UI_OBJECT_TYPE type;
        //All elements which are children of this one
        UIElementLst* subelements;

        //The window (to calculate sizes)
        SDL_Window* window;

        //The UI offsets/sizing
        float view_x_offset;
        float view_y_offset;
        float view_width;
        float view_height;

        //The UI draw area
        SDL_Rect draw_area;
};


#endif