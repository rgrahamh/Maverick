#ifndef ENTITY_H
#define ENTITY_H

#include "../HashTable/HashTable.hpp"
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
		Entity(const char* name, float start_x, float start_y, int draw_layer = 1);
		virtual ~Entity();
		char* getName();
		float getX();
		float getY();
		float getWidth();
		float getHeight();
		HitboxList* getHitboxes();
		int getDrawLayer();
		float getDrawAxis();
		bool isActive();
		bool isVisible();
		void* getAttr(const char* key);
		uint32_t getType();

		int addAnimation(const char* animation_name);

		int addSprite(const char* animation_name, const char* sprite_path, unsigned int keytime, int x_offset, int y_offset, int width = -1, int height = -1);

		int addHitbox(const char* animation_name, HITBOX_SHAPE shape, float x_offset, float y_offset, float x_element, float y_element, unsigned int type, int sprite_num);
		int addHitbox(HITBOX_SHAPE shape, float x_offset, float y_offset, float x_element, float y_element, unsigned int type);
		int addHitbox(const char* animation_name, HITBOX_SHAPE shape, float x_offset, float y_offset, float x_element, float y_element, unsigned int type);
		int addHitbox(const char* animation_name, HITBOX_SHAPE shape, float x_offset, float y_offset, float x_element, float y_element, unsigned int type, float angle, float slice_prop, int sprite_num);
		int addHitbox(HITBOX_SHAPE shape, float x_offset, float y_offset, float x_element, float y_element, unsigned int type, float angle, float slice_prop);
		int addHitbox(const char* animation_name, HITBOX_SHAPE shape, float x_offset, float y_offset, float x_element, float y_element, unsigned int type, float angle, float slice_prop);

		int addSound(const char* animation_name, const char* sound_path, int sequence_num);

		void setX(float x);
		void setY(float y);
		int setAnimation(const char* animation_name);
		int setScale(const char* animation_name, float x_scale, float y_scale);
		void setScale(float x_scale, float y_scale);
		int setSize(const char* animation_name, float width, float height);
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

		virtual int serializeData(char** buff_ptr);
		virtual int serializeAssets(char** buff_ptr, std::unordered_set<std::string>& sprite_set, std::unordered_set<std::string>& audio_set);
		
	protected:
		//Name
		char* name;

		//The entity type (exact values are specified by child classes)
		uint32_t type;

        //Position
        float x;
        float y;

		//Attributes
		HashTable* attr;

		//If the object is active/visible
		bool active;
		bool visible;

		//Animation
		Animation* active_animation;
		AnimationList* animations;
		AnimationList* ignored_animations;

		//Draw layer tracking
		int draw_layer;

		Animation* findAnimation(const char* animation_name);
};

#endif