#include "./UIBorders.hpp"
#include "Maverick/Engine/Engine.hpp"

UIBorders::UIBorders(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height,
                     int draw_layer, const char* border_pattern, BORDER_TYPE border_type)
    : UIElement(name, view_x_offset, view_y_offset, view_width, view_height, draw_layer){
    this->type = UI_ELEMENT_TYPE::BORDERS;
    this->height_buff = 0;
    this->width_buff = 0;

    this->border_pattern = StrDeepCopy(border_pattern);
    this->border_type = border_type;

    addBorders();
}

unsigned int UIBorders::getHeightBuff(){
    return this->height_buff;
}

unsigned int UIBorders::getWidthBuff(){
    return this->width_buff;
}

void UIBorders::addBorders(){
    this->height_buff = 0;
    this->width_buff = 0;
    
    if(border_pattern != nullptr && strlen(border_pattern) != 0){
        //The patterns we look for while building borders
        char* file_patterns[4] = {"_top.bmp", "_bottom.bmp", "_left.bmp", "_right.bmp"};
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
                    double border_width, border_height, border_x, border_y;
                    switch(1 << i){
                        case(BORDER_TYPE::TOP_BORDER):{
                            border_width = this->draw_area.w;
                            border_height = border_surface->h * engine->getNativeScale();
                            border_x = this->draw_area.x;
                            border_y = this->draw_area.y;

                            if(border_height > this->height_buff){
                                this->height_buff = border_height;
                            }
                            break;
                        }
                        case(BORDER_TYPE::BOTTOM_BORDER):{
                            border_width = this->draw_area.w;
                            border_height = border_surface->h * engine->getNativeScale();
                            border_x = this->draw_area.x;
                            border_y = this->draw_area.y + this->draw_area.h - border_height;

                            if(border_height > this->height_buff){
                                this->height_buff = border_height;
                            }
                            break;
                        }
                        case(BORDER_TYPE::LEFT_BORDER):{
                            border_width = border_surface->w * engine->getNativeScale();
                            border_height = this->draw_area.h;
                            border_x = this->draw_area.x;
                            border_y = this->draw_area.y;

                            if(border_width > this->width_buff){
                                this->width_buff = border_width;
                            }
                            break;
                        }
                        case(BORDER_TYPE::RIGHT_BORDER):{
                            border_width = border_surface->w * engine->getNativeScale();
                            border_height = this->draw_area.h;
                            border_x = this->draw_area.x + this->draw_area.w - border_width;
                            border_y = this->draw_area.y;

                            if(border_width > this->width_buff){
                                this->width_buff = border_width;
                            }
                            break;
                        }
                    }

                    int win_width, win_height;
                    SDL_GetWindowSize(engine->getWindow(), &win_width, &win_height);

                    this->deleteSubelement(border_names[i]);

                    UIElement* border = new UIElement(border_names[i], border_x / (double)win_width, border_y / (double)win_height,
                                                      border_width / (double)win_width, border_height / (double)win_height, 1);
                    borders[i] = border;
                    Animation* border_background = new Animation(border_names[i], 1);
                    border->setBackground(border_background);
                    subelements.push_back(border);
                }
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
    
    for(int i = 0; i < 4; i++){
        if(!(border_type & (1 << i)) && borders[i] != nullptr){
            delete borders[i];
            borders[i] = nullptr;
        }
    }
    addBorders();
}