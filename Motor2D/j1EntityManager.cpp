#include "j1EntityManager.h"
#include "j1Player.h"
#include "j1FlyingEnemy.h"
#include "j1Troll.h"
#include "j1Collectables.h"
#include "Brofiler/Brofiler.h"

j1EntityManager::j1EntityManager()
{
	name.create("entities");
}

j1EntityManager::~j1EntityManager()
{}

j1Entity* j1EntityManager::CreateEntity(EntityTypes type, iPoint position)
{
	j1Entity* ret = nullptr;
	switch (type)
	{
	case PLAYER: ret = new j1Player();
		if (ret != nullptr)
			entities.add(ret);
		break;
	case FLY: AddtoSpawningQueue(position, FLY);
		break;

	case TROLL: AddtoSpawningQueue(position, TROLL);
		break;
	
	case COLLECT: AddtoSpawningQueue(position, COLLECT);
		break;

	}
	return ret;
}

void j1EntityManager::DestroyEntity(j1Entity* entity)
{
	int num = entities.find(entity);
	RELEASE(entities.At(num)->data);
	entities.del(entities.At(num));
}

bool j1EntityManager::Start()
{
	if (first_loop) { player = (j1Player*)CreateEntity(PLAYER); first_loop = false; }

	p2List_item<j1Entity*>* entity_iterator;
	for (entity_iterator = entities.start; entity_iterator!=NULL; entity_iterator = entity_iterator->next) {
		entity_iterator->data->Start();
	}
	return true;
}

bool j1EntityManager::PreUpdate()
{
	CheckPlayerPostoSpawn();	// Spawn enemies (TROLL|FLY) depending on player pos
	BROFILER_CATEGORY("EntityManagerPreUpdate", Profiler::Color::Bisque);
	p2List_item<j1Entity*>* entity_iterator; 
	for (entity_iterator = entities.start; entity_iterator!=NULL; entity_iterator = entity_iterator->next)
	{entity_iterator->data->PreUpdate();}
	return true;
}

bool j1EntityManager::Update(float dt)
{
	BROFILER_CATEGORY("EntityManagerUpdate", Profiler::Color::Bisque);
	p2List_item<j1Entity*>* entity_iterator;
	for (entity_iterator = entities.start; entity_iterator!=NULL; entity_iterator = entity_iterator->next)
	{entity_iterator->data->Update(dt);}
	return true;
}

bool j1EntityManager::PostUpdate()
{
	CheckPlayerPostoDespawn(); // Despawn enemies (TROLL|FLY|COLLECT) depending on player pos
	BROFILER_CATEGORY("EntityManagerPostUpdate", Profiler::Color::Bisque);
	p2List_item<j1Entity*>* entity_iterator;
	for (entity_iterator = entities.start; entity_iterator!=NULL; entity_iterator = entity_iterator->next)
	{entity_iterator->data->PostUpdate();}
	return true;
}
bool j1EntityManager::CleanUp()
{
	SpawnListReset();
	p2List_item<j1Entity*>* entity_iterator;
	for (entity_iterator = entities.start; entity_iterator!=NULL; entity_iterator = entity_iterator->next)
	{entity_iterator->data->CleanUp();}
	return true;
}

void j1EntityManager::OnCollision(Collider* c1, Collider* c2) 
{
	p2List_item<j1Entity*>* entity_iterator;
	for (entity_iterator = entities.start; entity_iterator!=NULL; entity_iterator = entity_iterator->next)
	{
		if (entity_iterator->data->collider == c1)
		{
			entity_iterator->data->OnCollision(c1, c2);
			break;
		}
	}
}

void  j1EntityManager::AddtoSpawningQueue(iPoint pos, EntityTypes ent_type) 
{
	for (uint i = 0; i < MAX_ENTITIES -1; ++i)
	{
		if (to_spawn[i].type == EntityTypes::NOTYPE) 
		{
			to_spawn[i].type = ent_type;
			to_spawn[i].pos = pos;	
			return;
		}
	}
}

void  j1EntityManager::CheckPlayerPostoSpawn() 
{
	for (uint i = 0; i < MAX_ENTITIES-1; ++i)
	{
		if (to_spawn[i].type != EntityTypes::NOTYPE && player->position.x+SPAWN_MARGIN>=to_spawn[i].pos.x) // need limit spawn depending on camera position 
		{
			j1Entity* entity;
			if (to_spawn[i].type== FLY) { entity = new j1FlyingEnemy(to_spawn[i].pos); }

			else if (to_spawn[i].type == TROLL) { entity = new j1Troll(to_spawn[i].pos); }

			else if (to_spawn[i].type == COLLECT) { entity = new j1Collectables(to_spawn[i].pos); }

			entities.add(entity);
			entity->Start();
			to_spawn[i].type = EntityTypes::NOTYPE;			
		}
	}
}

