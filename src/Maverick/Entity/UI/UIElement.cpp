#include "UIElement.hpp"
#include "../../Engine/Engine.hpp"

/** UIElement constructor (for viewport calcs, 1.0 is one screen width/height)
 * @param name The name of the UIElement
 * @param view_x_offset The viewport X offset of the UIElement
 * @param view_y_offset The viewport X offset of the UIElement
 * @param view_width The viewport width of the UIElement
 * @param view_height The viewport height of the UIElement
 * @param animation_num The animation number of the UIElement (use for multiple would be blinking cursors)
 * @param draw_layer The draw layer of the UIElement (all child elements will be drawn directly on top)
 */

UIElement::UIElement(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height, int draw_layer)
         : Entity(name, 0, 0, draw_layer){
    //Setting viewport position/scaling stats
    this->view_x_offset = view_x_offset;
    this->view_y_offset = view_y_offset;
    this->view_width = view_width;
    this->view_height = view_height;

    this->type = UI_ELEMENT_TYPE::GENERIC_ELEMENT;

    int win_width, win_height;
    SDL_GetWindowSize(Engine::getInstance()->getWindow(), &win_width, &win_height);

    this->x = view_x_offset * win_width;
    this->y = view_y_offset * win_height;

    this->draw_area.x = this->x;
    this->draw_area.y = this->y;
    this->draw_area.w = view_width * win_width;
    this->draw_area.h = view_height * win_height;
}

/** Default UIElement destructor
 */
UIElement::~UIElement(){
    for(UIElement* subelement : subelements){
        delete subelement;
    }
}

/** Called every frame for processing; can be overridden by children
 * @param delta The amount of time that has passed (in ms)
 * @param step_size The step size used for physics calcluation (probably not needed here)
 */
void UIElement::process(uint64_t delta, unsigned int steps){
    return;
}

/** Called every frame for processing
 * @param delta The amount of time that has passed (in ms)
 * @param step_size The step size used for physics calcluation (probably not needed here)
 */
void UIElement::_process(uint64_t delta, unsigned int steps){
    this->process(delta, steps);

    for(UIElement* subelement : subelements){
        if(subelement->isActive()){
            subelement->_process(delta, steps);
        }
    }

    cleanupHitboxImmunity(delta);
}

/** Handles actions for this UIElement
 * @param control The control struct
 */
void UIElement::action(Control* control){
    return;
}

/** Handles actions for this UIElement and all children UIElements
 * @param control The Control class that represents user input
 */
void UIElement::_action(Control* control){
    this->action(control);

    //Perform all actions for child elements (generic element has no default actions)
    for(UIElement* subelement : subelements){
        if(subelement->isActive()){
            subelement->_action(control);
        }
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
    int win_width, win_height;
    SDL_GetRendererOutputSize(Engine::getInstance()->getCamera()->getRenderer(), &win_width, &win_height);
    for(UIElement* subelement : subelements){
        subelement->_draw(renderer, delta, camera_x, camera_y);
    }
}

/** Updates the draw area to be current
 */
inline void UIElement::updateDrawArea(){
    int win_width, win_height;
    SDL_GetRendererOutputSize(Engine::getInstance()->getCamera()->getRenderer(), &win_width, &win_height);
    this->draw_area.x = this->x + this->view_x_offset * win_width;
    this->draw_area.y = this->y + this->view_y_offset * win_height;

    this->draw_area.w = this->view_width * win_width;
    this->draw_area.h = this->view_height * win_height;
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
        this->active_animation->draw(renderer, delta, camera_x * -1, camera_y * -1);
    }
}

/** Sets the scale of the UIElement
 * @param view_width The viewport width (0.0 is none of the screen, 1.0 is the whole thing)
 * @param view_height The viewport height (0.0 is none of the screen, 1.0 is the whole thing)
 */
void UIElement::setViewSize(double view_width, double view_height){
    int win_width, win_height;
    SDL_GetWindowSize(Engine::getInstance()->getWindow(), &win_width, &win_height);
    double old_view_width = this->view_width;
    double old_view_height = this->view_height;
    this->view_width = view_width;
    this->view_height = view_height;

    double view_width_diff = view_width - old_view_width;
    double view_height_diff = view_height - old_view_height;

    //Set scale for this element
    AnimationList* animation_cursor = animations;
    while(animation_cursor != nullptr){
        animation_cursor->animation->setSize(animation_cursor->animation->getSprite()->rect->w * (1.0 + view_width_diff), 
                                             animation_cursor->animation->getSprite()->rect->h * (1.0 + view_height_diff));
        animation_cursor = animation_cursor->next;
    }

    //Set scale for all children elements
    for(UIElement* subelement : subelements){
        subelement->setViewSize(subelement->getViewWidth() + view_width_diff, subelement->getViewHeight() + view_height_diff);
    }

    updateDrawArea();
}

