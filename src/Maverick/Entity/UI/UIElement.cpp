#include "UIElement.hpp"
#include "../../Engine/Engine.hpp"

extern Engine* engine;

/** UIElement constructor (for viewport calcs, 1.0 is one screen width/height)
 * @param name The name of the UIElement
 * @param view_x_offset The viewport X offset of the UIElement
 * @param view_y_offset The viewport X offset of the UIElement
 * @param view_width The viewport width of the UIElement
 * @param view_height The viewport height of the UIElement
 * @param animation_num The animation number of the UIElement (use for multiple would be blinking cursors)
 * @param draw_layer The draw layer of the UIElement (all child elements will be drawn directly on top)
 * @param window The current window (used for viewport calculation)
 */

//DETERMINE USAGE OF TYPE!!!!!!!!!!!!!!!!


UIElement::UIElement(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height, SDL_Window* window, int draw_layer)
         : Entity(name, 0, 0, draw_layer){
    //Setting viewport position/scaling stats
    this->view_x_offset = view_x_offset;
    this->view_y_offset = view_y_offset;
    this->view_width = view_width;
    this->view_height = view_height;
    this->window = window;

    this->type = UI_ELEMENT_TYPE::GENERIC_ELEMENT;

    this->x = view_x_offset * SCREEN_WIDTH;
    this->y = view_y_offset * SCREEN_HEIGHT;

    this->draw_area.x = this->x;
    this->draw_area.y = this->y;
    this->draw_area.w = view_width * SCREEN_WIDTH;
    this->draw_area.h = view_height * SCREEN_HEIGHT;

    this->subelements = nullptr;
}

/** Default UIElement destructor
 */
UIElement::~UIElement(){
    /*UIElementList* cursor = subelements;
    while(cursor != nullptr){
        UIElementList* tmp = cursor;
        delete cursor->element;

        cursor = cursor->next;
        delete tmp;
    }*/
}

void UIElement::process(uint64_t delta, double step_size){
    return;
}

void UIElement::_process(uint64_t delta, double step_size){
    this->process(delta, step_size);

    UIElementList* cursor = subelements;
    while(cursor != nullptr){
        if(cursor->element->isActive()){
            cursor->element->_process(delta, step_size);
        }
        cursor = cursor->next;
    }

    cleanupHitboxImmunity(delta);
}

/** Handles actions for this UIElement
 * @param event The event that has occurred
 */
void UIElement::action(Control* control){
    return;
}

/** Handles actions for this UIElement and all children UIElements
 * @param event The event that has occurred
 */
void UIElement::_action(Control* control){
    this->action(control);

    //Perform all actions for child elements (generic element has no default actions)
    UIElementList* cursor = this->subelements;
    while(cursor != nullptr){
        if(cursor->element->isActive()){
            cursor->element->_action(control);
        }
        cursor = cursor->next;
    }
}

/** Draws this UIElement and all children UIElements
 * @param renderer The SDL_Renderer we're drawing to
 * @param delta The time passed since last draw (in ms)
 * @param camera_x The X location of the camera
 * @param camera_y The Y location of the camera
 */
void UIElement::_draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y){
    //Draw this element
    this->draw(renderer, delta, camera_x, camera_y);

    //Draw all children elements (AFTER the parent element)
    UIElementList* cursor = this->subelements;
    while(cursor != nullptr && cursor->element != nullptr){
        cursor->element->_draw(renderer, delta, camera_x, camera_y);
        cursor = cursor->next;
    }
}

/** Draws this UIElement
 * @param renderer The SDL_Renderer we're drawing to
 * @param delta The time passed since last draw (in ms)
 * @param camera_x The X location of the camera
 * @param camera_y The Y location of the camera
 */
void UIElement::draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y){
    //Draw this element
    if(active_animation != nullptr){
        this->active_animation->draw(renderer, delta, camera_x, camera_y);
    }
}

/** Sets the scale of the UIElement
 * @param width The width
 * @param height The height
 */
void UIElement::setViewSize(double view_width, double view_height){
    //Set scale for this element
    AnimationList* animation_cursor = animations;
    while(animation_cursor != nullptr){
        animation_cursor->animation->setSize(view_width * SCREEN_WIDTH, view_height * SCREEN_HEIGHT);
    }

    //Set scale for all children elements
    UIElementList* cursor = this->subelements;
    while(cursor != nullptr){
        cursor->element->setViewSize(view_width, view_height);
        cursor = cursor->next;
    }
}

/** Sets the visibility of the current animation state
 * @return If the object is active
 */
