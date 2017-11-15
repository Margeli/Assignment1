#ifndef __j1ENTITY_H__
#define __j1ENTITY_H__

#include "j1Module.h"
#include "j1EntityManager.h"
#include "p2Point.h"
#include "j1Animation.h"
#include "j1Collisions.h"

struct Collider;
struct Animation;
struct SDL_Texture;
enum EntityTypes;


class j1Entity
{
public:
	j1Entity(EntityTypes type);

	virtual bool Start() {
		return true;
	};
	virtual bool Update(float dt) {
		return true;
	};
	virtual bool PostUpdate() {
		return true;
	};
	virtual bool PreUpdate() {
		return true;
	};
	virtual bool CleanUp() {
		return true;
	};

public:

	EntityTypes type;
	iPoint position;
	iPoint initial_pos;
	Animation* animation = nullptr;
	Collider* collider = nullptr;
	SDL_Texture* sprites = nullptr;
	
	virtual void Draw();
	virtual void OnCollision(Collider* c1, Collider* c2)
	{};
	virtual void SetInitialPos();
	

};

#endif // __j1ENTITY_H__