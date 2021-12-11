#include "./UIBorders.hpp"

extern TextureHash* texture_hash;

UIBorders::UIBorders(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height,
                             unsigned int animation_num, int draw_layer, SDL_Window* window, char* border_pattern,
                             uint8_t border_types)
    : UIElement(name, view_x_offset, view_y_offset, view_width, view_height, animation_num, draw_layer, UI_OBJECT_TYPE::BORDERS, window){
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

void UIBorders::addBorders(char* border_pattern, uint8_t border_types){
    int win_width, win_height;
    SDL_GetWindowSize(this->window, &win_width, &win_height);

    if(border_pattern != nullptr && strlen(border_pattern) != 0){
        //The patterns we look for while building borders
        char* file_patterns[4] = {"_top.png", "_bottom.png", "_left.png", "_right.png"};
        char* border_names[4] = {"top_border", "bottom_border", "left_border", "right_border"};
        for(int i = 0; i < 4; i++){
            if(border_types & (1 << i)){
                //Make a new combined string, formatted <border_pattern><file_pattern> (as an example, "basic_border_top.png")
                int combined_len = strlen(file_patterns[i]) + strlen(border_pattern) + 1;
                char sprite_path[combined_len];
                strcpy(sprite_path, border_pattern);
                strcat(sprite_path, file_patterns[i]);
                sprite_path[combined_len - 1] = '\0';

                //Get the surface of the border (necessary to check if the texture can be loaded & get texture height/width)
                SDL_Surface* border_surface = texture_hash->get(sprite_path);

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
                    this->borders[i] = new UIElement(border_names[i], border_x / (double)win_width, border_y / (double)win_height,
                                                     border_width / (double)win_width, border_height / (double)win_height, 1, 1,
                                                     UI_OBJECT_TYPE::GENERIC, this->window);
                    this->borders[i]->addSprite(0, sprite_path);
                }
            }
        }
    }
}

/** Draws this UITextBox
 * @param renderer The SDL_Renderer we're drawing to
 * @param delta The time passed since last draw (in ms)
 * @param camera_x The X location of the camera
 * @param camera_y The Y location of the camera
 */
void UIBorders::draw(SDL_Renderer* renderer, uint32_t delta, int camera_x, int camera_y){
    //Draw the textbox background
    if(active_animation < this->total_animation_num){
        this->animations[active_animation]->draw(renderer, delta, camera_x, camera_y);
    }

    //Draw the borders
    for(int i = 0; i < 4; i++){
        if(this->borders[i] != nullptr){
            this->borders[i]->draw(renderer, delta, camera_x, camera_y);
        }
    }

}