#ifndef __j1ENTITIES_H__
#define __j1ENTITIES_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "j1Entity.h"

#define MAX_ENTITIES 50

enum EntityTypes
{
	PLAYER,
	TROLL,
	FLY
};


class j1Player;
class j1Entity;

class j1EntityManager : public j1Module
{
public:
	j1EntityManager();
	virtual ~j1EntityManager();

	virtual bool Start();
	virtual bool Update(float dt);
	virtual bool PostUpdate();
	virtual bool PreUpdate();
	virtual bool CleanUp();
	
	void OnCollision(Collider* c1, Collider* c2);

	void DestroyEntity(p2List_item<j1Entity*>* entitytoremove);//tocheck

	j1Entity* CreateEntity(EntityTypes type, iPoint position = {0,0});// position in the player case it's irrevelant since its modified in the start()

	j1Player* player;

protected:

	
	p2List<j1Entity*> entities;
	
};

#endif // __j1ENTITIES_H__