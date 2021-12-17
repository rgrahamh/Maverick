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
UIElement::UIElement(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height,
                     unsigned int animation_num, int draw_layer, SDL_Window* window)
        :  Entity(name, 0, 0, animation_num, draw_layer){
    //Setting viewport position/scaling stats
    this->view_x_offset = view_x_offset;
    this->view_y_offset = view_y_offset;
    this->view_width = view_width;
    this->view_height = view_height;
    this->window = window;

    int win_width, win_height;
    SDL_GetWindowSize(this->window, &win_width, &win_height);

    this->x = view_x_offset * win_width;
    this->y = view_y_offset * win_height;

    this->draw_area.x = this->x;
    this->draw_area.y = this->y;
    this->draw_area.w = view_width * win_width;
    this->draw_area.h = view_height * win_height;

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

void UIElement::process(uint32_t delta){
    return;
}

void UIElement::_process(uint32_t delta){
    this->process(delta);

    UIElementList* cursor = subelements;
    while(cursor != nullptr){
        if(cursor->element->isActive()){
            cursor->element->_process(delta);
        }
        cursor = cursor->next;
    }
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
void UIElement::_draw(SDL_Renderer* renderer, uint32_t delta, int camera_x, int camera_y){
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
void UIElement::draw(SDL_Renderer* renderer, uint32_t delta, int camera_x, int camera_y){
    //Draw this element
    if(active_animation < this->total_animation_num){
        this->animations[active_animation]->draw(renderer, delta, camera_x, camera_y);
    }
}

/** Sets the scale of the UIElement
 * @param x_scale The X scale
 * @param y_scale The Y scale
 */
void UIElement::setScale(float x_scale, float y_scale){
    //Set scale for this element
    for(unsigned int i = 0; i < this->total_animation_num; i++){
        this->animations[i]->setScale(x_scale, y_scale);
    }

    //Set scale for all children elements
    UIElementList* cursor = this->subelements;
    while(cursor != nullptr){
        for(unsigned int i = 0; i < this->total_animation_num; i++){
            this->animations[i]->setScale(x_scale, y_scale);
        }
        cursor = cursor->next;
    }
}

/** Sets the scale of the UIElement
 * @param width The width
 * @param height The height
 */
void UIElement::setViewSize(double view_width, double view_height){
    int win_width, win_height;
    SDL_GetWindowSize(window, &win_width, &win_height);
    //Set scale for this element
    for(unsigned int i = 0; i < this->total_animation_num; i++){
        this->animations[i]->setSize(view_width * win_width, view_height * win_height);
    }

    //Set scale for all children elements
    UIElementList* cursor = this->subelements;
    while(cursor != nullptr){
        for(unsigned int i = 0; i < this->total_animation_num; i++){
            this->animations[i]->setSize(view_width * win_width, view_height * win_height);
        }
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
 * @param animation_num The animation number
 * @param sprite_path The filepath to the sprite you're adding
 * @param keytime The number of frames until the animation progresses
 * @param x_offset The X offset of the sprite
 * @param y_offset The Y offset of the sprite
 * @param width The view width of the sprite (scales to element if -1)
 * @param height The view height of the sprite (scales to element if -1)
 */ 
void UIElement::addSprite(unsigned int animation_num, const char* sprite_path, unsigned int keytime, float x_offset, float y_offset, float width, float height){
    if(animation_num < this->total_animation_num){
        int win_width, win_height;
        SDL_GetWindowSize(window, &win_width, &win_height);

        if(width != -1){
            width *= (float)win_width;
        }
        else{
            width = this->draw_area.w;
        }
        if(height != -1){
            height *= (float)win_height;
        }
        else{
            height = this->draw_area.h;
        }
        this->animations[animation_num]->addFrame(sprite_path, keytime, x_offset, y_offset, width, height);
    }
}

/** Adds an element to the child element list
 * @param element The element to add
 */
void UIElement::addElement(UIElement* element){
    UIElementList* new_element = new UIElementList;
    new_element->element = element;
    new_element->next = subelements;
    subelements = new_element;
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