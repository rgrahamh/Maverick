#ifndef UITEXT_H
#define UITEXT_H

#include "../UIElement.hpp"
#include "../../../Font/Font.hpp"

enum ALIGNMENT{
    CENTER_ALIGN,
    STANDARD_ALIGN
};

class UIText : public UIElement{
    public:
        UIText(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height, int draw_layer, const char* font_path, const char* text = "", float scroll_speed = 0.0, unsigned int size = 12, ALIGNMENT x_alignment = ALIGNMENT::STANDARD_ALIGN, ALIGNMENT y_alignment = ALIGNMENT::STANDARD_ALIGN);
        virtual ~UIText();

        void draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y);
        void process(uint64_t delta, unsigned int steps) override;

        //Set the displayed text
        void setText(const char* text);

        //Sets the font
        void setFont(const char* font_name);

        //Sets the style (strikethrough, bold, etc.)
        void setStyle(enum FONT_STYLE style);

        //Sets the size
        void setSize(unsigned int size);

        //Sets the text color
        void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);

        //Sets the X alignment
        void setXAlignment(ALIGNMENT x_alignment);

        //Sets the Y alignment
        void setYAlignment(ALIGNMENT y_alignment);

        //Sets the speed at which text scrolls
        //(0.0 means it's instant, otherwise it's in units of chars/sec)
        void setScrollSpeed(float scroll_speed);

        //Updates to the next page's contents (for continued text on the next "page")
        void nextPage();

        //If we hit the end of the text
        bool hitTextEnd();

        //If we hit the character limit
        bool hitDrawEnd();

    private:
        //Gets the next line break
        char* getNextBreak(char* str);

        //Gets the letter height
        float getCharHeight();

        //Flushes current print & ref buffers
        void flushBuffers();

        //The text
        char* text;

        //The start of the next text selection
        char* text_buff_cursor;

        //The reference buffer (and where it is on the text)
        char* ref_buff;

        //The printing buffer
        char* print_buff;

        //The font object & filepath
        Font* font;
        
        //Text size (scaling factor)
        uint8_t size;

        //Text style (italics, bold, etc.)
        enum FONT_STYLE style;

        //The scroll speed (chars per second)
        float scroll_speed;
        uint32_t timer;

        //Line characteristics
        uint32_t* line_start_x;
        uint32_t num_lines;

        //The foreground & background colors
        SDL_Color font_color;

        //The text's horizontal alignment
        ALIGNMENT x_alignment;

        //The text's vertical alignment
        ALIGNMENT y_alignment;
};

#endif