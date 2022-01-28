#ifndef COLORFILTER_H
#define COLORFILTER_H

#include "../Filter.hpp"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

enum COLOR_FILTER_TYPE{
    RED_ADDITION,
    BLUE_ADDITION,
    GREEN_ADDITION,
    RED_SUBTRACTION,
    BLUE_SUBTRACTION,
    GREEN_SUBTRACTION
};

class ColorFilter : public Filter{
    public:
        ColorFilter(double x, double y, const char* red_add_str, const char* blue_add_str, const char* green_add_str, const char* red_sub_str, const char* blue_sub_str, const char* green_sub_str);
        ~ColorFilter();

        int setFilter(COLOR_FILTER_TYPE filter_type, const char* filter_path);
        void apply(SDL_Surface* input);

    private:
        SDL_Surface* red_addition_filter;
        SDL_Surface* blue_addition_filter;
        SDL_Surface* green_addition_filter;
        SDL_Surface* red_subtraction_filter;
        SDL_Surface* blue_subtraction_filter;
        SDL_Surface* green_subtraction_filter;

        uint8_t calcShift(uint32_t mask);
};
#endif