#include "./UIBorders.hpp"
#include "../../../Engine/Engine.hpp"

extern Engine* engine;

/** UIBorders constructor (for viewport calcs, 1.0 is one screen width/height)
 * @param name The name of the UIElement
 * @param view_x_offset The viewport X offset of the UIElement
 * @param view_y_offset The viewport X offset of the UIElement
 * @param view_width The viewport width of the UIElement
 * @param view_height The viewport height of the UIElement
 * @param animation_num The animation number of the UIElement (use for multiple would be blinking cursors)
 * @param draw_layer The draw layer of the UIElement (all child elements will be drawn directly on top)
 * @param window The current window (used for viewport calculation)
 * @param border_pattern The pattern for the border texture files you wish to use
 * @param border_types The border locations
 */
UIBorders::UIBorders(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height,
                     int draw_layer, char* border_pattern, uint8_t border_types)
    : UIElement(name, view_x_offset, view_y_offset, view_width, view_height, draw_layer){
    this->type = UI_ELEMENT_TYPE::BORDERS;

    this->subelements = nullptr;

    memset(this->borders, '\0', sizeof(UIElement*) * 4);

    addBorders(border_pattern, border_types);
}

/** Default destructor
 */
UIBorders::~UIBorders(){
    for(int i = 0; i < 4; i++){
        if(this->borders[i] != nullptr){
            delete this->borders[i];
        }
    }
}

/** Creates & adds borders based upon the border_pattern & border_types
 * @param border_pattern The file pattern to match border files to (ex. "<border_pattern>[_top | _bottom | _left | _right].bmp")
 * @param border_types The types of borders you want (bitwise or of zero to all BORDER_TYPE fields, depending upon which sides you want the borders)
 */
void UIBorders::addBorders(char* border_pattern, uint8_t border_types){
    if(border_pattern != nullptr && strlen(border_pattern) != 0){
        //The patterns we look for while building borders
        char* file_patterns[4] = {"_top.bmp", "_bottom.bmp", "_left.bmp", "_right.bmp"};
        char* border_names[4] = {"top_border", "bottom_border", "left_border", "right_border"};
        for(int i = 0; i < 4; i++){
            if(border_types & (1 << i)){
                //Make a new combined string, formatted <border_pattern><file_pattern> (as an example, "basic_border_top.bmp")
                int combined_len = strlen(file_patterns[i]) + strlen(border_pattern) + 1;
                char sprite_path[combined_len];
                strcpy(sprite_path, border_pattern);
                strcat(sprite_path, file_patterns[i]);
                sprite_path[combined_len - 1] = '\0';

                //Get the surface of the border (necessary to check if the texture can be loaded & get texture height/width)
                SDL_Surface* border_surface = engine->getSurface(sprite_path);

                if(border_surface != nullptr){
                    double border_width, border_height, border_x, border_y;
                    switch(1 << i){
                        case(BORDER_TYPE::TOP_BORDER):{
                            border_width = draw_area.w;
                            border_height = border_surface->h;
                            border_x = draw_area.x;
                            border_y = draw_area.y;
                            break;
                        }
                        case(BORDER_TYPE::BOTTOM_BORDER):{
                            border_width = draw_area.w;
                            border_height = border_surface->h;
                            border_x = draw_area.x;
                            border_y = draw_area.y + this->draw_area.h - border_surface->h;
                            break;
                        }
                        case(BORDER_TYPE::LEFT_BORDER):{
                            border_width = border_surface->w;
                            border_height = draw_area.h;
                            border_x = draw_area.x;
                            border_y = draw_area.y;
                            break;
                        }
                        case(BORDER_TYPE::RIGHT_BORDER):{
                            border_width = border_surface->w;
                            border_height = draw_area.h;
                            border_x = draw_area.x + this->draw_area.w - border_surface->w;
                            border_y = draw_area.y;
                            break;
                        }
                    }

                    //Draw layer doesn't matter here because we're drawing them manually w/ the text box
                    if(borders[i] != nullptr){
                        delete borders[i];
                    }
                    this->borders[i] = new UIElement(border_names[i], border_x / (double)SCREEN_WIDTH, border_y / (double)SCREEN_HEIGHT,
                                                     border_width / (double)SCREEN_WIDTH, border_height / (double)SCREEN_HEIGHT, 1);
                    this->borders[i]->addAnimation("border", 1);
                    this->borders[i]->addFrame("border", 0);
                    this->borders[i]->addSprite("border", "default", sprite_path);
                    this->borders[i]->setAnimation("border");
                }
            }
        }
    }
}

/** Draws these UIBorders
 * @param renderer The SDL_Renderer we're drawing to
 * @param delta The time passed since last draw (in ms)
 * @param camera_x The X location of the camera
 * @param camera_y The Y location of the camera
 */
void UIBorders::draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y){
    //Draw the textbox background
    if(active_animation != nullptr){
        this->active_animation->draw(renderer, delta, camera_x, camera_y);
    }

    //Draw the borders
    for(int i = 0; i < 4; i++){
        if(this->borders[i] != nullptr){
            this->borders[i]->draw(renderer, delta, camera_x, camera_y);
        }
    }

}