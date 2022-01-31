#ifndef COLORFILTER_H
#define COLORFILTER_H

#include "../Filter.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

enum COLOR_FILTER_TYPE{
    ADD_COLOR_FILTER,
    SUB_COLOR_FILTER
};

class ColorFilter : public Filter{
    public:
        ColorFilter(double x, double y, const char* add_filter_str, const char* sub_filter_str);
        ~ColorFilter();

        int setFilter(COLOR_FILTER_TYPE filter_type, const char* filter_path);
        void apply(SDL_Renderer* renderer);

    private:
        SDL_Surface* add_surface;
        SDL_Surface* sub_surface;
        SDL_Texture* add_filter;
        SDL_Texture* sub_filter;

        uint8_t calcShift(uint32_t mask);
        void setWidthHeight();
};
#endif