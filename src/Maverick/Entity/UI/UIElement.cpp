#include "UIElement.hpp"
#include "../../Engine/Engine.hpp"

UIElement::UIElement(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height, int draw_layer)
         : Entity(name, draw_layer){
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

    this->width = view_width * win_width;
    this->height = view_height * win_height;

    this->background = nullptr;
}

UIElement::~UIElement(){
    for(UIElement* subelement : subelements){
        delete subelement;
    }
}

void UIElement::process(uint64_t delta, unsigned int steps){
    return;
}

void UIElement::_process(uint64_t delta, unsigned int steps){
    this->process(delta, steps);

    for(UIElement* subelement : subelements){
        if(subelement->isActive()){
            subelement->_process(delta, steps);
        }
    }
}

void UIElement::action(Control* control){
    return;
}

void UIElement::_action(Control* control){
    this->action(control);

    //Perform all actions for child elements (generic element has no default actions)
    for(UIElement* subelement : subelements){
        if(subelement->isActive()){
            subelement->_action(control);
        }
    }
}

void UIElement::_draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y){
    //Draw this element
    updateDrawArea();

    if(this->background != nullptr){
        this->background->draw(renderer, delta, camera_x, camera_y);
    }

    this->draw(renderer, delta, camera_x, camera_y);

    //Draw all children elements (AFTER the parent element)
    int win_width, win_height;
    SDL_GetRendererOutputSize(Engine::getInstance()->getCamera()->getRenderer(), &win_width, &win_height);
    for(UIElement* subelement : subelements){
        if(subelement->isVisible()){
            subelement->_draw(renderer, delta, camera_x, camera_y);
        }
    }
}

void UIElement::draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y){
    //Draw logic goes here
}

inline void UIElement::updateDrawArea(){
    int win_width, win_height;
    SDL_GetRendererOutputSize(Engine::getInstance()->getCamera()->getRenderer(), &win_width, &win_height);
    this->x = this->view_x_offset * win_width;
    this->y = this->view_y_offset * win_height;
    this->width = this->view_width * win_width;
    this->height = this->view_height * win_height;
}

void UIElement::setViewSize(double view_width, double view_height){
    int win_width, win_height;
    SDL_GetWindowSize(Engine::getInstance()->getWindow(), &win_width, &win_height);
    double old_view_width = this->view_width;
    double old_view_height = this->view_height;
    this->view_width = view_width;
    this->view_height = view_height;
}

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
}

void UIElement::setActive(bool active){
    this->active = active;

    //Set active for all child elements
    for(UIElement* subelement : subelements){
        subelement->setActive(active);
    }
}

void UIElement::setVisible(bool visible){
    this->visible = visible;

    //Set visible for all child elements
    for(UIElement* subelement : subelements){
        subelement->setVisible(visible);
    }
}

void UIElement::addSubelement(UIElement* element){
    if(element != nullptr){
        subelements.push_back(element);
    }
}

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

std::vector<UIElement*>& UIElement::getSubelements(){
    return this->subelements;
}

double UIElement::getWidth(){
    int height, width;
    SDL_GetWindowSize(Engine::getInstance()->getWindow(), &width, &height);
    return width * this->view_width;
}

double UIElement::getHeight(){
    int height, width;
    SDL_GetWindowSize(Engine::getInstance()->getWindow(), &width, &height);
    return height * this->view_height;
}

double UIElement::getViewWidth(){
    return this->view_width;
}

double UIElement::getViewHeight(){
    return this->view_height;
}

bool UIElement::isMouseInside(Control* control){
    const MouseState* mouse = control->getMouse();
    if(mouse->x > this->x && mouse->x < (this->x + this->width) &&
       mouse->y > this->y && mouse->y < (this->y + this->height)){
        return true;
    }

    return false;
}

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

int UIElement::serializeAssets(FILE* file, SerializeSet& serialize_set){
    if(Entity::serializeAssets(file, serialize_set) == -1){
        return -1;
    }

    for(UIElement* subelement : subelements){
        subelement->serializeAssets(file, serialize_set);
    }

    return 0;
}

void UIElement::setBackground(Animation* background){
    if(this->background == nullptr){
        delete this->background;
    }

    this->background = background;
}