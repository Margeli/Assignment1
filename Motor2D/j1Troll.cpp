#include "j1Troll.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Enemies.h"

j1Troll::j1Troll(int x, int y) : j1Enemy(x, y)
{
	sprite_path = App->tex->Load("textures/Troll1.png");

	LoadTrollAnimations();

	enemie_collider = App->collis->AddCollider({ position.x, position.y, 66, 50 }, COLLIDER_ENEMIE, (j1Module*)App->enemies);

	animation = &idle_left;
}

// Should have the initial pos of enemies in a XML

void j1Troll::Move()
{
	if (enemie_collider != nullptr) { enemie_collider->SetPos(position.x, position.y); }
}

bool j1Troll::IsPointInCircle(float playposX, float playposY, float enemposX, float enemposY, float radi) const
{
	return ((playposX - enemposX)*(playposX - enemposX) + (playposY - enemposY)*(playposY - enemposY)) < radi*radi;
}

void j1Troll::OnCollision(Collider* c1, int num_enemy)
{
}

void j1Troll::LoadTrollAnimations() 
{
	idle_right.LoadEnemyAnimations("idle_right", "troll");
	idle_left.LoadEnemyAnimations("idle_left", "troll");
	walk_right.LoadEnemyAnimations("walk_right", "troll");
	walk_left.LoadEnemyAnimations("walk_left", "troll");
	jump_right.LoadEnemyAnimations("jump_right", "troll");
	jump_left.LoadEnemyAnimations("jump_left", "troll");
	attack_right.LoadEnemyAnimations("attack_right", "troll");
	attack_left.LoadEnemyAnimations("attack_left", "troll");
	death_right.LoadEnemyAnimations("death_right", "troll");
	death_left.LoadEnemyAnimations("death_left", "troll");
}