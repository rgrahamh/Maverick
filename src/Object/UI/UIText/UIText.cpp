#include "UIText.hpp"

/** UIText constructor (for viewport calcs, 1.0 is one screen width/height)
 * @param name The name of the UIElement
 * @param view_x_offset The viewport X offset of the UIElement
 * @param view_y_offset The viewport X offset of the UIElement
 * @param view_width The viewport width of the UIElement
 * @param view_height The viewport height of the UIElement
 * @param animation_num The animation number of the UIElement (use for multiple would be blinking cursors)
 * @param draw_layer The draw layer of the UIElement (all child elements will be drawn directly on top)
 * @param obj_type The object type (a UI_OBJECT_TYPE)
 * @param window The current window (used for viewport calculation)
 * @param text The text to display
 * @param font_path The filepath to the .ttf file you wish to use
 * @param point The point (size) of the text
 * @param scroll_speed The speed at which text scrolls (in units of chars/sec)
 */
UIText::UIText(const char* name, float view_x_offest, float view_y_offset, float view_width, float view_height, unsigned int animation_num, int draw_layer, enum UI_OBJECT_TYPE obj_type, SDL_Window* window, const char* text, const char* font_path, unsigned int point, float scroll_speed)
    : UIElement(name, view_x_offest, view_y_offset, view_width, view_height, animation_num, draw_layer, obj_type, window){
    this->point = point;
    this->scroll_speed = scroll_speed;

    this->buff = nullptr;
    this->text = nullptr;
    this->font = nullptr;
    if(font_path != nullptr){
        this->setFont(font_path);
    }
    this->back_color = SDL_Color{0xFF, 0xFF, 0xFF, 0x00};
    this->font_color = SDL_Color{0x00, 0x00, 0x00, 0xFF};

    this->setText(text);
}

/** UIText Destructor
 */
UIText::~UIText(){
    if(this->font != nullptr){
        free(this->font);
    }
    if(this->text != nullptr){
        free(this->text);
    }
    if(this->buff != nullptr){
        free(this->buff);
    }
}

/** Sets the text's contents 
 * @param font_path The text you wish to use (will make a local copy of the text)
 */
void UIText::setText(const char* text){
    //Reset the text timer (since we have new text)
    this->timer = 0.0;

    unsigned int len = strlen(text);

    if(this->text != nullptr){
        free(this->text);
    }
    this->text = (char*)malloc(len + 1);

    if(this->buff != nullptr){
        free(this->buff);
    }
    this->buff = (char*)malloc(len + 1);

    for(int i = 0; i < strlen(text); i++){
        this->text[i] = text[i];
    }
}

/** Sets the text's font (call upon change of font or point)
 * @param font_path The path to a .ttf you wish to use
 */
void UIText::setFont(const char* font_path){
    this->setStyle(TTF_STYLE_NORMAL);
    if(this->font != nullptr){
        delete this->font;
    }

    this->font_path = font_path;
    this->font = TTF_OpenFont(font_path, point);
    if(font == nullptr){
        fprintf(stderr, "Could not open texture: %s\n", TTF_GetError());
    }
}

/** Sets the point (size) of the font
 * @param point The point (size) of the font
 */
void UIText::setPoint(unsigned int point){
    this->point = point;
    this->setFont(this->font_path);
}

/** Sets the style of the font
 * @param style Any number of TTF_STYLE_* attrs OR'd together
 */
void UIText::setStyle(uint8_t style){
    if(this->font != nullptr){
        TTF_SetFontStyle(this->font, style);
    }
}

/** Sets the color of font
 * @param r The red value of the font
 * @param g The green value of the font
 * @param b The blue value of the font
 * @param a The alpha value of the font
 */
void UIText::setFontColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    this->font_color = SDL_Color{r, g, b, a};
}

/** Sets the background color of text
 * @param r The red value of the background
 * @param g The green value of the background
 * @param b The blue value of the background
 * @param a The alpha value of the background
 */
void UIText::setBackgroundColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    this->back_color = SDL_Color{r, g, b, a};
}

/** Sets the text scroll speed
 * @param scroll_speed The speed of text scroll speed (in chars/sec)
 */
void UIText::setScrollSpeed(float scroll_speed){
    this->scroll_speed = scroll_speed;
}

/** Draws this UIElement and all children UIElements
 * @param renderer The SDL_Renderer we're drawing to
 * @param delta The time passed since last draw (in ms)
 * @param camera_x The X location of the camera
 * @param camera_y The Y location of the camera
 */
void UIText::draw(SDL_Renderer* renderer, uint32_t delta, int camera_x, int camera_y){
    //Draw this element
    if(this->font != nullptr && this->text != nullptr){
        //Still need to figure out line breaks in the function; maybe use a buff we populate w/ the normal buff,
        // then call this section repeatedly w/ the new buf once populated?
        SDL_Surface* surface = TTF_RenderText(this->font, this->buff, this->font_color, this->back_color);
        if(surface != nullptr){
            SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
            SDL_RenderCopy(renderer, texture, nullptr, &this->draw_area);
            SDL_FreeSurface(surface);
            SDL_DestroyTexture(texture);
        }
    }

    //Draw all children elements (AFTER the parent element)
    UIElementLst* cursor = this->subelements;
    while(cursor != nullptr){
        cursor->element->draw(renderer, delta, camera_x, camera_y);
        cursor = cursor->next;
    }
}

/** Handles UIText processing (like scrolling text print)
 * @param delta The amount of time that has passed since last processing (in ms)
 */
void UIText::process(uint32_t delta){
    if(this->buff != nullptr && this->text != nullptr){
        //Scrolling the text from the internal text representation to the buffer
        //Determine the current buff len & text len
        unsigned int buff_len = strlen(this->buff);
        unsigned int text_len = strlen(this->text);
        if(buff_len < text_len){
            //If we aren't scrolling
            if(this->scroll_speed <= 0.0){
                for(int i = 0; i < text_len; i++){
                    this->buff[i] = this->text[i];
                }
            }
            //If we are scrolling
            else{
                //Calculate the max possible index based on the new time
                this->timer += delta;
                unsigned int max_index = this->timer * this->scroll_speed / 1000;

                //Fill buff from the current spot until we hit max_index or text_len
                for(unsigned int i = buff_len; i < max_index && i < text_len; i++){
                    this->buff[i] = this->text[i];
                }
            }
        }
    }
}