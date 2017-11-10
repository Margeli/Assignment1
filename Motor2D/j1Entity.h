#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "j1EntityManager.h"
#include "p2Point.h"

enum EntityTypes {
	NO_TYPE,
	PLAYER,
	TROLL,
	FLY
};


class j1Entity : public j1EntityManager
{
public:
	j1Entity(EntityTypes type);
	~j1Entity();

	virtual bool Start();
	virtual bool Update(float dt);
	virtual bool PostUpdate();
	virtual bool PreUpdate();
	virtual bool CleanUp();

	EntityTypes type = NO_TYPE;
	iPoint position;
	Animation* animation = nullptr;
	Collider* collider = nullptr;
	SDL_Texture* sprite_path = nullptr;
	
	virtual void Draw();
	virtual void OnCollision(Collider* c1, Collider* c2);


};

#endif // __j1ENTITY_H__