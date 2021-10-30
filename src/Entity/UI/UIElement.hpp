#ifndef UI_H
#define UI_H

#include "../Entity.hpp"

enum UI_OBJECT_TYPE{
    GENERIC,
    ELEMENT_GROUP,
    WINDOW,
    TEXT,
    TEXT_BOX,
    SELECTOR,
    BOX,
    RADIAL
};

class UIElement;

struct UIElementList{
    UIElement* element;
    struct UIElementList* next;
};

class UIElement : public Entity{
    public:
        UIElement(const char* name, float view_x_offest, float view_y_offset, float view_width, float view_height, unsigned int animation_num, int draw_layer, enum UI_OBJECT_TYPE obj_type, SDL_Window* window);
        virtual ~UIElement();

		virtual void _process(uint32_t delta);
		virtual void process(uint32_t delta);
		virtual void _action(Control* control);
		virtual void action(Control* control);
        virtual void _draw(SDL_Renderer* renderer, uint32_t delta, int camera_x, int camera_y);
        virtual void draw(SDL_Renderer* renderer, uint32_t delta, int camera_x, int camera_y);

		virtual void setScale(float x_scale, float y_scale);
        virtual void setViewSize(float view_width, float view_height);
		virtual void setActive(bool active);
		virtual void setVisible(bool visible);

        void addElement(UIElement* element);
        void addSprite(unsigned int animation_num, const char* sprite_path, unsigned int keytime, float x_offset, float y_offset, float width = -1.0, float height = -1.0);

        UIElement* getElement(const char* name);
        UIElementList* getSubelements();

    protected:
        //The UI object type
        enum UI_OBJECT_TYPE type;
        //All elements which are children of this one
        UIElementList* subelements;

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