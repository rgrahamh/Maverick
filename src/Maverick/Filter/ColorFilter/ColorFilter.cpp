#include "ColorFilter.hpp"
#include "../../Engine/Engine.hpp"

static const SDL_BlendMode add_blend = SDL_ComposeCustomBlendMode(SDL_BlendFactor::SDL_BLENDFACTOR_ONE,
                                                                  SDL_BlendFactor::SDL_BLENDFACTOR_ONE,
                                                                  SDL_BlendOperation::SDL_BLENDOPERATION_ADD,
                                                                  SDL_BlendFactor::SDL_BLENDFACTOR_ZERO,
                                                                  SDL_BlendFactor::SDL_BLENDFACTOR_ONE,
                                                                  SDL_BlendOperation::SDL_BLENDOPERATION_ADD);

static const SDL_BlendMode sub_blend = SDL_ComposeCustomBlendMode(SDL_BlendFactor::SDL_BLENDFACTOR_ONE,
                                                                  SDL_BlendFactor::SDL_BLENDFACTOR_ONE,
                                                                  SDL_BlendOperation::SDL_BLENDOPERATION_SUBTRACT,
                                                                  SDL_BlendFactor::SDL_BLENDFACTOR_ZERO,
                                                                  SDL_BlendFactor::SDL_BLENDFACTOR_ZERO,
                                                                  SDL_BlendOperation::SDL_BLENDOPERATION_MAXIMUM);

extern Engine* engine;
ColorFilter::ColorFilter(double x, double y, const char* sub_filter_str, const char* add_filter_str)
           : Filter(x, y){
    this->x = x;
    this->y = y;

    SDL_Renderer* renderer = engine->getCamera()->getRenderer();

    this->add_surface = engine->getSurface(add_filter_str);
    if(this->add_surface != nullptr){
        this->add_filter = SDL_CreateTextureFromSurface(renderer, this->add_surface);
        if(add_filter != nullptr){
            SDL_SetTextureBlendMode(this->add_filter, SDL_BLENDMODE_ADD);
        }
    }

    this->sub_surface = engine->getSurface(sub_filter_str);
    if(this->sub_surface != nullptr){
        this->sub_filter = SDL_CreateTextureFromSurface(renderer, this->sub_surface);
        if(sub_filter != nullptr){
            SDL_SetTextureBlendMode(this->sub_filter, SDL_BLENDMODE_MOD);
        }
    }

    this->setWidthHeight();
}

ColorFilter::~ColorFilter(){
    if(this->add_surface != nullptr){
        SDL_FreeSurface(this->add_surface);
    }
    if(this->add_filter != nullptr){
        SDL_DestroyTexture(this->add_filter);
    }
    if(this->sub_surface != nullptr){
        SDL_FreeSurface(this->sub_surface);
    }
    if(this->sub_filter != nullptr){
        SDL_DestroyTexture(this->sub_filter);
    }
}

inline void ColorFilter::setWidthHeight(){
    if(this->add_surface != nullptr && this->sub_surface != nullptr){
        this->width = (this->add_surface->w > this->sub_surface->w)? this->add_surface->w : this->sub_surface->w;
        this->height = (this->add_surface->h > this->sub_surface->h)? this->add_surface->h : this->sub_surface->h;
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

    SDL_Renderer* renderer = engine->getCamera()->getRenderer();
    if(filter_type == COLOR_FILTER_TYPE::ADD_COLOR_FILTER){
        SDL_Texture* new_texture = SDL_CreateTextureFromSurface(renderer, new_surface);
        if(new_texture != nullptr){
            if(add_surface != nullptr){
                SDL_FreeSurface(add_surface);
            }
            if(add_filter != nullptr){
                SDL_DestroyTexture(add_filter);
            }
            this->add_surface = new_surface;
            this->add_filter = new_texture;
            SDL_SetTextureBlendMode(this->add_filter, add_blend);
        }
    }
    else if(filter_type == COLOR_FILTER_TYPE::SUB_COLOR_FILTER){
        SDL_Texture* new_texture = SDL_CreateTextureFromSurface(renderer, new_surface);
        if(new_texture != nullptr){
            if(sub_surface != nullptr){
                SDL_FreeSurface(sub_surface);
            }
            if(sub_filter != nullptr){
                SDL_DestroyTexture(sub_filter);
            }
            this->sub_surface = new_surface;
            this->sub_filter = new_texture;
            SDL_SetTextureBlendMode(this->sub_filter, sub_blend);
        }
    }

    this->setWidthHeight();
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

inline void ColorFilter::apply(SDL_Renderer* renderer){
    if(this->add_filter == nullptr || this->sub_filter == nullptr){
        return;
    }

    Camera* camera = engine->getCamera();

    uint64_t camera_x = camera->getX();
    uint64_t camera_y = camera->getY();

    SDL_Rect draw_rect;
    draw_rect.x = this->x - camera_x;
    draw_rect.y = this->y - camera_y;
    draw_rect.w = this->width;
    draw_rect.h = this->height;

    if(draw_rect.x > SCREEN_WIDTH || draw_rect.y > SCREEN_HEIGHT ||
       draw_rect.x + draw_rect.w <= 0 || draw_rect.y + draw_rect.h <= 0){
        return;
    }

    //Cropping off unnecessary right/bottom section
    if(draw_rect.x + draw_rect.w > SCREEN_WIDTH){
        draw_rect.w = SCREEN_WIDTH - draw_rect.x;
    }
    if(draw_rect.y + draw_rect.h > SCREEN_HEIGHT){
        draw_rect.h = SCREEN_HEIGHT - draw_rect.y;
    }

    //Cropping off unnecessary left/top section
    if(draw_rect.x < 0){
        draw_rect.w -= abs(draw_rect.x);
        draw_rect.x = 0;
    }
    if(draw_rect.y < 0){
        draw_rect.h -= abs(draw_rect.h);
        draw_rect.y = 0;
    }

    SDL_RenderCopy(renderer, sub_filter, nullptr, &draw_rect);
    SDL_RenderCopy(renderer, add_filter, nullptr, &draw_rect);
}