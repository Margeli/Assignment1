#include "j1Troll.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Player.h"
#include "j1Entity.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1Audio.h"

#define GRAVITY 2
#define TROLL_ATTACK_RANGE 170
#define TROLL_DETECTION_RANGE 350
#define TROLL_SPEED 1.00f
#define ADDED_COLLIDER_WIDTH 10
#define ADDED_COLLIDER_HEIGHT 50
#define COLLIDER_MARGIN_RIGHT 25
#define COLLIDER_MARGIN_LEFT 77
#define TROLL_HEIGHT 100
#define ORIGIN_POSITION 20
#define BOTTOM_SCENE_LIMIT 750

j1Troll::j1Troll(iPoint pos) : j1Entity(EntityTypes::TROLL)		// Should have the initial pos of enemies in a XML
{
	LoadTrollAnimations();	
	position = initial_pos = pos;
}

bool j1Troll::Start() 
{
	bool ret = true;
	collider = App->collis->AddCollider({ position.x, position.y, 66, 50 }, COLLIDER_ENEMIE, App->entities);
	sprites = App->tex->Load("textures/Troll1.png");	
	if (!sprites) { LOG("Error loading troll's textures.");  ret = false; }
	animation = &idle_left;
	LoadTrollAudio();
	SetInitialPos();
	return ret;
}

bool j1Troll::LoadTrollAudio()
{
	bool ret = true;
	troll_death = App->audio->LoadFx("audio/fx/troll_death.wav");
	troll_attack = App->audio->LoadFx("audio/fx/troll_attack.wav");
	if (!troll_death || !troll_attack) { LOG("Error loading troll's audio.");  ret = false; }
	return true;
}

bool j1Troll::IsPointInCircle(float playposX, float playposY, float enemposX, float enemposY, float radi) const
{
	return ((playposX - enemposX)*(playposX - enemposX) + (playposY - enemposY)*(playposY - enemposY)) < radi*radi;
}

void j1Troll::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_GROUND)
	{
		switch (c1->CheckDirection(c2->rect))
		{
		case ENTITY_ABOVE:
			position.y = c2->rect.y - TROLL_HEIGHT;
			break;
		case ENTITY_BELOW:
			position.y = c2->rect.y + c2->rect.h;
			break;
		case ENTITY_RIGHT:
			position.x = c2->rect.x + COLLIDER_MARGIN_RIGHT;
			break;
		case ENTITY_LEFT:
			position.x = c2->rect.x - COLLIDER_MARGIN_LEFT;
			break;
		}
	}
}

void j1Troll::LoadTrollAnimations()		
{
	idle_right.LoadEnemyAnimations("idle_right", "troll");
	idle_left.LoadEnemyAnimations("idle_left", "troll");
	walk_right.LoadEnemyAnimations("walk_right", "troll");
	walk_left.LoadEnemyAnimations("walk_left", "troll");
	jump_right.LoadEnemyAnimations("jump_right", "troll");
	jump_left.LoadEnemyAnimations("jump_left", "troll");
	attack_right.LoadEnemyAnimations("attack_right", "troll");
	attack_left.LoadEnemyAnimations("attack_left", "troll");
	death_right.LoadEnemyAnimations("death_right", "troll");
	death_left.LoadEnemyAnimations("death_left", "troll");
}

bool j1Troll::CleanUp() 
{
	LOG("Unloading Troll.");
	App->tex->UnLoad(sprites);
	collider->to_delete = true;
	return true;
}

bool j1Troll::Update(float dt) 
{
	position.y += GRAVITY;

	if (position.y > BOTTOM_SCENE_LIMIT) { CleanUp(); } //App->audio->PlayFx(troll_death);  App->audio->CleanFx();	
	//If the sound is played, it repeats forever. If I use the function clean fx, all the fx in the game are cleaned and don't sound anymore.

	if (IsPointInCircle(App->entities->player->position.x, App->entities->player->position.y, position.x, position.y, TROLL_DETECTION_RANGE))
	{
		//iPoint origin = { position.x + ORIGIN_POSITION, position.y + ORIGIN_POSITION };
		//iPoint destination = { App->entities->player->position.x + PLAYERWIDTH / 2, App->entities->player->position.y + PLAYERHEIGHT - 40, };
		//path = App->pathfind->FindPath(origin, destination);

		//if (App->entities->player->player_hurted == false && path->path.Count() != 0) { Move(*path); }

		if (App->entities->player->position.x == position.x) //SAME POS X			
		{
		//	if (facing == Facing::LEFT) { animation = &walk_left; }
		//	else if (facing == Facing::RIGHT) { animation = &walk_right; }
		}
		else if (App->entities->player->position.x < position.x)		//MOVE LEFT
		{
			position.x -= TROLL_SPEED;
			animation = &walk_left;
		}
		else if (App->entities->player->position.x > position.x)	//MOVE RIGHT
		{
			position.x += TROLL_SPEED;
			animation = &walk_right;
		}

	
		if (IsPointInCircle(App->entities->player->position.x, App->entities->player->position.y, position.x, position.y, TROLL_ATTACK_RANGE))	 //ATTACK
		{
				 if (App->entities->player->position.x < position.x)
				{
					 App->audio->PlayFx(troll_attack);
					 animation = &attack_left;
				}
				 else if (App->entities->player->position.x > position.x)
				 {
					 App->audio->PlayFx(troll_attack);
					 animation = &attack_right;
				 }
		}
	}
	if (collider != nullptr) { collider->SetPos(position.x + ADDED_COLLIDER_WIDTH, position.y + ADDED_COLLIDER_HEIGHT); }

	if (App->entities->player->hitted == true) { animation = &idle_left; }

	Draw();

	return true;
}

void j1Troll::Move(Pathfinding& _path)
{
	direction = App->pathfind->CheckDirection(_path);

	if (direction == MoveTo::M_DOWN)
	{
		position.y += TROLL_SPEED;
		return;
	}

	if (direction == MoveTo::M_RIGHT)
	{
		position.x += TROLL_SPEED;
		facing = Facing::RIGHT;
		return;
	}
	if (direction == MoveTo::M_LEFT)
	{
		position.x -= TROLL_SPEED;
		facing = Facing::LEFT;
		return;
	}
}


