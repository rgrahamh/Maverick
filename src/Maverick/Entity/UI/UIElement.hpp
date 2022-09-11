#ifndef UI_H
#define UI_H

#include "../Entity.hpp"

#include <vector>

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
        /** UIElement constructor (for viewport calcs, 1.0 is one screen width/height)
         * @param name The name of the UIElement
         * @param view_x_offset The viewport X offset of the UIElement
         * @param view_y_offset The viewport X offset of the UIElement
         * @param view_width The viewport width of the UIElement
         * @param view_height The viewport height of the UIElement
         * @param animation_num The animation number of the UIElement (use for multiple would be blinking cursors)
         * @param draw_layer The draw layer of the UIElement (all child elements will be drawn directly on top)
         */
        UIElement(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height, int draw_layer = 0);

        /** Default UIElement destructor
         */
        virtual ~UIElement();

        /** Called every frame for processing
         * @param delta The amount of time that has passed (in ms)
         * @param steps The number of physics steps that have passed since last processing
         */
		virtual void _process(uint64_t delta, unsigned int steps) override;

        /** Called every frame for processing; can be overridden by children
         * @param delta The amount of time that has passed (in ms)
         * @param steps The number of physics steps that have passed
         */
		virtual void process(uint64_t delta, unsigned int steps) override;

        /** Handles actions for this UIElement and all children UIElements
         * @param control The Control class that represents user input
         */
		virtual void _action(Control* control) override;

        /** Handles actions for this UIElement
         * @param control The control struct
         */
		virtual void action(Control* control) override;

        /** Draws this UIElement and all children UIElements
         * @param renderer The SDL_Renderer we're drawing to
         * @param delta The time passed since last draw (in ms)
         * @param camera_x The X location of the camera
         * @param camera_y The Y location of the camera
         */
        virtual void _draw(SDL_Renderer* renderer, uint64_t delta, int x_offset, int y_offset) override;

        /** Draws this UIElement
         * @param renderer The SDL_Renderer we're drawing to
         * @param delta The time passed since last draw (in ms)
         * @param camera_x The X offset of the draw area
         * @param camera_y The Y offset of the draw area
         */
        virtual void draw(SDL_Renderer* renderer, uint64_t delta, int x_offset, int y_offset) override;

        /** Serializing UI Elements (WIP)
         * @param file An open file to write to
         * @param base_zone The zone this object belongs to (used for zone-based offsets)
         * @return 0 on success
         */
        virtual int serializeData(FILE* file, Zone* base_zone) override;

        /** Serializing UI Element assets (to be overridden by children, as necessary)
         * @param file An open file to write to
         * @param serialize_set The serialization set (logs saved assets)
         * @return -1 if serialization failed, 0 otherwise
         */
        virtual int serializeAssets(FILE* file, SerializeSet& serialize_set) override;

        /** Sets the visibility of the current animation state
         * @return If the object is active
         */
		virtual void setActive(bool active) override;

        /** Sets the visibility of the current animation state
         * @return If the object is active
         */
		virtual void setVisible(bool visible) override;

        /** Sets the scale of the UIElement
         * @param view_width The viewport width (0.0 is none of the screen, 1.0 is the whole thing)
         * @param view_height The viewport height (0.0 is none of the screen, 1.0 is the whole thing)
         */
        void setViewSize(double view_width, double view_height);

        /** Sets the UI offset of the UIElement
         * @param view_x_offset The new X viewport offset
         * @param view_y_offset The new Y viewport offset
         */
        void setViewOffset(double x_offset, double y_offset);

        /** Sets the background animation of the UIElement
         * @param animation A pointer to the new background animation
         */
        void setBackground(Animation* background_ptr);

        /** Adds an element to the child element list
         * @param element The element to add
         * @note Virtual so that the UIElement can do something custom with the "add a new subelement" logic
         */
        virtual void addSubelement(UIElement* element);

        /** Tries to get an element of the given name
         * @param name The name of the element
         * @return A pointer to the element with a matching name, or NULL if not found
         */
        UIElement* getSubelement(const char* name);
        
        /** Deletes a given subelement
         * @param name The name of the subelement you wish to delete
         */
        void deleteSubelement(const char* name);

        /** Gets the subelements
         * @return The subelement list
         */
        std::vector<UIElement*>& getSubelements();

        /** Gets the width of the UI element (in pixels)
         * @return The width of the UI element (in pixels)
         */
        double getWidth() override;

        /** Gets the pixel height of the UI element
         * @return The pixel height of the UI element
         */
        double getHeight() override;

        /** Gets the view width of the UI element
         * @return The view width of the UI element
         */
        double getViewWidth();

        /** Gets the view height of the UI element
         * @return The view height of the UI element
         */
        double getViewHeight();

    protected:
        //All elements which are children of this one
        std::vector<UIElement*> subelements;

        //The UI offsets/sizing
        double view_x_offset;
        double view_y_offset;
        double view_width;
        double view_height;

        Animation* background;

        //The UI pixel width/height (used for drawing & action calculations)
        double width;
        double height;

        /** Returns true if the mouse is inside the draw area, and false otherwise
         * @return If the mouse is within the draw area
         */
        virtual bool isMouseInside(Control* control);

        /** Updates the draw area to be current
         */
        void updateDrawArea();
};


#endif