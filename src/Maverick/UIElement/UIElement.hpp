#ifndef UIELEMENT_H
#define UIELEMENT_H

#include "Maverick/Animation/Animation.hpp"
#include "Maverick/Animation/Hitbox/Hitbox.hpp"
#include "Maverick/Animation/Hitbox/HitRect/HitRect.hpp"
#include "Maverick/Animation/Hitbox/HitEllipse/HitEllipse.hpp"
#include "Maverick/Animation/Hitbox/HitEllipse/HitCone/HitCone.hpp"
#include "Maverick/Animation/Hitbox/HitboxCollision.hpp"
#include "Maverick/Control/Control.hpp"

#include <SDL2/SDL.h>
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

class Zone;
class UIElement{
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

		/** Construct a UIElement from file
		 * @param file The file you wish to construct the entity with, pointing to the object type indicator
		 */
		UIElement(FILE* file);

        /** Default UIElement destructor
         */
        virtual ~UIElement();

        /** Called every frame for processing
         * @param delta The amount of time that has passed (in ms)
         * @param steps The number of physics steps that have passed since last processing
         */
		virtual void _process(uint64_t delta, unsigned int steps);

        /** Called every frame for processing; can be overridden by children
         * @param delta The amount of time that has passed (in ms)
         * @param steps The number of physics steps that have passed
         */
		virtual void process(uint64_t delta, unsigned int steps);

        /** Handles actions for this UIElement and all children UIElements
         * @param control The Control class that represents user input
         */
		virtual void _action(Control* control);

        /** Handles actions for this UIElement
         * @param control The control struct
         */
		virtual void action(Control* control);

        /** Draws this UIElement and all subelements
         * @param delta The time passed since last draw (in ms)
         * @param draw_scope The area that the UIElement is allowed to draw in
         */
        virtual void _draw(uint64_t delta, const SDL_Rect& draw_scope);

        /** Draws this UIElement
         * @param delta The time passed since last draw (in ms)
         * @param draw_scope The area that the UIElement is allowed to draw in
         */
        virtual void draw(uint64_t delta, const SDL_Rect& draw_scope);

        /** Serializing UI Elements (WIP)
         * @param file An open file to write to
         * @param base_zone The zone this object belongs to (used for zone-based offsets)
         * @return 0 on success
         */
        virtual int serializeData(FILE* file, Zone* base_zone);

        /** Serializing UI Element assets (to be overridden by children, as necessary)
         * @param file An open file to write to
         * @param serialize_set The serialization set (logs saved assets)
         * @return -1 if serialization failed, 0 otherwise
         */
        virtual int serializeAssets(FILE* file, SerializeSet& serialize_set);

		/** Deserialized data from a file
		 * @param file The file to read info from
		 * @return -1 if serialization failed, 0 otherwise
		 */
		virtual int deserializeData(FILE* file);

        /** Sets the view width of the UIElement
         * @param view_width The viewport width (0.0 is none of the screen, 1.0 is the whole thing)
         */
        void setViewWidth(double view_width);

        /** Sets the view height of the UIElement
         * @param view_height The viewport height (0.0 is none of the screen, 1.0 is the whole thing)
         */
        void setViewHeight(double view_height);

        /** Sets the X UI offset of the UIElement
         * @param view_x_offset The new X viewport offset
         */
        void setViewXOffset(double x_offset);

        /** Sets the Y UI offset of the UIElement
         * @param view_y_offset The new Y viewport offset
         */
        void setViewYOffset(double y_offset);

        /** Sets the background animation of the UIElement
         * @param animation A pointer to the new background animation
         */
        void setBackground(Animation* background_ptr);

        /** Deletes the background animation of the UIElement
         */
        void deleteBackground();

        /** Sets the visibility of the current animation state
         * @return If the object is active
         */
		void setActive(bool active);

        /** Sets the visibility of the current animation state
         * @return If the object is active
         */
		void setVisible(bool visible);

        /** Sets the vertical buffer of the UI element
         * @param vertical_buffer The height buffer of the UI element applied on both top & bottom
         */
        void setHeightBuffer(double height_buff);

        /** Sets the horizontal buffer of the UI element
         * @param width_buff The horizontal buffer of the UI element applied on both top & bottom (as a view; 0.0 is no buffer, 0.1 is a tenth of the screen)
         */
        void setWidthBuffer(double horizontal_buffer);

		/** Gets if the current entity is active (can be interacted with)
		 * @return If the entity is active
		 */
		bool isActive();

		/** Gets if the current entity is visible (will/won't be drawn)
		 * @return If the entity is active
		 */
		bool isVisible();

		/** Gets the UIElement type
		 * @return A uint32_t representation of the UIElement type set in the constructor
		 */
		uint32_t getType();

		/** Gets the name of the entity
		 * @return The name of the entity
		 */
		const char* getName();

		/** Gets the draw layer of the entity
		 * @return The draw layer of the entity
		 */
		int16_t getDrawLayer();

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

        /** Draws all subelements (may be overridden for child behavior)
         * @param delta The amount of time that has passed (in ms) since the last draw operation
         * @param draw_scope The allowed draw area for the UIElement
         */
        virtual void drawSubelements(uint64_t delta, const SDL_Rect& draw_scope);

        /** Gets the width of the UI element (in pixels)
         * @return The width of the UI element (in pixels)
         */
        double getWidth();

        /** Gets the pixel height of the UI element
         * @return The pixel height of the UI element
         */
        double getHeight();

        /** Gets the view width of the UI element
         * @return The view width of the UI element
         */
        double getViewWidth();

        /** Gets the view height of the UI element
         * @return The view height of the UI element
         */
        double getViewHeight();

    protected:
		//UIElement name
		char* name;

        //All elements which are children of this one
        std::vector<UIElement*> subelements;

        //The UI offsets/sizing
        double view_x_offset;
        double view_y_offset;
        double view_width;
        double view_height;

        //Element buffers (applied to THIS element, not children)
        double width_buff;
        double height_buff;

        //The draw area (used for drawing & action calculations; recalculated every draw step)
        SDL_Rect draw_area;

		//Draw layer tracking
		int16_t draw_layer;

		//The entity type (exact values are specified by child classes)
		uint32_t type;

		//If the object is active/visible
		bool active;
		bool visible;

        //The background anmiation (takes up the entire draw space by default)
        Animation* background;

        /** Returns true if the mouse is inside the draw area, and false otherwise
         * @return If the mouse is within the draw area
         */
        virtual bool isMouseInside(Control* control);
};

#endif