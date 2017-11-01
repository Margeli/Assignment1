#include "j1Enemies.h"
#include "p2Defs.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Animation.h"
#include "j1Audio.h"
#include "j1Player.h"

j1Enemies::j1Enemies()
{
	name.create("Troll1");
	graphics = nullptr;

	idle_right.PushBack({ 0, 0, 81, 63 });
	idle_right.PushBack({ 93, 0, 84, 65 });
	idle_right.PushBack({ 187, 0, 88, 65  });
	idle_right.PushBack({ 281, 0, 81, 63 });
	idle_right.PushBack({ 375, 0, 86, 64 });
	idle_right.PushBack({ 467, 0, 85, 65 });
	idle_right.PushBack({ 561, 0, 85, 65 });
	idle_right.loop = true;
	idle_right.speed = 0.05f;

	idle_left.PushBack({ 0, 66, 85, 64 });
	idle_left.PushBack({ 94, 67, 85, 63 });
	idle_left.PushBack({ 187, 67, 86, 62 });
	idle_left.PushBack({ 281, 67, 88, 63 });
	idle_left.PushBack({ 375, 67, 85, 63 });
	idle_left.PushBack({ 469, 68, 84, 62 });
	idle_left.PushBack({ 561, 68, 81, 62 });
	idle_left.loop = true;
	idle_left.speed = 0.05f;

	walk_right.PushBack({ 0, 264, 93, 62});
	walk_right.PushBack({ 93, 265, 93, 62 });
	walk_right.PushBack({ 187, 262, 84, 64 });
	walk_right.PushBack({ 281, 262, 78, 64 });
	walk_right.PushBack({ 376, 262, 78, 64 });
	walk_right.PushBack({ 467, 262, 83, 64 });
	walk_right.PushBack({ 562, 263, 90, 63 });
	walk_right.loop = true;
	walk_right.speed = 0.05f;

	walk_left.PushBack({ 0, 327, 93, 62 });
	walk_left.PushBack({ 93, 327, 93, 62 });
	walk_left.PushBack({ 187, 327, 84, 64 });
	walk_left.PushBack({ 281, 327, 78, 64 });
	walk_left.PushBack({ 376, 327, 78, 64 });
	walk_left.PushBack({ 467, 327, 83, 64 });
	walk_left.PushBack({ 562, 327, 90, 63 });
	walk_left.loop = true;
	walk_left.speed = 0.05f;

	jump_right.PushBack({ 0, 132, 82, 62 });
	jump_right.PushBack({ 93, 132, 88, 62 });
	jump_right.PushBack({ 187, 132, 93, 63 });
	jump_right.PushBack({ 281, 132, 94, 63 });
	jump_right.PushBack({ 375, 132, 92, 63 });
	jump_right.PushBack({ 467, 132, 94, 63 });
	jump_right.PushBack({ 562, 132, 92, 63 });
	jump_right.loop = true;
	jump_right.speed = 0.05f;

	jump_left.PushBack({ 0, 197, 82, 62 });
	jump_left.PushBack({ 93, 197, 88, 62 });
	jump_left.PushBack({ 187, 197, 93, 63 });
	jump_left.PushBack({ 281, 197, 94, 63 });
	jump_left.PushBack({ 375, 197, 92, 63 });
	jump_left.PushBack({ 467, 197, 94, 63 });
	jump_left.PushBack({ 562, 197, 92, 63 });
	jump_left.loop = true;
	jump_left.speed = 0.05f;

	attack_right.PushBack({ 0, 393, 88, 94 });
	attack_right.PushBack({ 93, 393, 82, 94 });
	attack_right.PushBack({ 187, 393, 79, 94 });
	attack_right.PushBack({ 281, 393, 79, 94 });
	attack_right.PushBack({ 375, 393, 84, 94 });
	attack_right.PushBack({ 467, 393, 82, 94 });
	attack_right.PushBack({ 561, 393, 85, 94 });
	attack_right.loop = true;
	attack_right.speed = 0.07f;

	attack_left.PushBack({ 0, 0, 0, 0 });
	attack_left.loop = true;
	attack_left.speed = 0.07f;

	death_right.PushBack({ 0, 0, 0, 0 });
	death_right.loop = false;
	death_right.speed = 0.07f;

	death_left.PushBack({ 0, 0, 0, 0 });
	death_left.loop = false;
	death_left.speed = 0.07f;
}


j1Enemies::~j1Enemies()
{
}

bool j1Enemies::Awake(pugi::xml_node& conf)
{
	return true;
}

bool j1Enemies::Start()
{
	bool ret = true;

	graphics = App->tex->Load("textures/Troll1.png");
	enemie_collider = App->collis->AddCollider({ position.x, position.y, 46, 60 }, COLLIDER_ENEMIE, this);

	if (!graphics) { LOG("Error loading Troll 1 sprites.");  ret = false; }

	current_animation = &idle_right;

	if (troll_sound == 0)
		troll_sound = App->audio->LoadFx("audio/fx/troll_attack.wav");

	if (troll_death_sound == 0)
		troll_death_sound = App->audio->LoadFx("audio/fx/troll_death.wav");

	return ret;
}

bool j1Enemies::CleanUp()
{
	LOG("Unloading Troll 1.");

	App->tex->UnLoad(graphics);

	if (enemie_collider != nullptr)
	{
		enemie_collider->to_delete = true;
	}

	return true;
}

bool j1Enemies::Update(float dt)
{
	//IA
	//Pathfinding applied to platformers.

	if (IsPointInCircle(App->player->position.x, App->player->position.y, App->enemie->position.x, App->enemie->position.y, 500) == true)
	{
		if (App->player->position.x < App->enemie->position.x)
		{
			App->enemie->position.x -= 1.5f;
			current_animation = &walk_left;
		}
		else if (App->player->position.x > App->enemie->position.x)
		{
			App->enemie->position.x += 1.5f;
			current_animation = &walk_right;
		}

	}
	
	if (enemie_collider != nullptr) { enemie_collider->SetPos(position.x, position.y + 5); }

	App->render->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()));
	return true;
}

bool j1Enemies::IsPointInCircle(float playposX, float playposY, float enemposX, float enemposY, float radi)
{
	return ((playposX - enemposX)*(playposX - enemposX) + (playposY - enemposY)*(playposY - enemposY)) < radi*radi;
}

void j1Enemies::InitialPos()
{
	position = { 500, 200 };
}

void j1Enemies::OnCollision(Collider* c1, Collider* c2)
{
	int margin = 0;
	switch (c2->type) {
	case COLLIDER_GROUND:

		switch (c1->CheckDirection(c2->rect)) {

		case ENEMIE_ABOVE:
			position.y = c2->rect.y - 80 - margin;		
			break;
		case ENEMIE_BELOW:
			position.y = c2->rect.y + c2->rect.h + margin;
			break;
		case ENEMIE_RIGHT:
			position.x = c2->rect.x + c2->rect.w + margin;
			break;
		case ENEMIE_LEFT:
			position.x = c2->rect.x - 46 - margin;
			break;
		}

		break;
	}
}

