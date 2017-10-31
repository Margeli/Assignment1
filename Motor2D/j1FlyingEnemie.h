#ifndef __FLYINGENEMIE_H__
#define __FLYINGENEMIE_H_

#include "j1Module.h"
#include "p2Point.h"
#include "j1Animation.h"
#include "j1Collisions.h"

struct SDL_Texture;

class j1FlyingEnemie : public j1Module
{
public:
	j1FlyingEnemie();

	virtual ~j1FlyingEnemie();

	bool Awake(pugi::xml_node& conf);

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2);



public:

	iPoint position;
	void initialpos();

	uint fly_sound;

	SDL_Texture* graphics = false;

	Animation* current_animation;

	Animation fly_right;
	Animation fly_left;

	Collider* enemie_collider;
};


#endif //__FLYINGENEMIE_H_
