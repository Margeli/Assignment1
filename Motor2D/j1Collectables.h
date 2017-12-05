#ifndef __j1COLLECT_H__
#define __j1COLLECT_H__

#include "p2Point.h"
#include "j1Entity.h"
#include "j1Enemy.h"
#include "j1Collisions.h"

class j1Entity;

class j1Collectables : public j1Entity
{
public:

	j1Collectables(iPoint pos);

	void OnCollision(Collider* c1, Collider* c2);

	void LoadAnimation();

	bool LoadAudio(); 

public:

	bool Start();

	bool Update(float dt);

	bool CleanUp();

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	Animation idle;
	
	iPoint found_pos;
	bool found = false;
private:
	uint pick_up;

};
#endif // __j1COLLECT_H__