void  j1EntityManager::CheckPlayerPostoDespawn()
{
	p2List_item<j1Entity*>* entity_iterator;
	for (entity_iterator = entities.start; entity_iterator; entity_iterator = entity_iterator->next)
	{
		if ((entity_iterator->data->type == TROLL) || (entity_iterator->data->type == FLY) || (entity_iterator->data->type == COLLECT))
		{
			if (entity_iterator->data->position.x + SPAWN_MARGIN < player->position.x)
			{
				entity_iterator->data->CleanUp();
				DestroyEntity(entity_iterator->data);
			}
		}
	}	
}	

bool j1EntityManager::Load(pugi::xml_node& data ) 
{
	SpawnListReset();
	EnemiesCleanUp();
	player->Load(data.child(player->name.GetString()));// player loading

	
	for (pugi::xml_node trolls = data.child("troll").child("position"); trolls; trolls = trolls.next_sibling()) {
		iPoint trollpos = { trolls.attribute("x").as_int(), trolls.attribute("y").as_int() };
		CreateEntity(TROLL, trollpos);
	}
	for (pugi::xml_node flies = data.child("fly").child("position"); flies; flies= flies.next_sibling()) {
		iPoint flypos = { flies.attribute("x").as_int(), flies.attribute("y").as_int() };
		CreateEntity(FLY, flypos);
	}
	for (pugi::xml_node pickups = data.child("collect").child("position"); pickups; pickups = pickups.next_sibling()) {
		iPoint pickuppos = { pickups.attribute("x").as_int(), pickups.attribute("y").as_int() };
		CreateEntity(COLLECT, pickuppos);
	}

	return true;
}

bool j1EntityManager::Save(pugi::xml_node& data) const
{
	player->Save(data.append_child(player->name.GetString()));

	pugi::xml_node trolls = data.append_child("troll");
	pugi::xml_node flies = data.append_child("fly");
	pugi::xml_node pickups = data.append_child("collect");

	p2List_item<j1Entity*>* entity_iterator;
	for (entity_iterator = entities.start; entity_iterator; entity_iterator = entity_iterator->next)//iterates over all enemies spawned
	{
		if (entity_iterator->data->type == TROLL) 
		{
			entity_iterator->data->Save(trolls);
		}
		if (entity_iterator->data->type == FLY)
		{
			entity_iterator->data->Save(flies);
		}
		if (entity_iterator->data->type == COLLECT)
		{
			entity_iterator->data->Save(pickups);
		}
	}

	for (int i = 0; i < MAX_ENTITIES - 1; ++i) { //iterates over all enemies on the spawning queue
		if (to_spawn[i].type != EntityTypes::NOTYPE) {
			if (to_spawn[i].type == TROLL) {
				pugi::xml_node troll_pos = trolls.append_child("position");
				troll_pos.append_attribute("x") = to_spawn[i].pos.x;
				troll_pos.append_attribute("y") = to_spawn[i].pos.y;			
			}
			if (to_spawn[i].type == FLY) {
				pugi::xml_node fly_pos = flies.append_child("position");
				fly_pos.append_attribute("x") = to_spawn[i].pos.x;
				fly_pos.append_attribute("y") = to_spawn[i].pos.y;
			}
			if (to_spawn[i].type == COLLECT) {
				pugi::xml_node pick_pos = pickups.append_child("position");
				pick_pos.append_attribute("x") = to_spawn[i].pos.x;
				pick_pos.append_attribute("y") = to_spawn[i].pos.y;
			}
		}
	}

	return true;
}

bool j1EntityManager::EnemiesCleanUp() {
	SpawnListReset();
	p2List_item<j1Entity*>* entity_iterator;
	for (entity_iterator = entities.start; entity_iterator; entity_iterator = entity_iterator->next) {
		if (entity_iterator->data->type == EntityTypes::TROLL || entity_iterator->data->type == EntityTypes::FLY || entity_iterator->data->type == EntityTypes::COLLECT) {
			entity_iterator->data->CleanUp();
			DestroyEntity(entity_iterator->data);
		}
	}

	return true;
}

void j1EntityManager::SetInitialPos()  // Sets the enemies to their initial position
{
	p2List_item<j1Entity*>* entity_iterator;
	for (entity_iterator = entities.start; entity_iterator; entity_iterator = entity_iterator->next) 
	{
		if (entity_iterator->data->type != EntityTypes::PLAYER) { entity_iterator->data->SetInitialPos(); }
	}
}


void j1EntityManager::SpawnListReset() {
	for (int i = 0; i < MAX_ENTITIES; i++) {
		to_spawn[i].type = EntityTypes::NOTYPE;			
	}

}