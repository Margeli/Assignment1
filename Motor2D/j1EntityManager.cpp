#include "j1EntityManager.h"
#include "j1Player.h"
#include "j1FlyingEnemie.h"
#include "j1Troll.h"

j1EntityManager::j1EntityManager()
{}

j1EntityManager::~j1EntityManager()
{}

j1Entity* j1EntityManager::CreateEntity(EntityTypes type) {

	j1Entity* ret = nullptr;
	switch (type) {
	case PLAYER: ret = new j1Player(); break;
	case FLY: ret = new j1FlyingEnemie(); break;
	case TROLL: ret = new j1Troll(); break;
	}
	if (ret != nullptr)
		entities.add(ret);
	return ret;
}


bool j1EntityManager::Start()
{
	p2List_item<j1Entity*>* entity_iterator;
	for (entity_iterator = entities.start; entity_iterator; entity_iterator = entity_iterator->next) {
		entity_iterator->data->Start();
	}
	return true;
}

bool j1EntityManager::PreUpdate()
{
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