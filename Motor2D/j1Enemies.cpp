#include "j1Enemies.h"
#include "p2Defs.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Animation.h"
#include "j1Audio.h"

j1Enemies::j1Enemies()
{
	name.create("Troll1");
	graphics = nullptr;

	idle_right.PushBack({ 0, 0, 0, 0 });
	idle_right.loop = true;
	idle_right.speed = 0.07f;

	idle_left.PushBack({ 0, 0, 0, 0 });
	idle_left.loop = true;
	idle_left.speed = 0.07f;

	walk_right.PushBack({ 0, 0, 0, 0});
	walk_right.loop = true;
	walk_right.speed = 0.07f;

	walk_left.PushBack({ 0, 0, 0, 0 });
	walk_left.loop = true;
	walk_left.speed = 0.07f;

	jump_right.PushBack({ 0, 0, 0, 0 });
	jump_right.loop = true;
	jump_right.speed = 0.07f;

	jump_left.PushBack({ 0, 0, 0, 0 });
	jump_left.loop = true;
	jump_left.speed = 0.07f;

	attack_right.PushBack({ 0, 0, 0, 0 });
	attack_right.loop = true;
	attack_right.speed = 0.07f;

	attack_left.PushBack({ 0, 0, 0, 0 });
	attack_left.loop = true;
	attack_left.speed = 0.07f;

	death_right.PushBack({ 0, 0, 0, 0 });
	death_right.loop = false;
	death_right.speed = 0.07f;

	death_left.PushBack({ 0, 0, 0, 0 });
	death_left.loop = false;
	death_left.speed = 0.07f;
}


j1Enemies::~j1Enemies()
{
}

bool j1Enemies::Awake(pugi::xml_node& conf)
{
	return true;
}

bool j1Enemies::Start()
{
	bool ret = true;

	graphics = App->tex->Load("textures/Troll1.png");
	enemie_collider = App->collis->AddCollider({ position.x, position.y, 0, 0 }, COLLIDER_ENEMIE, this);

	if (!graphics) { LOG("Error loading Troll 1 sprites.");  ret = false; }

	current_animation = &idle_right;

	if (troll_sound == 0)
		troll_sound = App->audio->LoadFx("audio/fx/troll_attack.wav");

	if (troll_death_sound == 0)
		troll_death_sound = App->audio->LoadFx("audio/fx/troll_death.wav");

	return ret;
}

bool j1Enemies::CleanUp()
{
	LOG("Unloading Troll 1.");

	App->tex->UnLoad(graphics);

	if (enemie_collider != nullptr)
	{
		enemie_collider->to_delete = true;
	}

	return true;
}

bool j1Enemies::Update(float dt)
{
	//IA
	//Pathfinding applied to platformers.
	
	return true;
}

void j1Enemies::OnCollision(Collider* c1, Collider* c2, CollissionDirectionEnemie direction)
{
	int margin = 0;
	switch (c2->type) 
	{
	case COLLIDER_GROUND:

		switch (direction)
		{
		case ENEMIE_ABOVE:
			position.y = c2->rect.y - 65 - margin;
			break;
		case ENEMIE_BELOW:
			position.y = c2->rect.y + c2->rect.h + margin;
			break;
		case ENEMIE_RIGHT:
			position.x = c2->rect.x + c2->rect.w + margin;
			break;
		case ENEMIE_LEFT:
			position.x = c2->rect.x - 46 - margin;
			break;
		}
		break;

	case COLLIDER_WALL://CHANGE PARAMETERS

		switch (direction)
		{
		case ENEMIE_ABOVE:
			position.y = c2->rect.y - 65 - margin;					
			break;
		case ENEMIE_BELOW:
			position.y = c2->rect.y + c2->rect.h + margin;
			break;
		case ENEMIE_RIGHT:
			position.x = c2->rect.x + c2->rect.w + margin;
			break;
		case ENEMIE_LEFT:
			position.x = c2->rect.x - 46 - margin;
			break;
		}
	case COLLIDER_PLAYER://CHANGE PARAMETERS

		switch (direction)
		{
		case ENEMIE_ABOVE:
			position.y = c2->rect.y - 65 - margin;
			break;
		case ENEMIE_BELOW:
			position.y = c2->rect.y + c2->rect.h + margin;
			break;
		case ENEMIE_RIGHT:
			position.x = c2->rect.x + c2->rect.w + margin;
			break;
		case ENEMIE_LEFT:
			position.x = c2->rect.x - 46 - margin;
			break;
		}

		break;

		break;
	}
}

void j1Enemies::Landing()
{
}

void j1Enemies::initialpos()
{
	position = { 100, 100 };
}