#include "j1Troll.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Player.h"
#include "j1Entity.h"
#include "j1Scene.h"
#include "j1Scene2.h"

#define GRAVITY 2
#define TROLL_ATTACK_RANGE 170
#define TROLL_DETECTION_RANGE 450
#define TROLL_SPEED 1.00f
#define ADDED_COLLIDER_WIDTH 10
#define ADDED_COLLIDER_HEIGHT 50

j1Troll::j1Troll(iPoint pos) : j1Entity(EntityTypes::TROLL)		// Should have the initial pos of enemies in a XML
{
	LoadTrollAnimations();	
	position = pos;
}

bool j1Troll::Start() 
{
	collider = App->collis->AddCollider({ position.x, position.y, 66, 50 }, COLLIDER_ENEMIE, App->entities);
	sprites = App->tex->Load("textures/Troll1.png");	
	animation = &idle_left;

	return true;
}

bool j1Troll::IsPointInCircle(float playposX, float playposY, float enemposX, float enemposY, float radi) const
{
	return ((playposX - enemposX)*(playposX - enemposX) + (playposY - enemposY)*(playposY - enemposY)) < radi*radi;
}

void j1Troll::OnCollision(Collider* c1, Collider* c2)
{
	int margin = 2;

	if (c2->type == COLLIDER_GROUND)
	{
		switch (c1->CheckDirection(c2->rect))
		{
		case ENTITY_ABOVE:
			position.y = c2->rect.y - 100;
			break;
		case ENTITY_BELOW:
			position.y = c2->rect.y + c2->rect.h;
			break;
		case ENTITY_RIGHT:
			position.x = c2->rect.x + c2->rect.w;
			break;
		case ENTITY_LEFT:
			position.x = c2->rect.x - 100 - margin;
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

	if (IsPointInCircle(App->entities->player->position.x, App->entities->player->position.y, position.x, position.y, TROLL_DETECTION_RANGE) == true)
	{
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

		if (IsPointInCircle(App->entities->player->position.x, App->entities->player->position.y, position.x, position.y, TROLL_ATTACK_RANGE) == true)	//ATTACK 
		{
				 if (App->entities->player->position.x < position.x)
				{
					 animation = &attack_left;
				}
				 else if (App->entities->player->position.x > position.x)
				 {
					 animation = &attack_right;
				 }
		}
	}
	if (collider != nullptr) { collider->SetPos(position.x + ADDED_COLLIDER_WIDTH, position.y + ADDED_COLLIDER_HEIGHT); }
	Draw();

	return true;
}

