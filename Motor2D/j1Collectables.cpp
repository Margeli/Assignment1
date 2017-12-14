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
	animation = &idle;
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
	
}

void j1Collectables::OnCollision(Collider* c1, Collider* c2)
{
	CollisionDirection direction;
	if (c2->type == COLLIDER_PLAYER)
	{
		App->entities->player->points += 20;
		found = true;
		collider->to_delete = true;
		App->entities->player->pickups_counter++;
	}
	
}

bool j1Collectables::CleanUp()
{
	LOG("Unloading collectable.");
	App->tex->UnLoad(sprites);
	if (App->collis->CollidersNum() != 0)
		collider->to_delete = true;
	
	return true;
}

bool j1Collectables::Update(float dt)
{

	BROFILER_CATEGORY("EntityFLYUpdate", Profiler::Color::Bisque);
	

	if (found == true) { 
		App->audio->PlayFx(pick_up); 		
		CleanUp();
		App->entities->DestroyEntity(this);
		return true;
	}
	
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