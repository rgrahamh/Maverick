#include "Filter.hpp"

Filter::Filter(double x, double y, uint64_t width, uint64_t height){
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

/** Gets the top left X coord of the filter
 * @return The top left X coord of the filter
 */
double Filter::getX(){
    return this->x;
}

/** Gets the top left Y coord of the filter
 * @return The top left Y coord of the filter
 */
double Filter::getY(){
    return this->y;
}

/** Gets the width of the filter
 * @return The width of the filter
 */
uint64_t Filter::getWidth(){
    return this->width;
}

/** Gets the height of the filter
 * @return The height of the filter
 */
uint64_t Filter::getHeight(){
    return this->height;
}

/** Gets if the filter is visible
 * @return True if the filter is visible, false otherwise
 */
bool Filter::getVisible(){
    return this->visible;
}

/** Sets the filter visibility
 * @param visible True if you want the filter to be visible, false otherwise
 */
void Filter::setVisible(bool visible){
    this->visible = visible;
}