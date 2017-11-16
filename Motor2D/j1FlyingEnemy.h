#ifndef __j1FLYINGENEMIE_H__
#define __j1FLYINGENEMIE_H_

#include "j1Enemy.h"
#include "p2Point.h"
#include "j1Animation.h"
#include "j1Collisions.h"
#include "j1Entity.h"

struct Pathfinding;
enum MoveTo;

class j1FlyingEnemy : public j1Entity
{
public:

	j1FlyingEnemy(iPoint pos);
		
	void OnCollision(Collider* c1, Collider* c2);
	
	bool IsPointInCircle(iPoint playpos, iPoint enempos, float radi) const;
	
	void LoadFlyAnimations();

	bool Start();

	bool Update(float dt);
	
	bool CleanUp();

	void Move(Pathfinding& _path);

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

public:

	Pathfinding* path = nullptr;
	MoveTo direction;
	
	Animation fly_right;
	Animation fly_left;
};


#endif //__FLYINGENEMIE_H_
