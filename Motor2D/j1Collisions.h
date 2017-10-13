#ifndef __MODULECOLLISION_H__
#define __MODULECOLLISION_H__

#include "j1Module.h"
#include "p2Point.h"
#include "SDL\include\SDL_rect.h"

#define MAX_COLLIDERS 800

enum COLLIDER_TYPE
{
	COLLIDER_NONE = 1,
	COLLIDER_PLAYER,
	COLLIDER_GROUND,
	COLLIDER_MAX
};

enum CollisionDirection
{	
	NO_COLLISION =0,
	PLAYER_ABOVE ,
	PLAYER_BELOW,
	PLAYER_RIGHT,
	PLAYER_LEFT


};

struct Collider
{
	SDL_Rect rect;
	bool to_delete = false;
	COLLIDER_TYPE type;
	CollisionDirection direction;

	j1Module* callback = nullptr;

	Collider(SDL_Rect rectangle, COLLIDER_TYPE type, j1Module* callback = nullptr) : rect(rectangle), type(type), callback(callback) {}

	void SetPos(int x, int y) {rect.x = x; rect.y = y;}
	bool CheckCollision(const SDL_Rect& r) const;
	CollisionDirection CheckDirection(const SDL_Rect& r);
};

class j1Collisions : public j1Module
{
public:

	j1Collisions();
	~j1Collisions();

	bool PreUpdate();
	bool Update(float dt);
	bool CleanUp();

	bool EraseCollider(Collider* collider);
	Collider* AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback = nullptr);
	void DebugDraw();
	//void GroupUpColliders();

	
private:

	Collider* colliders[MAX_COLLIDERS];
	bool matrix[COLLIDER_MAX][COLLIDER_MAX];
	bool debug = false;
};

#endif// __MODULECOLLISION_H__

