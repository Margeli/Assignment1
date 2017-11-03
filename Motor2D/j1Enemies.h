#ifndef __j1ENEMIES_H__
#define __j1ENEMIES_H_

#include "j1Module.h"

#define MAX_ENEMIES 20

struct SDL_Texture;

enum ENEMY_TYPES
{
	NO_TYPE,
	TROLL,
	FLYING
};

class j1Enemy;

struct EnemyInfo
{
	ENEMY_TYPES type = ENEMY_TYPES::NO_TYPE;
	int x, y;
	
};

class j1Enemies : public j1Module
{
public:


	j1Enemies();

	virtual ~j1Enemies();

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void OnCollision(Collider* c1, Collider* c2);	

	bool AddEnemy(ENEMY_TYPES type, int x, int y);

	j1Enemy* enemies[MAX_ENEMIES];

private:

	void SpawnEnemy(const EnemyInfo& info);

	EnemyInfo queue[MAX_ENEMIES];

	SDL_Texture* sprites;
};

#endif //__j1ENEMIES_H_
