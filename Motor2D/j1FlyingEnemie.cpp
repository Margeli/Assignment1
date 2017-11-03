#include "j1FlyingEnemie.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Enemies.h"

j1FlyingEnemie::j1FlyingEnemie(int x, int y) : j1Enemy(x, y)
{
	
	sprite_path = App->tex->Load("textures/Fly.png");

	fly_right.PushBack({ 0, 0, 0, 0 });
	fly_right.loop = true;
	fly_right.speed = 0.07f;

	fly_left.PushBack({ 0, 0, 0, 0 });
	fly_left.loop = true;
	fly_left.speed = 0.07f;


	collider = App->collis->AddCollider({ position.x, position.y, 0, 0 }, COLLIDER_ENEMIE, (j1Module*)App->enemies);

	animation = &fly_left;
}

// Should have the initial pos of enemies in a XML

void j1FlyingEnemie::Move()
{
	//IA
	//Pathfinding applied to platformers.

	
	collider->SetPos(position.x, position.y + 5);
}

void j1FlyingEnemie::OnCollision(Collider* c1, int num_enemy)
{

}


