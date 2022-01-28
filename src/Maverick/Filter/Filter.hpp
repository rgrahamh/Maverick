#ifndef FILTER_H
#define FILTER_H

//That's why I say hey man nice shot; what a good shot, man....

#include <inttypes.h>
#include <SDL2/SDL.h>

//I don't know why we'd ever need this many filters, but for the sake of even spacing we'll let it be lol
enum FILTER_TYPE{
    GENERIC_FILTER = 0x30000000,
    COLOR_FILTER,
	EXTENDED_FILTER_TYPE_START = 0x30000800,
	FILTER_END   = 0x3FFFFFFF
};

class Filter{
    public:
        //width/height are optional because they will typically depend upon the filter you're applying (which isn't known yet)
        Filter(double x, double y, uint64_t width = 0, uint64_t height = 0);

        double getX();
        double getY();
        uint64_t getWidth();
        uint64_t getHeight();
        bool getVisible();
        void setVisible(bool visible);

        virtual void apply(SDL_Surface* input) = 0;

    protected:
        bool visible;

        double x;
        double y;
        uint64_t width;
        uint64_t height;

        uint8_t calcShift(uint32_t mask);
};

struct FilterList{
    Filter* filter;
    FilterList* next;
};
#endif