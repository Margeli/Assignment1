#ifndef __j1ENEMY_H__
#define __j1ENEMY_H__


#include "j1Animation.h"
#include "p2Point.h"


struct SDL_Texture;
struct Collider;

class j1Enemy
{
protected:
	Animation* animation = nullptr;
	Collider* collider = nullptr;


public:
	iPoint position;
	SDL_Texture* sprite_path = nullptr;

public:
	j1Enemy(int x, int y);
	virtual ~j1Enemy();

	const Collider* GetCollider() const;

	
	float hits_life;
	virtual void Move() {};
	virtual void Draw(SDL_Texture* sprites);	
	virtual void OnCollision(Collider* collider, int num_enemy);
};

#endif // __j1ENEMY_H__
