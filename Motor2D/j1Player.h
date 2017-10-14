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

	void OnCollision(Collider* c1, Collider* c2, CollisionDirection direction);


	void Jump();

public:

	bool camera_movement = false;
	float speed;
	float jump_increment;
	float jump_speed;
	bool landing;
	iPoint position;
	void InitialPos();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;


	SDL_Texture* graphics = nullptr;

	Animation* current_animation = nullptr;
	Animation idle;
	Animation walk;
	Animation jump;
	Animation run;

	Animation idleleft;
	Animation walkleft;
	Animation jumpleft;
	Animation runleft;
	Collider * playercoll;

};


#endif // __j1PLAYER_H__