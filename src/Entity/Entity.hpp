#ifndef ENTITY_H
#define ENTITY_H

#include "../Animation/Animation.hpp"
#include "../Animation/Hitbox/Hitbox.hpp"
#include "../Animation/Hitbox/HitRect/HitRect.hpp"
#include "../Animation/Hitbox/HitEllipse/HitEllipse.hpp"
#include "../Animation/Hitbox/HitEllipse/HitCone/HitCone.hpp"
#include "../Animation/Hitbox/HitboxCollision.hpp"
#include "../Control/Control.hpp"

#include <SDL2/SDL.h>
#include <unordered_map>

class Entity{
	public:
		Entity(const char* name, float start_x, float start_y, unsigned int animation_num, int draw_layer = 1);
		virtual ~Entity();
		char* getName();
		float getX();
		float getY();
		float getWidth();
		float getHeight();
		HitboxList* getHitboxes();
		unsigned int getDrawLayer();
		float getDrawAxis();
		bool isActive();
		bool isVisible();
		void* getAttr(const char* key);

		void addSprite(unsigned int animation_num, const char* sprite_path, unsigned int keytime, int x_offset, int y_offset, int width = -1, int height = -1);

		void addHitbox(unsigned int animation_num, HITBOX_SHAPE shape, float x_offset, float y_offset, float x_element, float y_element, unsigned int type, int sprite_num);
		void addHitbox(unsigned int animation_start, unsigned int animation_end, HITBOX_SHAPE shape, float x_offset, float y_offset, float x_element, float y_element, unsigned int type);
		void addHitbox(unsigned int animation_num, HITBOX_SHAPE shape, float x_offset, float y_offset, float x_element, float y_element, unsigned int type);
		void addHitbox(unsigned int animation_num, HITBOX_SHAPE shape, float x_offset, float y_offset, float x_element, float y_element, unsigned int type, float angle, float slice_prop, int sprite_num);
		void addHitbox(unsigned int animation_start, unsigned int animation_end, HITBOX_SHAPE shape, float x_offset, float y_offset, float x_element, float y_element, unsigned int type, float angle, float slice_prop);
		void addHitbox(unsigned int animation_num, HITBOX_SHAPE shape, float x_offset, float y_offset, float x_element, float y_element, unsigned int type, float angle, float slice_prop);

		void setX(float x);
		void setY(float y);
		void setAnimation(unsigned int animation_num);
		void setScale(unsigned int animation_num, float x_scale, float y_scale);
		void setScale(float x_scale, float y_scale);
		void setSize(unsigned int animation_num, float width, float height);
		void setSize(float width, float height);
		void setActive(bool active);
		void setVisible(bool visible);
		void setAttr(const char* key, void* val);

		//Processing functions
		virtual void _process(uint32_t delta) = 0;
		//Need this for custom processing
		virtual void process(uint32_t delta) = 0;

		virtual void _action(Control* control) = 0;
		virtual void action(Control* control) = 0;

		virtual void _draw(SDL_Renderer* renderer, uint32_t delta, int camera_x, int camera_y) = 0;
		virtual void draw(SDL_Renderer* renderer, uint32_t delta, int camera_x, int camera_y) = 0;
		
	protected:
		//Name
		char* name;

        //Position
        float x;
        float y;

		//Attributes
		HashTable* attr;

		//If the object is active/visible
		bool active;
		bool visible;

		//Animation tracking
		unsigned int active_animation;
		unsigned int animation_num;

		//Animation
		Animation** animations;
		AnimationList* ignored_animations;
};

#endif