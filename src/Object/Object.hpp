typedef struct ObjectList{
	Object* obj;
	Object* next;
} ObjLst;

class Object{
	public:
		Object
		float getX();
		float getY();
		
	protected:
		//Position
		float x;
		float y;

		//Velocity
		float xV;
		float yV;

		//Acceleration
		float xA;
		float yA;

		//Coefficient of friction
		float friction;

		//Hitboxes
		Hitbox* hitboxes;

		//Animation
		Animation* animation;
}
