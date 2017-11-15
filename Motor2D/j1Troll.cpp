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
#define TROLL_DETECTION_RANGE 450
#define TROLL_SPEED 1.00f
#define ADDED_COLLIDER_WIDTH 10
#define ADDED_COLLIDER_HEIGHT 50
#define COLLIDER_MARGIN_RIGHT 24
#define COLLIDER_MARGIN_LEFT 77
#define TROLL_HEIGHT 100

j1Troll::j1Troll(iPoint pos) : j1Entity(EntityTypes::TROLL)		// Should have the initial pos of enemies in a XML
{
	LoadTrollAnimations();	
	position = pos;
}

bool j1Troll::Start() 
{
	bool ret = true;
	collider = App->collis->AddCollider({ position.x, position.y, 66, 50 }, COLLIDER_ENEMIE, App->entities);
	sprites = App->tex->Load("textures/Troll1.png");	
	if (!sprites) { LOG("Error loading troll's textures.");  ret = false; }
	animation = &idle_left;
	LoadTrollAudio();
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

void j1Troll::LoadTrollAnimations()		//FIX ANIMATIONS SIZE (ATTACK CHANGES ITS POSITION)
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

	if (position.y > 800) { App->audio->PlayFx(troll_death);  App->audio->CleanFx();  CleanUp(); }	

	

	if (IsPointInCircle(App->entities->player->position.x, App->entities->player->position.y, position.x, position.y, TROLL_DETECTION_RANGE))
	{
		//troll_path = App->pathfind->FindPath(position, App->entities->player->position);
		//App->pathfind->DrawPath(*troll_path);

		if (App->entities->player->position.x == position.x) //SAME POSITION IN X			
		{
			if(App->entities->player->LEFT) 		animation = &idle_left;
			if(App->entities->player->RIGHT)		animation = &idle_right;		//TROLL SHOULD LOOK RIGHT  //TODO 
		}

		else if (App->entities->player->position.x < position.x)	//WALK LEFT
			{
				position.x -= TROLL_SPEED;
				animation = &walk_left;
			}
		else if (App->entities->player->position.x > position.x)	//WALK RIGHT
			{
				position.x += TROLL_SPEED;
				animation = &walk_right;
			}

		if (IsPointInCircle(App->entities->player->position.x, App->entities->player->position.y, position.x, position.y, TROLL_ATTACK_RANGE))	//ATTACK 
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
	Draw();

	return true;
}

