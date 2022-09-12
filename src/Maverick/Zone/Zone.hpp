#ifndef ZONE_H
#define ZONE_H

#include "../Object/Object.hpp"
#include "../UIElement/UIElement.hpp"

class Zone{
    public:
        /** The zone constructor
         * @param name The name of the zone
         */
        Zone(const char* name, uint64_t global_x_coord, uint64_t global_y_coord);

        /** The zone default destructor
         */
        ~Zone();

        /** Gets all objects in the zone
         * @return An ObjectList* of all objects in the zone
         */
        ObjectList* getObjects();

        /** Gets all UI Elements in the zone
         * @return A UIElementList* of all UI Elements in the zone
         */
        UIElementList* getUIElements();

        /** Gets the global X coordinate
         * @return The global X coodrdinate
         */
        uint64_t getGlobalX();

        /** Gets the global Y coordinate
         * @return The global Y coodrdinate
         */
        uint64_t getGlobalY();

        /** Adds an object to the zone
         * @param element The object to add to the zone
         */
        void addObject(Object* object);

        /** Adds a UI element to the zone
         * @param element The element to add to the zone
         */
        void addUIElement(UIElement* element);

        /** Gets the name of the zone
         * @return The name of the zone
         */
        const char* getName();
    private:
        ObjectList* objects;
        UIElementList* ui;
        const char* name;

        uint64_t global_x_coord;
        uint64_t global_y_coord;
};

struct ZoneList{
    Zone* zone;
    struct ZoneList* next;
};

#endif