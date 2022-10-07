#include "UIElement.hpp"
#include "Maverick/Engine/Engine.hpp"

UIElement::UIElement(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height, int draw_layer){
    //ALWAYS HAVE THIS LINE w/ A DIFF TYPE YOU DEFINE FOR A SUBCLASS!
    this->type = UI_ELEMENT_TYPE::GENERIC_ELEMENT;

    this->name = StrDeepCopy(name);
    this->draw_layer = draw_layer;

    //Setting viewport position/scaling stats
    this->view_x_offset = view_x_offset;
    this->view_y_offset = view_y_offset;
    this->view_width = view_width;
    this->view_height = view_height;

    int win_width, win_height;
    SDL_GetWindowSize(Engine::getInstance()->getWindow(), &win_width, &win_height);

    this->draw_area.x = view_x_offset * win_width;
    this->draw_area.y = view_y_offset * win_height;

    this->draw_area.w = view_width * win_width;
    this->draw_area.h = view_height * win_height;

    this->background = nullptr;

    this->visible = true;
    this->active = true;

    this->width_buff = 0;
    this->height_buff = 0;
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

void UIElement::drawSubelements(uint64_t delta, const SDL_Rect& draw_scope)
{
    //Draw all children elements (AFTER the parent element)
    for(UIElement* subelement : subelements){
        if(subelement->isVisible()){
            subelement->_draw(delta, draw_scope);
        }
    }
}

void UIElement::_draw(uint64_t delta, const SDL_Rect& draw_scope){
    //Update draw area
    this->draw_area.x = draw_scope.x + (this->view_x_offset * draw_scope.w) + this->width_buff;
    this->draw_area.y = draw_scope.y + (this->view_y_offset * draw_scope.h) + this->height_buff;

    int width_mod = this->width_buff * 2;
    int height_mod = this->height_buff * 2;
    
    //If the width/height would be 0 or less, skip the draw step
    if(draw_scope.w <= width_mod || draw_scope.h <= height_mod){
        this->draw_area.w = 0;
        this->draw_area.h = 0;
        return;
    }

    this->draw_area.w = ceil(((double)draw_scope.w) * this->view_width) - width_mod;
    this->draw_area.h = ceil(((double)draw_scope.h) * this->view_height) - height_mod;

    //Draw the background, if present
    if(this->background != nullptr){
        this->background->draw(delta, this->draw_area);
    }

    //Draw any sort of custom logic
    this->draw(delta, this->draw_area);

    this->drawSubelements(delta, this->draw_area);
}

void UIElement::draw(uint64_t delta, const SDL_Rect& draw_scope){
    //Any custom UIElement logic goes here
}

const char* UIElement::getName(){
    return this->name;
}

int16_t UIElement::getDrawLayer(){
    return this->draw_layer;
}

bool UIElement::isActive(){
    return this->active;
}

bool UIElement::isVisible(){
    return this->visible;
}

uint32_t UIElement::getType(){
    return this->type;
}

void UIElement::setViewWidth(double view_width){
    this->view_width = view_width;
}

void UIElement::setViewHeight(double view_height){
    this->view_height = view_height;
}

void UIElement::setViewXOffset(double view_x_offset){
    this->view_x_offset = view_x_offset;
}

void UIElement::setViewYOffset(double view_y_offset){
    this->view_y_offset = view_y_offset;
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
    if(element != nullptr && element != this){
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
    if(mouse->x > this->draw_area.x && mouse->x < (this->draw_area.x + this->draw_area.w) &&
       mouse->y > this->draw_area.y && mouse->y < (this->draw_area.y + this->draw_area.h)){
        return true;
    }

    return false;
}

int UIElement::serializeData(FILE* file, Zone* base_zone){
    if(file == nullptr){
        return -1;
    }

    //Write the entity type
    WriteVar(this->type, uint32_t, file);

    //Identifier
    uint16_t identifier_len = strlen(this->name);
    WriteVar(identifier_len, uint16_t, file);
    fwrite(this->name, 1, identifier_len, file);

    //Draw layer
    WriteVar(draw_layer, int16_t, file);

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
    if(file == nullptr){
        return -1;
    }

    for(UIElement* subelement : subelements){
        subelement->serializeAssets(file, serialize_set);
    }

    return 0;
}

int UIElement::deserializeData(FILE* file){
    if(file == nullptr){
        return -1;
    }

    //Entity type
    ReadVar(this->type, file);

    //Identifier
    uint16_t identifier_len;
    ReadVar(identifier_len, file);
    this->name = (char*)malloc(identifier_len + 1);
    fread(&this->name, sizeof(char), identifier_len, file);
    name[identifier_len] = '\0';

    //Draw layer
    ReadVar(this->draw_layer, file);

    return 0;
}

void UIElement::setBackground(Animation* animation){
    this->background = animation;
}

Animation* UIElement::getBackground(){
    return this->background;
}

void UIElement::deleteBackground(){
    if(this->background == nullptr){
        delete this->background;
    }
}