#ifndef __ENEMIES_H__
#define __ENEMIES_H_

#include "j1Module.h"
#include "p2Point.h"
#include "j1Animation.h"
#include "j1Collisions.h"

struct SDL_Texture;

class j1Enemies : public j1Module
{
public:
	j1Enemies();

	virtual ~j1Enemies();

	bool Awake(pugi::xml_node& conf);

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2);

	bool j1Enemies::IsPointInCircle(float playposX, float playposY, float enemposX, float enemposY, float radi);

public:

	iPoint position;
	void InitialPos();

	uint troll_sound;
	uint troll_death_sound;

	SDL_Texture* graphics = false;
	
	Animation* current_animation;
	Animation idle_right;
	Animation walk_right;
	Animation jump_right;
	Animation attack_right;
	Animation death_right;

	Animation idle_left;
	Animation walk_left;
	Animation jump_left;
	Animation attack_left;
	Animation death_left;

	Collider* enemie_collider;
};


#endif //__ENEMIES_H_
