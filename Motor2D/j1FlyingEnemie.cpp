#include "j1FlyingEnemie.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Enemies.h"
#include "j1EntityManager.h"

j1FlyingEnemie::j1FlyingEnemie() : j1Entity(EntityTypes::FLY)
{
	sprite_path = App->tex->Load("textures/Fly.png");

	collider = App->collis->AddCollider({ position.x, position.y, 50, 60 }, COLLIDER_ENEMIE);

	LoadFlyAnimations();

	animation = &fly_left;
}

// Should have the initial pos of enemies in a XML

void j1FlyingEnemie::Move()
{
	//IA
	//Pathfinding applied to platformers.
	
	collider->SetPos(position.x, position.y + 5);
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


