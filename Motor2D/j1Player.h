#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"
#include "p2Point.h"
#include "j1Animation.h"
#include "j1Collisions.h"
#include "j1Entity.h"

struct SDL_Texture;
class GuiImage;

#define PLAYERHEIGHT 69
#define PLAYERWIDTH 45
#define LIFES 5

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
	void LittleJump();

	int lifes;
	int points = 0;
	int max_score = 0;
	

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

	p2SString name;

private:

	Facing facing = Facing::RIGHT;

	void InitialPlayerPos();

	float jump_limit;
	float jump_pos;
	float jump_speed;
	bool walking = false;
	bool landing = false;
	bool can_jump = false;
	bool jumping = false;
	bool double_jump = true;
	void JumpReset();
	bool littlejump = false;
	uint littlejumphigh;
	bool godmode = false;
	void LoadPlayerAnimations();
	void AddPlayerGui();
	void UpdatePlayerGui();

	uint jump_sound;
	uint sword_sound;
	uint playersteps;
	uint lose_fx;
	uint hurt_fx;
	uint die_fx;
	uint troll_death;

	GuiImage* full_heart[LIFES];
	GuiImage* empty_heart[LIFES];
};


#endif // __j1PLAYER_H__