/** Sets the UI offset of the UIElement
 * @param view_x_offset The new X viewport offset
 * @param view_y_offset The new Y viewport offset
 */
void UIElement::setViewOffset(double view_x_offset, double view_y_offset){
    int win_width, win_height;
    SDL_Renderer* renderer = Engine::getInstance()->getCamera()->getRenderer();
    SDL_GetRendererOutputSize(renderer, &win_width, &win_height);
    double view_x_diff = (view_x_offset - this->view_x_offset);
    double view_y_diff = (view_y_offset - this->view_y_offset);

    this->view_x_offset = view_x_offset;
    this->view_y_offset = view_y_offset;

    this->x += view_x_diff * win_width;
    this->y += view_y_diff * win_height;
    updateDrawArea();
}

/** Sets the visibility of the current animation state
 * @return If the object is active
 */
void UIElement::setActive(bool active){
    this->active = active;

    //Set active for all child elements
    for(UIElement* subelement : subelements){
        subelement->setActive(active);
    }
}

/** Sets the visibility of the current animation state
 * @return If the object is active
 */
void UIElement::setVisible(bool visible){
    this->visible = visible;

    //Set visible for all child elements
    for(UIElement* subelement : subelements){
        subelement->setVisible(visible);
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

    int win_width, win_height;
    SDL_GetWindowSize(Engine::getInstance()->getWindow(), &win_width, &win_height);

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
    return animation->addSprite(sprite_set, sprite_path, x_offset, y_offset, width, height);
}

/** Adds an element to the child element list
 * @param element The element to add
 */
void UIElement::addSubelement(UIElement* element){
    if(element != nullptr){
        subelements.push_back(element);
    }
}

/** Tries to get an element of the given name
 * @param name The name of the element
 * @return A pointer to the element with a matching name, or NULL if not found
 */
UIElement* UIElement::getSubelement(const char* name){
    //Check self
    if(strcmp(this->name, name) == 0){
        return this;
    }

    //Check children
    for(UIElement* subelement : subelements){
        UIElement* element = subelement->getSubelement(name);
        if(element != nullptr){
            return element;
        }
    }

    return nullptr;
}

void UIElement::deleteSubelement(const char* name){
    for(int i = 0; i < subelements.size(); i++)
    {
        if(strcmp(subelements[i]->getName(), name) == 0){
            delete subelements[i];
            subelements.erase(subelements.begin() + i);
            break;
        }
    }
}

/** Gets the subelements
 * @return The subelement list
 */
std::vector<UIElement*>& UIElement::getSubelements(){
    return this->subelements;
}

/** Gets the width of the UI element (in pixels)
 * @return The width of the UI element (in pixels)
 */
float UIElement::getWidth(){
    int height, width;
    SDL_GetWindowSize(Engine::getInstance()->getWindow(), &width, &height);
    return width * this->view_width;
}

/** Gets the pixel height of the UI element
 * @return The pixel height of the UI element
 */
float UIElement::getHeight(){
    int height, width;
    SDL_GetWindowSize(Engine::getInstance()->getWindow(), &width, &height);
    return height * this->view_height;
}

/** Gets the view width of the UI element
 * @return The view width of the UI element
 */
double UIElement::getViewWidth(){
    return this->view_width;
}

/** Gets the view height of the UI element
 * @return The view height of the UI element
 */
double UIElement::getViewHeight(){
    return this->view_height;
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

/** Serializing UI Elements (WIP)
 * @param file An open file to write to
 * @param base_zone The zone this object belongs to (used for zone-based offsets)
 * @return 0 on success
 */
int UIElement::serializeData(FILE* file, Zone* base_zone){
    if(Entity::serializeData(file, base_zone) == -1){
        return -1;
    }

    //Write X/Y offsets
    WriteVar((uint64_t)view_x_offset, uint64_t, file);
    WriteVar((uint64_t)view_y_offset, uint64_t, file);

    //Write width/height
    WriteVar((uint64_t)view_width, uint64_t, file);
    WriteVar((uint64_t)view_height, uint64_t, file);

    for(UIElement* subelement : subelements){
        subelement->serializeData(file, base_zone);
    }

    return 0;
}

/** Serializing UI Element assets (to be overridden by children, as necessary)
 * @param file An open file to write to
 * @param serialize_set The serialization set (logs saved assets)
 * @return -1 if serialization failed, 0 otherwise
 */
int UIElement::serializeAssets(FILE* file, SerializeSet& serialize_set){
    if(Entity::serializeAssets(file, serialize_set) == -1){
        return -1;
    }

    for(UIElement* subelement : subelements){
        subelement->serializeAssets(file, serialize_set);
    }

    return 0;
}