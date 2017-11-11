/*
#include "j1Enemies.h"
#include "j1Enemy.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "p2Log.h"
#include "j1Troll.h"
#include "j1FlyingEnemie.h"


#define SPAWN_MARGIN 200


j1Enemies::j1Enemies()
{
	name.create("enemies");

	for (uint i = 0; i < MAX_ENEMIES; ++i)
		enemies[i] = nullptr;
}

// Destructor
j1Enemies::~j1Enemies()
{}

bool j1Enemies::Start()
{
	// Create a prototype for each enemy available so we can copy them around
	//sprites = App->textures->Load("Assets/Images/Bonus_Spaceship.png");
	//sprites = App->textures->Load("Assets/Images/Green_Shooter.png");
	//sprites = App->textures->Load("Assets/Images/Light_Shooter.png");
	
	//if (sprites == nullptr) {
	//LOG("Error loading enemy's sprites.");
	//}

	return true;
}

bool j1Enemies::PreUpdate()
{
	bool ret = true;
	
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type != ENEMY_TYPES::NO_TYPE) // need limit spawn depending on camera position 
		{
			SpawnEnemy(queue[i]);
			queue[i].type = ENEMY_TYPES::NO_TYPE;
			LOG("Spawning enemy at %d", queue[i].x);
		}
	}

	return ret;
}

// Called before render is available
bool j1Enemies::Update(float dt)
{
	bool ret = true;
	for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr) enemies[i]->Move();

	for (uint i = 0; i < MAX_ENEMIES; ++i)
		if (enemies[i] != nullptr) {
			enemies[i]->Draw(enemies[i]->sprite_path);
		}

	return ret;
}

bool j1Enemies::PostUpdate()
{

	bool ret = true;
	// check camera position to decide what to spawn
	//for (uint i = 0; i < MAX_ENEMIES; ++i)
	//{
	//	if (enemies[i] != nullptr)
	//	{
	//		if (App->render->camera.y + (-SCREEN_HEIGHT - SPAWN_MARGIN)*SCREEN_SIZE>-enemies[i]->position.y*SCREEN_SIZE)
	//		{
	//			LOG("DeSpawning enemy at %d", enemies[i]->position.y);
	//			delete enemies[i];
	//			enemies[i] = nullptr;
	//		}
	//	}
	//}

	return ret;
}

// Called before quitting
bool j1Enemies::CleanUp()
{
	LOG("Freeing all enemies");

	 // need to unload all enemy tex
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
			delete enemies[i];
			enemies[i] = nullptr;
		}
	}

	return true;
}

bool j1Enemies::AddEnemy(ENEMY_TYPES type, int x, int y)
{
	bool ret = false;

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (queue[i].type == ENEMY_TYPES::NO_TYPE)
		{
			queue[i].type = type;
			queue[i].x = x;
			queue[i].y = y;
			
			ret = true;
			break;
		}
	}

	return ret;
}

void j1Enemies::SpawnEnemy(const EnemyInfo& info)
{
	// find room for the new enemy
	uint i = 0;
	for (; enemies[i] != nullptr && i < MAX_ENEMIES; ++i);

	if (i != MAX_ENEMIES)
	{
		switch (info.type)
		{
		case ENEMY_TYPES::TROLL:
			enemies[i] = new j1Troll(info.x, info.y);
			break;

		case ENEMY_TYPES::FLYING:
			enemies[i] = new j1FlyingEnemie(info.x, info.y);
			break;	

		}
	}
}

void j1Enemies::OnCollision(Collider* c1, Collider* c2)
{
	/*for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr && enemies[i]->GetCollider() == c1)
		{
			enemies[i]->OnCollision(c2, i);


			break;
		}
	}
}*/
