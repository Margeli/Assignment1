#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1Animation.h"
#include "j1Collisions.h"

struct SDL_Texture;

class j1Player : public j1Module
{
public:

	j1Player();

	virtual ~j1Player();

	bool Awake(pugi::xml_node& conf);

	bool Start();

	bool Update(float dt);

	bool CleanUp();

public:

	float speed;
	float time;
	iPoint position;

	SDL_Texture* graphics = nullptr;

	Animation* current_animation = nullptr;
	Animation idle;
	Animation walk;
	Animation jump;

	Collider * playercoll;

};


#endif // __j1PLAYER_H__