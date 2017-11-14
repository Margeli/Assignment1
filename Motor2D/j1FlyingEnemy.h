#ifndef __j1FLYINGENEMIE_H__
#define __j1FLYINGENEMIE_H_

#include "j1Enemy.h"
#include "p2Point.h"
#include "j1Animation.h"
#include "j1Collisions.h"
#include "j1Entity.h"

class Pathfinding;
class j1FlyingEnemy : public j1Entity
{
public:

	j1FlyingEnemy(iPoint pos);
		
	void OnCollision(Collider* c1, Collider* c2);

	bool IsPointInCircle(float playposX, float playposY, float enemposX, float enemposY, float radi) const;
	
	void LoadFlyAnimations();

	bool Start();
	bool Update(float dt);
	
	bool CleanUp();

public:

	Pathfinding* path =nullptr;
		
	Animation fly_right;
	Animation fly_left;
};


#endif //__FLYINGENEMIE_H_
