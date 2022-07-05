#ifndef ENTITY_H
#define ENTITY_H

#include "../HashTable/AttributeHash/AttributeHash.hpp"
#include "../Animation/Animation.hpp"
#include "../Animation/Hitbox/Hitbox.hpp"
#include "../Animation/Hitbox/HitRect/HitRect.hpp"
#include "../Animation/Hitbox/HitEllipse/HitEllipse.hpp"
#include "../Animation/Hitbox/HitEllipse/HitCone/HitCone.hpp"
#include "../Animation/Hitbox/HitboxCollision.hpp"
#include "../Control/Control.hpp"

#include <SDL2/SDL.h>
#include <unordered_map>

class Zone;

class Entity{
	public:
		Entity(const char* name, float start_x, float start_y, int draw_layer = 1);
		Entity(FILE* file);
		virtual ~Entity();
		char* getName();
		double getX();
		double getY();
		virtual float getWidth();
		virtual float getHeight();
		HitboxList* getHitboxes();
		int16_t getDrawLayer();
		double getUpperDrawAxis();
		double getLowerDrawAxis();
		void* getAttr(const char* key);
		uint32_t getType();
		bool isActive();
		bool isVisible();
		bool checkHitboxImmunity(Entity* other, Hitbox* hitbox);

		int addAnimation(Animation* animation);
		int addAnimation(const char* animation_name, uint32_t num_sprite_sets);

		int addFrame(const char* animation_name, unsigned int keytime = 0, unsigned int iter = 1);
		int addSpriteSet(const char* animation_name, const char* sprite_set);
		int addSprite(const char* animation_name, const char* sprite_set, const char* sprite_id, int x_offset = 0, int y_offset = 0, int width = -1, int height = -1);

		int addHitbox(const char* animation_name, HITBOX_SHAPE shape, double x_offset, double y_offset, double z_offset, double x_element, double y_element, double depth, unsigned int type, int sprite_num, int32_t hitbox_group = -1, uint32_t immunity_timer = 0);
		void addHitboxImmunity(Entity* other, Hitbox* hitbox);

		int addSound(const char* animation_name, const char* sound_path, int sequence_num);

		void setX(double x);
		void setY(double y);
		int setAnimation(const char* animation_name);
		int setSize(const char* animation_name, float width, float height);
		void setSize(float width, float height);
		void setActive(bool active);
		void setVisible(bool visible);
		int setSpriteSet(const char* animation_name, const char* sprite_set);
		int setSpriteSet(const char* sprite_set);

		int setUpperDrawAxis(const char* animation_name, double draw_axis, int32_t sprite_num);
		int setUpperDrawAxis(double draw_axis, int32_t sprite_num);

		int setLowerDrawAxis(const char* animation_name, double draw_axis, int32_t sprite_num);
		int setLowerDrawAxis(double draw_axis, int32_t sprite_num);

		//Sets an attribute
		void setAttr(const char* key, bool val);
		void setAttr(const char* key, char val);
		void setAttr(const char* key, int64_t val);
		void setAttr(const char* key, uint64_t val);
		void setAttr(const char* key, char* val);

		//Processing functions
		virtual void _process(uint64_t delta, unsigned int steps) = 0;
		//Need this for custom processing
		virtual void process(uint64_t delta, unsigned int steps) = 0;

		virtual void _action(Control* control) = 0;
		virtual void action(Control* control) = 0;

		virtual void _draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y) = 0;
		virtual void draw(SDL_Renderer* renderer, uint64_t delta, int camera_x, int camera_y) = 0;

		//Call
		int serializeAssets(FILE* file, SerializeSet& serialize_set);
		virtual int serializeExtendedAssets(FILE* file, SerializeSet& serialize_set) = 0;

		int serializeData(FILE* file, Zone* base_zone);
		virtual int serializeExtendedData(FILE* file, Zone* base_zone) = 0;

		int deserializeData(FILE* file);
		virtual int deserializeExtendedData(FILE* file);
		
	protected:
		//Name
		char* name;

		//The entity type (exact values are specified by child classes)
		uint32_t type;

        //Position
        double x;
        double y;
		double z;

		//Attributes
		AttributeHash* attr;

		//If the object is active/visible
		bool active;
		bool visible;

		//Animation
		Animation* active_animation;
		AnimationList* animations;
		uint16_t num_animations;

		//Hitbox group ignoring (key=other object; first tuple=hitbox group; second tuple=timer)
		HitboxImmunityList* hitbox_immunity;

		//Draw layer tracking
		int16_t draw_layer;

		Animation* findAnimation(const char* animation_name);
		void cleanupHitboxImmunity(uint64_t delta);
};

#endif