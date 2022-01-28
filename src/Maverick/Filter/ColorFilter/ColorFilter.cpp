#include "ColorFilter.hpp"
#include "../../Engine/Engine.hpp"

extern Engine* engine;
ColorFilter::ColorFilter(double x, double y, const char* red_add_str, const char* blue_add_str, const char* green_add_str, const char* red_sub_str, const char* blue_sub_str, const char* green_sub_str)
           : Filter(x, y){
    this->x = x;
    this->y = y;

    SDL_Renderer* renderer = engine->getCamera()->getRenderer();

    red_addition_filter = engine->getSurface(red_add_str);
    blue_addition_filter = engine->getSurface(blue_add_str);
    green_addition_filter = engine->getSurface(green_add_str);
    red_subtraction_filter = engine->getSurface(red_sub_str);
    blue_subtraction_filter = engine->getSurface(blue_sub_str);
    green_subtraction_filter = engine->getSurface(green_sub_str);

    this->width = red_addition_filter->w;
    this->height = red_addition_filter->h;
}

ColorFilter::~ColorFilter(){
    if(red_addition_filter != nullptr){
        SDL_FreeSurface(red_addition_filter);
    }
    if(blue_addition_filter != nullptr){
        SDL_FreeSurface(blue_addition_filter);
    }
    if(green_addition_filter != nullptr){
        SDL_FreeSurface(green_addition_filter);
    }
    if(red_subtraction_filter != nullptr){
        SDL_FreeSurface(red_subtraction_filter);
    }
    if(blue_subtraction_filter != nullptr){
        SDL_FreeSurface(blue_subtraction_filter);
    }
    if(green_subtraction_filter != nullptr){
        SDL_FreeSurface(green_subtraction_filter);
    }
}

int ColorFilter::setFilter(COLOR_FILTER_TYPE filter_type, const char* filter_path){
    if(filter_path == nullptr){
        return -1;
    }

    SDL_Surface* new_surface = engine->getSurface(filter_path);
    if(new_surface == nullptr){
        return -1;
    }

    if(new_surface->w < this->width){
        this->width = new_surface->w;
    }
    if(new_surface->h < this->height){
        this->height = new_surface->h;
    }

    SDL_Surface* cleanup;
    if(filter_type == COLOR_FILTER_TYPE::RED_ADDITION){
        cleanup = red_addition_filter;
        red_addition_filter = new_surface;
    }
    else if(filter_type == COLOR_FILTER_TYPE::BLUE_ADDITION){
        cleanup = blue_addition_filter;
        blue_addition_filter = new_surface;
    }
    else if(filter_type == COLOR_FILTER_TYPE::GREEN_ADDITION){
        cleanup = green_addition_filter;
        green_addition_filter = new_surface;
    }
    else if(filter_type == COLOR_FILTER_TYPE::RED_SUBTRACTION){
        cleanup = red_subtraction_filter;
        red_subtraction_filter = new_surface;
    }
    else if(filter_type == COLOR_FILTER_TYPE::BLUE_SUBTRACTION){
        cleanup = blue_subtraction_filter;
        blue_subtraction_filter = new_surface;
    }
    else if(filter_type == COLOR_FILTER_TYPE::GREEN_SUBTRACTION){
        cleanup = green_subtraction_filter;
        green_subtraction_filter = new_surface;
    }

    if(cleanup != nullptr){
        SDL_FreeSurface(cleanup);
    }
}

inline uint8_t ColorFilter::calcShift(uint32_t mask)
{
    if(mask == 0xff){
        return 0;
    }
    else if(mask == 0xff00){
        return 8;
    }
    else if(mask == 0xff0000){
        return 16;
    }
    return 24;
}

