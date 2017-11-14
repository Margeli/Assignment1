#include "j1FlyingEnemy.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1EntityManager.h"
#include "j1Player.h"
#include "j1Entity.h"
#include "j1Pathfinding.h"

#define GRAVITY 2
#define COLLIDER_MARGIN_RIGHT 24
#define COLLIDER_MARGIN_LEFT 77
#define FLY_SPEED 0.5f
#define FLYING_ENEMY_DETECION_RANGE 500

j1FlyingEnemy::j1FlyingEnemy(iPoint pos) : j1Entity(EntityTypes::FLY) 
{
	position = pos;		
	LoadFlyAnimations();	
}

bool j1FlyingEnemy::Start()
{
	collider = App->collis->AddCollider({ position.x, position.y, 50, 60 }, COLLIDER_ENEMIE, App->entities);	// Should have the initial pos of enemies in a XML
	sprites = App->tex->Load("textures/Fly.png");
	animation = &fly_left;

	return true;
}

bool j1FlyingEnemy::IsPointInCircle(float playposX, float playposY, float enemposX, float enemposY, float radi) const	//Creates a circular detection area
{
	return ((playposX - enemposX)*(playposX - enemposX) + (playposY - enemposY)*(playposY - enemposY)) < radi*radi;
}

void j1FlyingEnemy::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_GROUND)
	{
		switch (c1->CheckDirection(c2->rect))
		{
		case ENTITY_ABOVE:
			position.y = c2->rect.y - 40;
			break;
		case ENTITY_BELOW:
			position.y = c2->rect.y + c2->rect.h;
			break;
		case ENTITY_RIGHT:
			position.x = c2->rect.x + COLLIDER_MARGIN_RIGHT;
			break;
		case ENTITY_LEFT:
			position.x = c2->rect.x - COLLIDER_MARGIN_LEFT;
			break;
		}
	}
}

void j1FlyingEnemy::LoadFlyAnimations()
{
	fly_right.LoadEnemyAnimations("fly_right", "fly");
	fly_left.LoadEnemyAnimations("fly_left", "fly");
}

bool j1FlyingEnemy::CleanUp()
{
	App->tex->UnLoad(sprites);
	collider->to_delete = true;	
	return true;
}
bool j1FlyingEnemy::Update(float dt)
{
	if (IsPointInCircle(App->entities->player->position.x, App->entities->player->position.y, position.x, position.y, FLYING_ENEMY_DETECION_RANGE))
	{
		path = App->pathfind->FindPath(position, App->entities->player->position);
		App->pathfind->DrawPath(*path);
	}

	if (App->entities->player->position.x <= position.x) { animation = &fly_left; }
	else { animation = &fly_right; }

	position.x += rand() % 3;			
	position.y -= rand() % 3;
	position.x -= rand() % 3;
	position.y += rand() % 3;
	
	if (collider!= nullptr)
	collider->SetPos(position.x, position.y + 5);
	Draw();
	return true;
}