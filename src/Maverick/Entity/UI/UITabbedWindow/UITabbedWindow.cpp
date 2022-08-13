#include "UITabbedWindow.hpp"
#include "../../../Engine/Engine.hpp"

UITabbedWindow::UITabbedWindow(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height,
                               int draw_layer, double tab_height, const char* font_path, unsigned int font_size, const char* border_pattern,
                               uint8_t border_type, uint32_t border_buff)
               :UIElement(name, view_x_offset, view_y_offset, view_width, view_height, draw_layer),
               draw_text("tab_text", 0.0, 0.0, 0.0, 0.0, 0, font_path, "", 0.0, font_size, ALIGNMENT::CENTER_ALIGN, ALIGNMENT::CENTER_ALIGN,
                         border_pattern, border_type, border_buff){
    this->tab_height = tab_height;
}

/** Adds an element to the child element list
 * @param element The element to add
 */
void UITabbedWindow::addElement(UIElement* element){
    if(element != nullptr){
        if(subelements.empty()){
            selected_element = element;
        }
        subelements.push_back(element);
    }
}

/** Called every frame for processing
 * @param delta The amount of time that has passed (in ms)
 * @param step_size The step size used for physics calcluation (probably not needed here)
 */
void UITabbedWindow::_process(uint64_t delta, unsigned int steps){
    this->process(delta, steps);

    if(selected_element != nullptr){
        selected_element->_process(delta, steps);
    }

    cleanupHitboxImmunity(delta);
}

/** Draws this UIElement
 * @param renderer The SDL_Renderer we're drawing to
 * @param delta The time passed since last draw (in ms)
 * @param camera_x The X location of the camera
 * @param camera_y The Y location of the camera
 */
void UITabbedWindow::_draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y){
    int win_width, win_height;
    SDL_GetRendererOutputSize(renderer, &win_width, &win_height);
    float tab_width = this->view_width / this->subelements.size();
    draw_text.setViewSize(tab_width, tab_height);

    for(int i = 0; i < subelements.size(); i++){
        draw_text.setViewOffset(this->view_x_offset + (tab_width * i), this->view_y_offset);
        draw_text.setText(subelements[i]->getName());
        draw_text.draw(renderer, delta, camera_x, camera_y);
    }

    selected_element->_draw(renderer, delta, camera_x, camera_y);
}

/** Handles actions for this UIElement
 * @param control The control struct
 */
void UITabbedWindow::_action(Control* control){
    int win_width, win_height;
    SDL_GetRendererOutputSize(Engine::getInstance()->getCamera()->getRenderer(), &win_width, &win_height);

    const MouseState* mouse = control->getMouse();
    if(isMouseInside(control) && mouse->button_state & SDL_BUTTON_LMASK){
        int tab_num = getTabNumber(control);
        if(tab_num != -1){
            selected_element = subelements[tab_num];
        }
    }

    if(selected_element != nullptr){
        selected_element->_action(control);
    }
}

/** Returns the tab number which the mouse is hovering over, or -1 if it's not over any tab
 * @param control The control struct
 */
int UITabbedWindow::getTabNumber(Control* control){
    int win_width, win_height;
    SDL_GetRendererOutputSize(Engine::getInstance()->getCamera()->getRenderer(), &win_width, &win_height);
    float tab_width = this->view_width / this->subelements.size();
    const MouseState* mouse = control->getMouse();

    for(int i = 0; i < subelements.size(); i++){
        if(mouse->x > this->draw_area.x && mouse->x < (this->draw_area.x + (i * tab_width)) &&
           mouse->y > this->draw_area.y && mouse->y < (this->draw_area.y + (this->tab_height * win_height))){
            return i;
        }
    }

    return -1;
}