inline void ColorFilter::apply(SDL_Surface* input){
    Camera* camera = engine->getCamera();

    uint64_t camera_x = camera->getX();
    int input_width = input->w;
    int32_t filter_width = this->width;
    int32_t start_x;
    if(x < camera_x){
        filter_width -= camera_x - x;
        start_x = 0;
    }
    else if(x + width > camera_x + input_width){
        filter_width -= (x + width) - (camera_x + input_width);
        start_x = x;
    }
    else{
        start_x = x - camera_x;
    }

    uint64_t camera_y = camera->getY();
    int input_height = input->h;
    int32_t filter_height = height;
    int32_t start_y;
    if(camera_y > y){
        filter_height -= camera_y - y;
        start_y = 0;
    }
    else if(y + height > camera_y + input_height){
        filter_height -= (y + height) - (camera_y + input_height);
        start_y = y;
    }
    else{
        start_y = y - camera_y;
    }

    //In this case, filter wouldn't be seen anyways so may as well return. Also prevents out-of-bounds.
    if(filter_width < 0 || filter_height < 0 || start_x < 0 || start_y < 0){
        return;
    }

    //We're assuming 32-bit imgs here; logic will have to be modified if that changes
    uint64_t y_input_offset = start_y * input->w;
    uint64_t y_filter_offset = width;

    uint32_t* pixels = (uint32_t*)input->pixels;
    uint32_t* red_add_pixels = (uint32_t*)red_addition_filter->pixels;
    uint32_t* blue_add_pixels = (uint32_t*)blue_addition_filter->pixels;
    uint32_t* green_add_pixels = (uint32_t*)green_addition_filter->pixels;
    uint32_t* red_sub_pixels = (uint32_t*)red_subtraction_filter->pixels;
    uint32_t* blue_sub_pixels = (uint32_t*)blue_subtraction_filter->pixels;
    uint32_t* green_sub_pixels = (uint32_t*)green_subtraction_filter->pixels;

    register uint32_t red_mask = input->format->Rmask;
    register uint32_t blue_mask = input->format->Bmask;
    register uint32_t green_mask = input->format->Gmask;
    register uint32_t alpha_mask = input->format->Amask;

    register uint8_t red_shamt = calcShift(input->format->Rmask);
    register uint8_t blue_shamt = calcShift(input->format->Bmask);
    register uint8_t green_shamt = calcShift(input->format->Gmask);

    for(uint64_t y_coord = start_y; y_coord < height; y_coord++){
        y_input_offset += input->w;
        y_filter_offset += width;
        for(uint64_t x_coord; x_coord < width; x_coord++){
            uint32_t old_pixel = pixels[x_coord + y_input_offset];
            uint16_t red_val = ((old_pixel & red_mask) >> red_shamt)
                               * ((256 + ((red_add_pixels[x_coord + y_filter_offset] & red_mask) >> red_shamt)) / 256.0)
                               * (((red_sub_pixels[x_coord + y_filter_offset] & red_mask) >> red_shamt) / 256.0);
            if(red_val > 255){
                red_val = 255;
            }

            uint16_t blue_val = ((old_pixel & blue_mask) >> blue_shamt)
                               * ((256 + ((blue_add_pixels[x_coord + y_filter_offset] & blue_mask) >> blue_shamt)) / 256.0)
                               * (((blue_sub_pixels[x_coord + y_filter_offset] & blue_mask) >> blue_shamt) / 256.0);
            if(blue_val > 255){
                blue_val = 255;
            }

            uint16_t green_val = ((old_pixel & green_mask) >> green_shamt)
                               * ((256 + ((green_add_pixels[x_coord + y_filter_offset] & green_mask) >> green_shamt)) / 256.0)
                               * (((green_sub_pixels[x_coord + y_filter_offset] & green_mask) >> green_shamt) / 256.0);
            if(green_val > 255){
                green_val = 255;
            }

            pixels[x_coord + y_input_offset] = (red_val << red_shamt) | (blue_val << blue_shamt) | (green_val << green_shamt) | (old_pixel & alpha_mask);
        }
    }
}