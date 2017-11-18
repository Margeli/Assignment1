#include "j1Collectables.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1EntityManager.h"
#include "j1Player.h"
#include "j1Entity.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1Scene2.h"

#define PICKUP_SIZE 32

j1Collectables::j1Collectables(iPoint pos) : j1Entity(EntityTypes::COLLECT)
{
	LoadAnimation();
	position = initial_pos = pos;
	collider = nullptr;
	sprites = nullptr;
}

bool j1Collectables::Start()
{
	bool ret = true;
	collider = App->collis->AddCollider({ position.x, position.y, 31, 32 }, COLLIDER_PICKUP, App->entities);
	sprites = App->tex->Load("textures/banana.png");
	if (!sprites) { LOG("Error loading  textures.");  ret = false; }
	LoadAudio();
	SetInitialPos();
	return ret;
}

bool j1Collectables::LoadAudio()
{
	bool ret = true;
	pick_up = App->audio->LoadFx("audio/fx/coin.wav");

	if (!pick_up) { LOG("Error loading audio.");  ret = false; }
	return ret;
}

void j1Collectables::LoadAnimation()
{
	idle.LoadEnemyAnimations("idle", "collect");
	found_anim.LoadEnemyAnimations("found", "collect");
}

void j1Collectables::OnCollision(Collider* c1, Collider* c2)
{
	CollisionDirection direction;
	if (c2->type == COLLIDER_PLAYER)
	{
		App->entities->player->points += 50;
		found = true;
		collider->to_delete = true;
	}
	else if (c2->type == COLLIDER_GROUND)
	{
		direction = c1->CheckDirection(c2->rect);
		if (direction == ENTITY_ABOVE)
		{
			position.y = c2->rect.y - PICKUP_SIZE;
		}
		else if (direction == ENTITY_BELOW)
		{
			position.y = c2->rect.y + c2->rect.h;
		}
		else if (direction == ENTITY_RIGHT)
		{
			position.x = c2->rect.x + c2->rect.w;
		}
		else if (direction == ENTITY_LEFT)
		{
			position.x = c2->rect.x - PICKUP_SIZE;
		}
	}
}

bool j1Collectables::CleanUp()
{
	LOG("Unloading collectable.");
	App->tex->UnLoad(sprites);
	collider->to_delete = true;
	return true;
}

bool j1Collectables::Update(float dt)
{
	position.y += GRAVITY;

	if (found == true) { App->audio->PlayFx(pick_up); animation = &found_anim; 	found = false; }
	else 	animation = &idle;
	if (collider != nullptr) { collider->SetPos(position.x, position.y); }
	Draw();
	return true;
}

bool j1Collectables::Save(pugi::xml_node& data) const
{
	pugi::xml_node pos = data.append_child("position");

	pos.append_attribute("x") = position.x;
	pos.append_attribute("y") = position.y;

	return true;
}

bool j1Collectables::Load(pugi::xml_node&) { return true; }