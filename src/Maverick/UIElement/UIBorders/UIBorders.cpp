#include "./UIBorders.hpp"
#include "Maverick/Engine/Engine.hpp"

UIBorders::UIBorders(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height,
                     int draw_layer, const char* border_pattern, BORDER_TYPE border_type)
    : UIElement(name, view_x_offset, view_y_offset, view_width, view_height, draw_layer){
    this->type = UI_ELEMENT_TYPE::BORDERS;

    this->border_pattern = StrDeepCopy(border_pattern);
    this->border_type = border_type;

    memset(this->borders, 0, sizeof(UIBorders*) * 4);

    addBorders();
}

void UIBorders::draw(uint64_t delta, const SDL_Rect& draw_scope){
    Engine* engine = Engine::getInstance();
    double native_scale = engine->getNativeScale();
    for(int i = 0; i < 4; i++){
        if(border_type & (1 << i) && borders[i] != nullptr){
            SDL_Rect border_draw;
            switch(1 << i){
                case(BORDER_TYPE::TOP_BORDER):{
                    border_draw.w = draw_scope.w;
                    border_draw.h = borders[i]->getBackground()->getHeight() * native_scale;
                    border_draw.x = draw_scope.x;
                    border_draw.y = draw_scope.y;
                    break;
                }
                case(BORDER_TYPE::BOTTOM_BORDER):{
                    border_draw.w = draw_scope.w;
                    border_draw.h = borders[i]->getBackground()->getHeight() * native_scale;
                    border_draw.x = draw_scope.x;
                    border_draw.y = draw_scope.y + draw_scope.h - borders[i]->getBackground()->getHeight() * native_scale;
                    break;
                }
                case(BORDER_TYPE::LEFT_BORDER):{
                    border_draw.w = borders[i]->getBackground()->getWidth() * native_scale;
                    border_draw.h = draw_scope.h;
                    border_draw.x = draw_scope.x;
                    border_draw.y = draw_scope.y;
                    break;
                }
                case(BORDER_TYPE::RIGHT_BORDER):{
                    border_draw.w = borders[i]->getBackground()->getWidth() * native_scale;
                    border_draw.h = draw_scope.h;
                    border_draw.x = draw_scope.x + draw_scope.w - borders[i]->getBackground()->getWidth() * native_scale;
                    border_draw.y = draw_scope.y;
                    break;
                }
            }

            borders[i]->_draw(delta, border_draw);
        }
    }
}

void UIBorders::addBorders(){
    if(border_pattern == nullptr || strlen(border_pattern) == 0){
        return;
    }

    //The patterns we look for while building borders
    char* file_patterns[4] = {"_top", "_bottom", "_left", "_right"};
    char* border_names[4] = {"top_border", "bottom_border", "left_border", "right_border"};
    for(int i = 0; i < 4; i++){
        if(border_type & (1 << i)){
            //Make a new combined string, formatted <border_pattern><file_pattern> (as an example, "basic_border_top.bmp")
            int combined_len = strlen(file_patterns[i]) + strlen(border_pattern) + 1;
            char sprite_path[combined_len];
            strcpy(sprite_path, border_pattern);
            strcat(sprite_path, file_patterns[i]);
            sprite_path[combined_len - 1] = '\0';

            //Get the surface of the border (necessary to check if the texture can be loaded & get texture height/width)
            Engine* engine = Engine::getInstance();
            SDL_Surface* border_surface = engine->getSurface(sprite_path);

            if(border_surface != nullptr){
                UIElement* border = new UIElement(border_names[i], 0, 0, 1.0, 1.0, 1);
                borders[i] = border;
                Animation* border_background = new Animation(border_names[i], 1);
                border_background->addFrame(0);
                border_background->addSprite("border", sprite_path, 0.0, 0.0);
                border->setBackground(border_background);
            }
        }
    }
}

void UIBorders::setBorderPattern(const char* border_pattern){
    if(border_pattern != nullptr){
        delete border_pattern;
    }
    this->border_pattern = StrDeepCopy(border_pattern);

    for(int i = 0; i < 4; i++){
        if(borders[i] != nullptr){
            delete borders[i];
            borders[i] = nullptr;
        }
    }

    addBorders();
}

void UIBorders::setBorderType(BORDER_TYPE border_type){
    this->border_type = border_type;
}