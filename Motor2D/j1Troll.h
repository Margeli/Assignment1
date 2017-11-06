#ifndef __j1TROLL_H__
#define __j1TROLL_H__

#include "j1Enemy.h"
#include "p2Point.h"
#include "j1Animation.h"
#include "j1Collisions.h"

class j1Troll : public j1Enemy
{
public:

	j1Troll(int x, int y);
	
	void Move();

	void OnCollision(Collider* c1, int num_enemy);
	
	void LoadTrollAnimations();

private:

	bool IsPointInCircle(float playposX, float playposY, float enemposX, float enemposY, float radi) const;
		
	Animation idle_right;
	Animation walk_right;
	Animation jump_right;
	Animation attack_right;
	Animation death_right;

	Animation idle_left;
	Animation walk_left;
	Animation jump_left;
	Animation attack_left;
	Animation death_left;

};
#endif // __j1TROLL_H__