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

enum RESOURCE_TYPE{
	BMP,
	SOUND,
	MUSIC,
	CUT
};

class Zone;

class Entity{
	public:
		Entity(const char* name, float start_x, float start_y, int draw_layer = 1);
		virtual ~Entity();
		char* getName();
		double getX();
		double getY();
		float getWidth();
		float getHeight();
		HitboxList* getHitboxes();
		int getDrawLayer();
		double getDrawAxis();
		void* getAttr(const char* key);
		uint16_t getType();
		bool isActive();
		bool isVisible();
		bool checkHitboxImmunity(Entity* other, Hitbox* hitbox);

		int addAnimation(const char* animation_name, uint32_t num_sprite_sets);

		int addAnimationFrame(const char* animation_name, unsigned int keytime = 0, unsigned int iter = 1);
		int addSpriteSet(const char* animation_name, const char* sprite_set);
		int addSprite(const char* animation_name, const char* sprite_path, const char* sprite_set, int x_offset = 0, int y_offset = 0, int width = -1, int height = -1);

		int addHitbox(const char* animation_name, HITBOX_SHAPE shape, double x_offset, double y_offset, double x_element, double y_element, unsigned int type, int sprite_num, int32_t hitbox_group = -1, uint32_t immunity_timer = 0);
		void addHitboxImmunity(Entity* other, Hitbox* hitbox);

		int addSound(const char* animation_name, const char* sound_path, int sequence_num);

		void setX(double x);
		void setY(double y);
		int setAnimation(const char* animation_name);
		int setScale(const char* animation_name, double x_scale, double y_scale);
		void setScale(double x_scale, double y_scale);
		int setSize(const char* animation_name, float width, float height);
		void setSize(float width, float height);
		void setActive(bool active);
		void setVisible(bool visible);
		void setAttr(const char* key, void* val);
		int setSpriteSet(const char* animation_name, const char* sprite_set);
		int setSpriteSet(const char* sprite_set);

		//Processing functions
		virtual void _process(uint32_t delta) = 0;
		//Need this for custom processing
		virtual void process(uint32_t delta) = 0;

		virtual void _action(Control* control) = 0;
		virtual void action(Control* control) = 0;

		virtual void _draw(SDL_Renderer* renderer, uint32_t delta, int camera_x, int camera_y) = 0;
		virtual void draw(SDL_Renderer* renderer, uint32_t delta, int camera_x, int camera_y) = 0;

		//Call
		int serializeAssets(FILE* file, std::unordered_set<std::string>& sprite_set, std::unordered_set<std::string>& audio_set, std::unordered_set<std::string>& music_set);
		virtual int serializeExtendedAssets(FILE* file, std::unordered_set<std::string>& sprite_set, std::unordered_set<std::string>& audio_set, std::unordered_set<std::string>& music_set);

		int serializeData(FILE* file, Zone* base_zone);
		virtual int serializeExtendedData(FILE* file, Zone* base_zone);
		
	protected:
		//Name
		char* name;

		//The entity type (exact values are specified by child classes)
		uint16_t type;

        //Position
        double x;
        double y;

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
		int draw_layer;

		Animation* findAnimation(const char* animation_name);
		void cleanupHitboxImmunity(uint32_t delta);
};

#endif