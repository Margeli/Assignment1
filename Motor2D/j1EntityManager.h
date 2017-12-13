#ifndef __j1ENTITIES_H__
#define __j1ENTITIES_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "j1Entity.h"
#include "p2Defs.h"

#define MAX_ENTITIES 50
#define SPAWN_MARGIN 2000

enum EntityTypes
{
	NOTYPE,
	PLAYER,
	TROLL,
	TROLL2,
	TROLL3,
	FLY,
	COLLECT
};

class j1Player;
class j1Entity;

class j1EntityManager : public j1Module
{
public:
	j1EntityManager();
	virtual ~j1EntityManager();
	void Init();
	bool Start();
	bool Update(float dt);
	bool PostUpdate();
	bool PreUpdate();
	bool CleanUp();
	bool first_loop = true;
	bool EnemiesStart();
	bool EnemiesCleanUp();
	void SetEnemiesInitialPos();
	
	void OnCollision(Collider* c1, Collider* c2);

	void DestroyEntity(j1Entity* entity);//tocheck

	j1Entity* CreateEntity(EntityTypes type, iPoint position = {0,0});// position in the player case it's irrevelant since its modified in the start()

	j1Player* player = nullptr;

	bool Load(pugi::xml_node& );
	bool Save(pugi::xml_node& ) const;

	bool paused = false;

	void CreatePlayer();

protected:
	struct EnemyInfo {
		iPoint pos;
		EntityTypes type = EntityTypes::NOTYPE;
	};
	EnemyInfo to_spawn[MAX_ENTITIES - 1];
	p2List<j1Entity*> entities;
private:
	void AddtoSpawningQueue(iPoint pos, EntityTypes t);
	void SpawnListReset();
	void CheckPlayerPostoSpawn();
	void CheckPlayerPostoDespawn();
};

#endif // __j1ENTITIES_H__