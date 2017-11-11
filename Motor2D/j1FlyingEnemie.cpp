#include "j1FlyingEnemie.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1EntityManager.h"

j1FlyingEnemie::j1FlyingEnemie(iPoint pos) : j1Entity(EntityTypes::FLY) 
{
	position = pos;		
	LoadFlyAnimations();	
}

// Should have the initial pos of enemies in a XML


bool j1FlyingEnemie::Start()
{
	collider = App->collis->AddCollider({ position.x, position.y, 50, 60 }, COLLIDER_ENEMIE, App->entities);
	sprites = App->tex->Load("textures/Fly.png");
	animation = &fly_left;

	return true;
}

bool j1FlyingEnemie::IsPointInCircle(float playposX, float playposY, float enemposX, float enemposY, float radi) const
{
	return ((playposX - enemposX)*(playposX - enemposX) + (playposY - enemposY)*(playposY - enemposY)) < radi*radi;
}

void j1FlyingEnemie::OnCollision(Collider* c1, int num_enemy)
{
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
	//IA
	//Pathfinding applied to platformers.
	collider->SetPos(position.x, position.y + 5);
	Draw();
	return true;
}