void UIElement::setActive(bool active){
    this->active = active;

    //Set visible for all child elements
    UIElementList* cursor = subelements;
    while(cursor != nullptr){
        cursor->element->setActive(active);
        cursor = cursor->next;
    }
}

/** Sets the visibility of the current animation state
 * @return If the object is active
 */
void UIElement::setVisible(bool visible){
    this->visible = visible;

    //Set active for all child elements
    UIElementList* cursor = subelements;
    while(cursor != nullptr){
        cursor->element->setVisible(visible);
        cursor = cursor->next;
    }
}

/** Adds a sprite to a given animation
 * @param animation_name The animation name
 * @param sprite_path The filepath to the sprite you're adding
 * @param keytime The number of frames until the animation progresses
 * @param x_offset The X offset of the sprite
 * @param y_offset The Y offset of the sprite
 * @param width The view width of the sprite (scales to element if -1)
 * @param height The view height of the sprite (scales to element if -1)
 */ 
int UIElement::addSprite(const char* animation_name, const char* sprite_set, const char* sprite_path, double x_offset, double y_offset, float width, float height){
    Animation* animation = findAnimation(animation_name);
    if(animation == nullptr){
        return -1;
    }

    if(width != -1){
        width *= (float)SCREEN_WIDTH;
    }
    else{
        width = this->draw_area.w;
    }
    if(height != -1){
        height *= (float)SCREEN_HEIGHT;
    }
    else{
        height = this->draw_area.h;
    }
    return animation->addSprite(sprite_set, sprite_path, x_offset, y_offset, width, height);
}

/** Adds an element to the child element list
 * @param element The element to add
 */
void UIElement::addElement(UIElement* element){
    UIElementList* new_element = new UIElementList;
    new_element->element = element;
    new_element->next = subelements;
    subelements = new_element;
    num_subelements++;
}

/** Tries to get an element of the given name
 * @param name The name of the element
 * @return A pointer to the element with a matching name, or NULL if not found
 */
UIElement* UIElement::getElement(const char* name){
    //Check self
    if(strcmp(this->name, name) == 0){
        return this;
    }

    //Check children
    UIElementList* cursor = this->subelements;
    while(cursor != nullptr){
        UIElement* element = cursor->element->getElement(name);
        if(element != nullptr){
            return element;
        }
        cursor = cursor->next;
    }

    return nullptr;
}

/** Gets the subelements
 * @return The subelement list
 */
UIElementList* UIElement::getSubelements(){
    return this->subelements;
}

/** Returns true if the mouse is inside the draw area, and false otherwise
 * @return If the mouse is within the draw area
 */
bool UIElement::isMouseInside(Control* control){
    const MouseState* mouse = control->getMouse();
    if(mouse->x > this->draw_area.x && mouse->x < (this->draw_area.x + this->draw_area.w) &&
       mouse->y > this->draw_area.y && mouse->y < (this->draw_area.y + this->draw_area.h)){
        return true;
    }

    return false;
}

int UIElement::serializeExtendedData(FILE* file, Zone* base_zone){
    //Get X/Y offsets
    uint64_t view_x_offset_swap = EndianSwap((uint64_t*)&view_x_offset);
    uint64_t view_y_offset_swap = EndianSwap((uint64_t*)&view_y_offset);

    //Get width/height
    uint64_t view_width_swap = EndianSwap((uint64_t*)&view_width);
    uint64_t view_height_swap = EndianSwap((uint64_t*)&view_height);

    fwrite(&view_x_offset_swap, sizeof(view_x_offset_swap), 1, file);
    fwrite(&view_y_offset_swap, sizeof(view_x_offset_swap), 1, file);

    fwrite(&view_width_swap, sizeof(view_width_swap), 1, file);
    fwrite(&view_height_swap, sizeof(view_height_swap), 1, file);

    UIElementList* subelement_cursor = subelements;
    while(subelement_cursor != nullptr){
        subelement_cursor->element->serializeData(file, base_zone);
        subelement_cursor = subelement_cursor->next;
    }

    return 0;
}

int UIElement::serializeExtendedAssets(FILE* file, std::unordered_set<std::string>& sprite_set, std::unordered_set<std::string>& audio_set, std::unordered_set<std::string>& music_set){
    UIElementList* subelement_cursor = subelements;
    while(subelement_cursor != nullptr){
        subelement_cursor->element->serializeAssets(file, sprite_set, audio_set, music_set);
        subelement_cursor = subelement_cursor->next;
    }
    return 0;
}