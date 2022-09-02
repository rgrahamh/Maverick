#include "./UITextBox.hpp"
#include "../../../Engine/Engine.hpp"

/** UITextBox constructor (for viewport calcs, 1.0 is one screen width/height)
 * @param name The name of the UIElement
 * @param view_x_offset The viewport X offset of the UIElement
 * @param view_y_offset The viewport X offset of the UIElement
 * @param view_width The viewport width of the UIElement
 * @param view_height The viewport height of the UIElement
 * @param animation_num The animation number of the UIElement (use for multiple would be blinking cursors)
 * @param draw_layer The draw layer of the UIElement (all child elements will be drawn directly on top)
 * @param font_path The path to the font you want to use for the text
 * @param text The text you wish to display in the box
 * @param scroll_speed The scroll speed of the text
 * @param point The point size of the text
 * @param text_x_alignment The horizontal alignment strategy
 * @param text_y_alignment The vertical alignment strategy
 * @param border_pattern The pattern for the border texture files you wish to use
 * @param border_types The border locations
 * @param border_buff How much additional buffer the text should be given (how far inset you should start the text box past the borders)
 */
UITextBox::UITextBox(const char* name, double view_x_offset, double view_y_offset, double view_width, double view_height,
                     int draw_layer, const char* font_path, char* text, float scroll_speed, int size, ALIGNMENT text_x_alignment,
                     ALIGNMENT text_y_alignment, const char* border_pattern, BORDER_TYPE border_types, uint16_t border_buff, bool wrapped)
    : UIElement(name, view_x_offset, view_y_offset, view_width, view_height, draw_layer){
    this->type = UI_ELEMENT_TYPE::TEXT_BOX;

    int win_width, win_height;
    Engine* engine = Engine::getInstance();
    SDL_GetWindowSize(engine->getWindow(), &win_width, &win_height);

    unsigned int height_buff = 0, width_buff = 0;
    if(border_pattern != nullptr){
        UIBorders* borders = new UIBorders(name, view_x_offset, view_y_offset, view_width, view_height,
                                      draw_layer, border_pattern, border_types);
        if(borders != nullptr){
            height_buff = borders->getHeightBuff() + (border_buff * engine->getGlobalXScale());
            width_buff = borders->getWidthBuff() + (border_buff * engine->getGlobalYScale());
        }

        this->addSubelement(borders);
    }

    double text_view_x_offset = ((view_x_offset * win_width) + width_buff) / win_width;
    double text_view_y_offset = ((view_y_offset * win_height) + height_buff) / win_height;

    double text_view_width = ((view_width * win_width) - (width_buff * 2)) / win_width;
    double text_view_height = ((view_height * win_height) - (height_buff * 2)) / win_height;

    this->text = new UIText(name, text_view_x_offset, text_view_y_offset, text_view_width, text_view_height, draw_layer,
                            font_path, text, scroll_speed, size, text_x_alignment, text_y_alignment, wrapped);
    
    this->addSubelement(this->text);
}

/** Sets the text in the textbox
 * @param style The text's new contents
 */
void UITextBox::setText(const char* text){
    this->text->setText(text);
}

/** Sets the text's font
 * @param style The text's new font
 */
void UITextBox::setFont(const char* font_path){
    this->text->setFont(font_path);
}

/** Sets the text's style
 * @param style The text's new style
 */
void UITextBox::setStyle(enum FONT_STYLE style){
    this->text->setStyle(style);
}

/** Sets the text's color
 * @param r The text's new red value
 * @param g The text's new green value
 * @param b The text's new blue value
 * @param a The text's new alpha value
 */
void UITextBox::setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a){
    this->text->setColor(r, g, b, a);
}

/** Sets the text's scrolling speed
 * @param scroll_speed The speed at which the text will scroll
 */
void UITextBox::setScrollSpeed(float scroll_speed){
    this->text->setScrollSpeed(scroll_speed);
}

/** Sets the text's horizontal alignment
 * @param y_alignment The desired vertical alignment
 */
void UITextBox::setXAlignment(ALIGNMENT x_alignment){
    this->text->setXAlignment(x_alignment);
}

/** Sets the text's vertical alignment
 * @param y_alignment The desired vertical alignment
 */
void UITextBox::setYAlignment(ALIGNMENT y_alignment){
    this->text->setYAlignment(y_alignment);
}