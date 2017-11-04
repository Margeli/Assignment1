#include "j1Troll.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Enemies.h"

j1Troll::j1Troll(int x, int y) : j1Enemy(x, y)
{
	
	sprite_path = App->tex->Load("textures/Troll1.png");

	idle_right.PushBack({ 0, 0, 81, 63 });
	idle_right.PushBack({ 93, 0, 84, 65 });
	idle_right.PushBack({ 187, 0, 88, 65 });
	idle_right.PushBack({ 281, 0, 88, 63 });
	idle_right.PushBack({ 375, 0, 86, 64 });
	idle_right.PushBack({ 467, 0, 85, 65 });
	idle_right.PushBack({ 561, 0, 85, 65 });
	idle_right.loop = true; 
	idle_right.speed = 0.2f;

	idle_left.PushBack({ 0, 66, 85, 64 });
	idle_left.PushBack({ 94, 67, 85, 63 });
	idle_left.PushBack({ 187, 67, 86, 62 });
	idle_left.PushBack({ 281, 67, 88, 63 });
	idle_left.PushBack({ 375, 67, 85, 63 });
	idle_left.PushBack({ 469, 68, 84, 62 });
	idle_left.PushBack({ 561, 68, 81, 62 });
	idle_left.loop = true;
	idle_left.speed = 0.2f;

	walk_right.PushBack({ 0, 264, 93, 62 });
	walk_right.PushBack({ 93, 265, 93, 62 });
	walk_right.PushBack({ 187, 262, 84, 64 });
	walk_right.PushBack({ 281, 262, 78, 64 });
	walk_right.PushBack({ 376, 262, 78, 64 });
	walk_right.PushBack({ 467, 262, 83, 64 });
	walk_right.PushBack({ 562, 263, 90, 63 });
	walk_right.loop = true;
	walk_right.speed = 0.2f;

	walk_left.PushBack({ 0, 327, 93, 62 });
	walk_left.PushBack({ 93, 327, 93, 62 });
	walk_left.PushBack({ 187, 327, 84, 64 });
	walk_left.PushBack({ 281, 327, 78, 64 });
	walk_left.PushBack({ 376, 327, 78, 64 });
	walk_left.PushBack({ 467, 327, 83, 64 });
	walk_left.PushBack({ 562, 327, 90, 63 });
	walk_left.loop = true;
	walk_left.speed = 0.2f;

	jump_right.PushBack({ 0, 132, 82, 62 });
	jump_right.PushBack({ 93, 132, 88, 62 });
	jump_right.PushBack({ 187, 132, 93, 63 });
	jump_right.PushBack({ 281, 132, 94, 63 });
	jump_right.PushBack({ 375, 132, 92, 63 });
	jump_right.PushBack({ 467, 132, 94, 63 });
	jump_right.PushBack({ 562, 132, 92, 63 });
	jump_right.loop = true;
	jump_right.speed = 0.2f;

	jump_left.PushBack({ 0, 197, 82, 62 });
	jump_left.PushBack({ 93, 197, 88, 62 });
	jump_left.PushBack({ 187, 197, 93, 63 });
	jump_left.PushBack({ 281, 197, 94, 63 });
	jump_left.PushBack({ 375, 197, 92, 63 });
	jump_left.PushBack({ 467, 197, 94, 63 });
	jump_left.PushBack({ 562, 197, 92, 63 });
	jump_left.loop = true;
	jump_left.speed = 0.2f;

	attack_right.PushBack({ 0, 393, 88, 94 });
	attack_right.PushBack({ 93, 393, 82, 94 });
	attack_right.PushBack({ 187, 393, 79, 94 });
	attack_right.PushBack({ 281, 393, 79, 94 });
	attack_right.PushBack({ 375, 393, 84, 94 });
	attack_right.PushBack({ 467, 393, 82, 94 });
	attack_right.PushBack({ 561, 393, 85, 94 });
	attack_right.loop = true;
	attack_right.speed = 0.2f;

	attack_left.PushBack({ 0, 487, 85, 94 });
	attack_left.PushBack({ 93, 487, 85, 94 });
	attack_left.PushBack({ 187, 487, 85, 94 });
	attack_left.PushBack({ 280, 487, 85, 94 });
	attack_left.PushBack({ 375, 487, 85, 94 });
	attack_left.PushBack({ 467, 487, 85, 94 });
	attack_left.PushBack({ 561, 487, 85, 94 });
	attack_left.loop = true;
	attack_left.speed = 0.2f;

	death_right.PushBack({ 0, 581, 72, 57 });
	death_right.PushBack({ 93, 581, 88, 57 });
	death_right.PushBack({ 187, 581, 102, 57 });
	death_right.PushBack({ 294, 581, 100, 57 });
	death_right.PushBack({ 395, 581, 92, 57 });
	death_right.PushBack({ 489, 581, 89, 57 });
	death_right.PushBack({ 579, 581, 91, 57 });
	death_right.loop = false;
	death_right.speed = 0.2f;

	death_left.PushBack({ 0, 638, 72, 57 });
	death_left.PushBack({ 93, 638, 88, 57 });
	death_left.PushBack({ 187, 638, 102, 57 });
	death_left.PushBack({ 294, 638, 100, 57 });
	death_left.PushBack({ 395, 638, 92, 57 });
	death_left.PushBack({ 489, 638, 89, 57 });
	death_left.PushBack({ 579, 638, 91, 57 });
	death_left.loop = false;
	death_left.speed = 0.2f;


	collider = App->collis->AddCollider({ position.x, position.y, 46, 60 }, COLLIDER_ENEMIE, (j1Module*)App->enemies);

	animation = &idle_right;
}

// Should have the initial pos of enemies in a XML

void j1Troll::Move()
{


	//IA
	//Pathfinding applied to platformers.

	/*if (IsPointInCircle(App->player->position.x, App->player->position.y, position.x, position.y, 500) == true)
	{
		if (App->player->position.x < App->enemie->position.x)
		{
			position.x -= 1.5f;
			animation = &walk_left;
		}
		else if (App->player->position.x > position.x)
		{
			position.x += 1.5f;
			animation = &walk_right;
		}

	}*/

	collider->SetPos(position.x, position.y + 5); 

}

bool j1Troll::IsPointInCircle(float playposX, float playposY, float enemposX, float enemposY, float radi) const
{
	return ((playposX - enemposX)*(playposX - enemposX) + (playposY - enemposY)*(playposY - enemposY)) < radi*radi;
}


void j1Troll::OnCollision(Collider* c1, int num_enemy)
{
	
}

