#include "./UIBorders.hpp"
#include "../../../Engine/Engine.hpp"

/** UIBorders constructor (for viewport calcs, 1.0 is one screen width/height)
 * @param name The name of the UIElement
 * @param view_x_offset The viewport X offset of the UIElement
 * @param view_y_offset The viewport X offset of the UIElement
 * @param view_width The viewport width of the UIElement
 * @param view_height The viewport height of the UIElement
 * @param animation_num The animation number of the UIElement (use for multiple would be blinking cursors)
 * @param draw_layer The draw layer of the UIElement (all child elements will be drawn directly on top)
 * @param border_pattern The pattern for the border texture files you wish to use
 * @param border_types The border locations
 */
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

/** Returns the height buffer for this set of borders
 * @return The height buffer for this set of borders
 */
unsigned int UIBorders::getHeightBuff(){
    return this->height_buff;
}

/** Returns the width buffer for this set of borders
 * @return The width buffer for this set of borders
 */
unsigned int UIBorders::getWidthBuff(){
    return this->width_buff;
}

/** Creates & adds borders based upon the border_pattern & border_types
 * @param border_pattern The file pattern to match border files to (ex. "<border_pattern>[_top | _bottom | _left | _right].bmp")
 * @param border_types The types of borders you want (bitwise or of zero to all BORDER_TYPE fields, depending upon which sides you want the borders)
 */
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
                            border_width = draw_area.w;
                            border_height = border_surface->h * engine->getGlobalYScale();
                            border_x = draw_area.x;
                            border_y = draw_area.y;

                            if(border_height > this->height_buff){
                                this->height_buff = border_height;
                            }
                            break;
                        }
                        case(BORDER_TYPE::BOTTOM_BORDER):{
                            border_width = draw_area.w;
                            border_height = border_surface->h * engine->getGlobalYScale();
                            border_x = draw_area.x;
                            border_y = draw_area.y + this->draw_area.h - border_height;

                            if(border_height > this->height_buff){
                                this->height_buff = border_height;
                            }
                            break;
                        }
                        case(BORDER_TYPE::LEFT_BORDER):{
                            border_width = border_surface->w * engine->getGlobalXScale();
                            border_height = draw_area.h;
                            border_x = draw_area.x;
                            border_y = draw_area.y;

                            if(border_width > this->width_buff){
                                this->width_buff = border_width;
                            }
                            break;
                        }
                        case(BORDER_TYPE::RIGHT_BORDER):{
                            border_width = border_surface->w * engine->getGlobalXScale();
                            border_height = draw_area.h;
                            border_x = draw_area.x + this->draw_area.w - border_width;
                            border_y = draw_area.y;

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
                    border->addAnimation("border", 1);
                    border->addFrame("border", 0);
                    border->addSprite("border", "default", sprite_path);
                    border->setAnimation("border");
                    subelements.push_back(border);
                }
            }
        }
    }
}

void UIBorders::setViewSize(double view_width, double view_height){
    UIElement::setViewSize(view_width, view_height);
    addBorders();
}

/** Sets the border pattern
 * @param border_pattern The border pattern to use
 */
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

/** Sets the border type
 * @param border_type The type of border to use
 */
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