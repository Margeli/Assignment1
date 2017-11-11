#ifndef __j1FLYINGENEMIE_H__
#define __j1FLYINGENEMIE_H_

#include "p2Point.h"
#include "j1Animation.h"
#include "j1Collisions.h"
#include "j1Entity.h"

class j1FlyingEnemie : public j1Entity
{
public:

	j1FlyingEnemie(iPoint pos);
		
	void OnCollision(Collider* c1, int num_enemy);

	bool IsPointInCircle(float playposX, float playposY, float enemposX, float enemposY, float radi) const;
	
	void LoadFlyAnimations();

	bool Start();
	bool Update(float dt);
	
	bool CleanUp();

public:
		
	Animation fly_right;
	Animation fly_left;

};


#endif //__FLYINGENEMIE_H_
