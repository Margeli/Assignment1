#include "j1FlyingEnemie.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1EntityManager.h"
#include "j1Player.h"
#include "j1Entity.h"

j1FlyingEnemie::j1FlyingEnemie(iPoint pos) : j1Entity(EntityTypes::FLY) 
{
	position = pos;		
	LoadFlyAnimations();	
}

bool j1FlyingEnemie::Start()
{
	collider = App->collis->AddCollider({ position.x, position.y, 50, 60 }, COLLIDER_ENEMIE, App->entities);	// Should have the initial pos of enemies in a XML
	sprites = App->tex->Load("textures/Fly.png");
	animation = &fly_left;

	return true;
}

bool j1FlyingEnemie::IsPointInCircle(float playposX, float playposY, float enemposX, float enemposY, float radi) const	//Creates a circular detection area
{
	return ((playposX - enemposX)*(playposX - enemposX) + (playposY - enemposY)*(playposY - enemposY)) < radi*radi;
}

void j1FlyingEnemie::OnCollision(Collider* c1, Collider* c2)
{
	//The OnCollision function has to be changed for all.	//TODO 
}

void j1FlyingEnemie::LoadFlyAnimations()
{
	fly_right.LoadEnemyAnimations("fly_right", "fly");
	fly_left.LoadEnemyAnimations("fly_left", "fly");
}


bool j1FlyingEnemie::CleanUp()
{
	App->tex->UnLoad(sprites);
	collider->to_delete = true;
	return true;
	
}
bool j1FlyingEnemie::Update(float dt)
{
	if (App->entities->player->position.x <= position.x) 		
		animation = &fly_left;
	else	animation = &fly_right;

	position.x += rand() % 4;			//Just to make it more fun while pathfinding isn't ready :)
	position.y -= rand() % 3;
	position.x -= rand() % 3 ;
	position.y += rand() % 3;

	if (collider!= nullptr)
	collider->SetPos(position.x, position.y + 5);
	Draw();
	return true;
}