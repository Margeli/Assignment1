#ifndef __j1ENTITIES_H__
#define __j1ENTITIES_H__

#include "j1Module.h"
#include "p2List.h"
#include "j1Entity.h"

#define MAX_ENTITIES 50




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
	
	
	bool DestroyEntity(j1Entity* entity);

	j1Entity* CreateEntity(EntityTypes type);

protected:

	
	p2List<j1Entity*> entities;
	
};

#endif // __j1ENTITIES_H__