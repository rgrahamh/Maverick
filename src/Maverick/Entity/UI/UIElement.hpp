#ifndef UI_H
#define UI_H

#include "../Entity.hpp"

class UIElement;

//UI elements are in the positives
enum UI_ELEMENT_TYPE{
    GENERIC_ELEMENT = 0x0,
    WINDOW,
    TEXT,
    TEXT_BOX,
    BORDERS,
    SELECTOR,
    BOX,
    SCREEN_BLIT,
    OBJECT_FRAME,
    RADIAL,
    EXTENDED_UI_ELEMENT_TYPE_START = 0x8000,
    UI_ELEMENT_END = 0x0FFFFFFF
};

struct UIElementList{
    UIElement* element;
    struct UIElementList* next;
};

class UIElement : public Entity{
    public:
        UIElement(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height, int draw_layer = 0);
        virtual ~UIElement();

		virtual void _process(uint64_t delta, unsigned int steps) override;
		virtual void process(uint64_t delta, unsigned int steps) override;
		virtual void _action(Control* control) override;
		virtual void action(Control* control) override;
        virtual void _draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y) override;
        virtual void draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y) override;

		virtual int serializeExtendedAssets(FILE* file, SerializeSet& serialize_set);
		virtual int serializeExtendedData(FILE* file, Zone* base_zone);

        virtual void setViewSize(double view_width, double view_height);
		virtual void setActive(bool active);
		virtual void setVisible(bool visible);

        void addElement(UIElement* element);
        int addSprite(const char* animation_name, const char* sprite_set, const char* sprite_path, double x_offset = 0.0, double y_offset = 0.0, float width = -1.0, float height = -1.0);

        UIElement* getElement(const char* name);
        UIElementList* getSubelements();

        float getWidth() override;
        float getHeight() override;
        double getViewWidth();
        double getViewHeight();

    protected:
        //All elements which are children of this one
        UIElementList* subelements;
        uint16_t num_subelements;

        //The UI offsets/sizing
        double view_x_offset;
        double view_y_offset;
        double view_width;
        double view_height;

        //The UI draw area
        SDL_Rect draw_area;

        //Checks if the mouse is inside of the draw area
        bool isMouseInside(Control* control);
};


#endif