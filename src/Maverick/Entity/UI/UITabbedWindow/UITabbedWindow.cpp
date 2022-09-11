#include "UITabbedWindow.hpp"
#include "../../../Engine/Engine.hpp"

UITabbedWindow::UITabbedWindow(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height,
                               int draw_layer, double tab_height, const char* font_path, unsigned int font_size, const char* border_pattern,
                               BORDER_TYPE border_type, uint32_t border_buff)
               :UIElement(name, view_x_offset, view_y_offset, view_width, view_height, draw_layer),
               draw_text("tab_text", 0.0, 0.0, 0.0, 0.0, 0, font_path, "", 0.0, font_size, ALIGNMENT::CENTER_ALIGN, ALIGNMENT::CENTER_ALIGN,
                         border_pattern, border_type, border_buff, false){
    this->tab_height = tab_height;
    this->current_tab = -1;
}

void UITabbedWindow::addSubelement(UIElement* element){
    if(element != nullptr){
        if(subelements.empty()){
            this->current_tab = 0;
        }
        subelements.push_back(element);
    }
}

void UITabbedWindow::_process(uint64_t delta, unsigned int steps){
    this->process(delta, steps);

    if(this->current_tab != -1){
        subelements[current_tab]->_process(delta, steps);
    }
}

void UITabbedWindow::_draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y){
    updateDrawArea();
    this->draw(renderer, delta, camera_x, camera_y);

    int win_width, win_height;
    SDL_GetRendererOutputSize(renderer, &win_width, &win_height);
    float tab_width = this->view_width / this->subelements.size();
    draw_text.setViewSize(tab_width, tab_height);

    for(int i = 0; i < subelements.size(); i++){
        draw_text.setText(subelements[i]->getName());
        draw_text._draw(renderer, delta, (-1 * camera_x) + (tab_width * i), -1 * camera_y);
    }

    subelements[current_tab]->_draw(renderer, delta, -1 * camera_x, -1 * camera_y);
}

void UITabbedWindow::_action(Control* control){
    int win_width, win_height;
    SDL_GetRendererOutputSize(Engine::getInstance()->getCamera()->getRenderer(), &win_width, &win_height);

    const MouseState* mouse = control->getMouse();
    if(isMouseInside(control) && mouse->button_state & SDL_BUTTON_LMASK){
        int tab_num = getTabNumber(control);
        if(tab_num != -1){
            current_tab = tab_num;
        }
    }

    if(current_tab != -1){
        subelements[current_tab]->_action(control);
    }
}

int UITabbedWindow::getTabNumber(Control* control){
    int win_width, win_height;
    SDL_GetRendererOutputSize(Engine::getInstance()->getCamera()->getRenderer(), &win_width, &win_height);
    double tab_width = this->view_width / this->subelements.size();
    const MouseState* mouse = control->getMouse();

    for(int i = 0; i < subelements.size(); i++){
        if(mouse->x > this->x && mouse->x < (this->x + ((i + 1) * (tab_width * win_width))) &&
           mouse->y > this->y && mouse->y < (this->y + (this->tab_height * win_height))){
            return i;
        }
    }

    return -1;
}