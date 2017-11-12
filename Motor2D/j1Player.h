#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1Animation.h"
#include "j1Collisions.h"
#include "j1Entity.h"

struct SDL_Texture;

#define SPEED 1
#define LIFES 1
#define PLAYERHEIGHT 65
#define PLAYERWIDTH 45

class j1Player : public j1Entity
{
public:

	j1Player();
	virtual ~j1Player();

	bool Awake(pugi::xml_node& conf);
	bool Start();
	bool Update(float dt);
	bool CleanUp();
	void OnCollision(Collider* c1, Collider* c2);
	void Dead();
	void PlayerHurted();
	void LoseOneLife();

	enum Player_Facing
	{
		RIGHT = 0,
		LEFT
	};

private:

	Player_Facing facing = RIGHT;

	float jump_limit;
	float jump_pos;
	float jump_speed;	
	bool walking = false;
	bool landing = false;
	bool can_jump = false;
	bool jumping = false;
	bool double_jump = true;
	void JumpReset();
	bool godmode = false;
	void LoadPlayerAnimations();

	uint jump_sound;
	uint sword_sound;
	uint playersteps;
	uint lose_fx;
	uint hurt_fx;
	uint die_fx;

public:

	int lifes;
	int points;
	int max_score = 0;
	float speed;

	Uint32 hit_time;

	bool camera_movement = false;
	bool use_input = true;
	bool dead = false;
	bool hitted = false;
	bool player_hurted = false;

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	Animation idle_right;
	Animation walk_right;
	Animation jump_right;
	Animation run_right;
	Animation attack_right;
	Animation death_right;

	Animation idle_left;
	Animation walk_left;
	Animation jump_left;
	Animation run_left;
	Animation attack_left;
	Animation death_left;
};


#endif // __j1PLAYER_H__