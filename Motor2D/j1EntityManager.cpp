#include "j1EntityManager.h"
#include "j1Player.h"
#include "j1FlyingEnemy.h"
#include "j1Troll.h"

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
	}
	return ret;
}

void j1EntityManager::DestroyEntity(j1Entity* entity){
	int num = entities.find(entity);
	entities.del(entities.At(num));
}

bool j1EntityManager::Start()
{
	player = (j1Player*)CreateEntity(PLAYER);

	p2List_item<j1Entity*>* entity_iterator;
	for (entity_iterator = entities.start; entity_iterator; entity_iterator = entity_iterator->next) {
		entity_iterator->data->Start();
	}
	return true;
}

bool j1EntityManager::PreUpdate()
{
	CheckPlayerPostoSpawn();// Spawn enemies (TROLL|FLY) depending on player pos

	p2List_item<j1Entity*>* entity_iterator; 
	for (entity_iterator = entities.start; entity_iterator; entity_iterator = entity_iterator->next) {
		entity_iterator->data->PreUpdate();
	}
	return true;
}

bool j1EntityManager::Update(float dt)
{
	p2List_item<j1Entity*>* entity_iterator;
	for (entity_iterator = entities.start; entity_iterator; entity_iterator = entity_iterator->next) {
		entity_iterator->data->Update(dt);
	}
	return true;

}
bool j1EntityManager::PostUpdate()
{
	CheckPlayerPostoDespawn(); // Despawn enemies (TROLL|FLY) depending on player pos

	p2List_item<j1Entity*>* entity_iterator;
	for (entity_iterator = entities.start; entity_iterator; entity_iterator = entity_iterator->next) {
		entity_iterator->data->PostUpdate();
	}
	return true;

}
bool j1EntityManager::CleanUp()
{
	p2List_item<j1Entity*>* entity_iterator;
	for (entity_iterator = entities.start; entity_iterator; entity_iterator = entity_iterator->next) {
		entity_iterator->data->CleanUp();
	}
	return true;
}

void j1EntityManager::OnCollision(Collider* c1, Collider* c2) 
{
	p2List_item<j1Entity*>* entity_iterator;
	for (entity_iterator = entities.start; entity_iterator; entity_iterator = entity_iterator->next) {
		if (entity_iterator->data->collider == c1) {
			entity_iterator->data->OnCollision(c1, c2);
			break;
		}
	}

}

void  j1EntityManager::AddtoSpawningQueue(iPoint pos, EntityTypes t) {

	for (uint i = 0; i < MAX_ENTITIES -1; ++i)
	{
		if (to_spawn[i].type == EntityTypes::NOTYPE) 
		{
			to_spawn[i].type = t;
			to_spawn[i].pos = pos;	
			return;
		}
	}

}

void  j1EntityManager::CheckPlayerPostoSpawn() {

	for (uint i = 0; i < MAX_ENTITIES-1; ++i)
	{
		if (to_spawn[i].type != EntityTypes::NOTYPE && player->position.x+SPAWN_MARGIN>=to_spawn[i].pos.x) // need limit spawn depending on camera position 
		{
			j1Entity* enemy;
			if (to_spawn[i].type== FLY) {
				enemy = new j1FlyingEnemy(to_spawn[i].pos);
			}
			else if (to_spawn[i].type == TROLL) {
				enemy = new j1Troll(to_spawn[i].pos);
			}
			entities.add(enemy);
			enemy->Start();
			to_spawn[i].type = EntityTypes::NOTYPE;			
		}
	}
}

void  j1EntityManager::CheckPlayerPostoDespawn() {

	p2List_item<j1Entity*>* entity_iterator;
	for (entity_iterator = entities.start; entity_iterator; entity_iterator = entity_iterator->next) {
		if ((entity_iterator->data->type == TROLL) || (entity_iterator->data->type == FLY)) {
			if (entity_iterator->data->position.x + SPAWN_MARGIN < player->position.x) {
				entity_iterator->data->CleanUp();
				DestroyEntity(entity_iterator->data);
			}
		}
	}	
}	

bool j1EntityManager::Load(pugi::xml_node& data ) {
	player->Load(data.child(player->name.GetString()));
	return true;
}
bool j1EntityManager::Save(pugi::xml_node& data) const {
	
	player->Save(data.append_child(player->name.GetString()));
	return true;
}