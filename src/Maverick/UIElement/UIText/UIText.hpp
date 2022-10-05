#ifndef UITEXT_H
#define UITEXT_H

#include "../UIElement.hpp"
#include "Maverick/Text/Text.hpp"

class UIText : public UIElement{
    public:
        /** UIText constructor (for viewport calcs, 1.0 is one screen width/height)
         * @param name The name of the UIElement
         * @param view_x_offset The viewport X offset of the UIElement
         * @param view_y_offset The viewport Y offset of the UIElement
         * @param view_width The viewport width of the UIElement
         * @param view_height The viewport height of the UIElement
         * @param animation_num The animation number of the UIElement (use for multiple would be blinking cursors)
         * @param draw_layer The draw layer of the UIElement (all child elements will be drawn directly on top)
         * @param text The text to display
         * @param font_path The filepath to the .ttf file you wish to use
         * @param scroll_speed The speed at which text scrolls (in units of chars/sec)
         * @param point The point (size) of the text
         * @param x_alignment The horizontal alignment of the text
         * @param y_alignment The vertical alignment of the text
         */
        UIText(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height, int draw_layer, const char* font_path, const char* text = "", float scroll_speed = 0.0, unsigned int size = 12, ALIGNMENT x_alignment = ALIGNMENT::STANDARD_ALIGN, ALIGNMENT y_alignment = ALIGNMENT::STANDARD_ALIGN);

        /** UIText Default Destructor
         */
        ~UIText();

        void draw(uint64_t delta, const SDL_Rect& draw_scope) override;
        void process(uint64_t delta, unsigned int steps) override;

        /** Sets the text's contents 
         * @param text The text you wish to use (will make a local copy of the text)
         */
        void setText(const char* text);

        /** Sets the text's font (call upon change of font or size)
         * @param font_name The path to a .ttf you wish to use
         */
        void setFont(const char* font_name);

        /** Sets the style of the font
         * @param style Any number of TTF_STYLE_* attrs OR'd together
         */
        void setStyle(enum FONT_STYLE style);

        /** Sets the size of the font
         * @param size The point (size) of the font
         */
        void setSize(unsigned int size);

        /** Sets the color of font
         * @param r The red value of the font
         * @param g The green value of the font
         * @param b The blue value of the font
         * @param a The alpha value of the font
         */
        void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0xFF);

        /** Sets the horizontal alignment
         * @param horizontal_align The horizontal alignment
         */
        void setHorizontalAlignment(ALIGNMENT horizontal_align);

        /** Sets the vertical alignment
         * @param vertical_align The vertical alignment
         */
        void setVerticalAlignment(ALIGNMENT vertical_align);

        /** Sets the text scroll speed
         * @param scroll_speed The speed of text scroll speed (0.0 means it's instant, otherwise it's in units of chars/sec)
         */
        void setScrollSpeed(float scroll_speed);

        /** Loads the next section of text into the ref buff
         */
        void nextPage();

        /** If we've hit the end of text
         * @return If we've hit the end of text
         */
        bool hitTextEnd();

        /** If we've hit the end of the draw area
         * @return If we've hit the end of the draw area
         */
        bool hitDrawEnd();

    private:
        /** Gets the next break point (a tab or space character)
         * @param str The string we're checking the next break for
         * @return A char* pointing to the first instance of a break
         */
        char* getNextBreak(char* str);

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

        //Character height (calculated on-init)
        uint32_t char_height;

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
        ALIGNMENT horizontal_align;

        //The text's vertical alignment
        ALIGNMENT vertical_align;
};

#endif