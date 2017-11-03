#ifndef __j1FLYINGENEMIE_H__
#define __j1FLYINGENEMIE_H_

#include "j1Enemy.h"
#include "p2Point.h"
#include "j1Animation.h"
#include "j1Collisions.h"

class j1FlyingEnemie : public j1Enemy
{
public:

	j1FlyingEnemie(int x, int y);
		
	void Move();

	void OnCollision(Collider* c1, int num_enemy);
	

public:
		
	Animation fly_right;
	Animation fly_left;

};


#endif //__FLYINGENEMIE_H_
