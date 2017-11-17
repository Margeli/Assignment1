#include "j1Troll.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Player.h"
#include "j1Entity.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1Audio.h"

#define TROLL_ATTACK_RANGE 200
#define TROLL_DETECTION_RANGE 350
#define TROLL_SPEED 1.00f
#define ADDED_COLLIDER_WIDTH 10
#define ADDED_COLLIDER_HEIGHT 50
#define TROLL_HEIGHT 100
#define TROLL_WIDTH 50
#define PATH_DISPLACEMENT_x 55
#define PATH_DISPLACEMENT_y 70

j1Troll::j1Troll(iPoint pos) : j1Entity(EntityTypes::TROLL)		// Should have the initial pos of enemies in a XML
{
	LoadTrollAnimations();	
	position = initial_pos = pos;
}

bool j1Troll::Start() 
{
	bool ret = true;
	collider = App->collis->AddCollider({ position.x, position.y, 66, 50 }, COLLIDER_ENEMIE, App->entities);
	sprites = App->tex->Load("textures/Troll1.png");	
	if (!sprites) { LOG("Error loading troll's textures.");  ret = false; }
	animation = &idle_left;
	LoadTrollAudio();
	SetInitialPos();
	return ret;
}

bool j1Troll::LoadTrollAudio()
{
	bool ret = true;
	troll_death = App->audio->LoadFx("audio/fx/troll_death.wav");
	troll_attack = App->audio->LoadFx("audio/fx/troll_attack.wav");
	if (!troll_death || !troll_attack) { LOG("Error loading troll's audio.");  ret = false; }
	return true;
}

bool j1Troll::IsPointInCircle(float playposX, float playposY, float enemposX, float enemposY, float radi) const
{
	return ((playposX - enemposX)*(playposX - enemposX) + (playposY - enemposY)*(playposY - enemposY)) < radi*radi;
}

void j1Troll::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_GROUND)
	{
		switch (c1->CheckDirection(c2->rect))
		{
		case ENTITY_ABOVE:
			position.y = c2->rect.y - TROLL_HEIGHT;
			break;
		case ENTITY_BELOW:
			position.y = c2->rect.y + c2->rect.h;
			break;
		case ENTITY_RIGHT:
			position.x = c2->rect.x + c2->rect.w;
			break;
		case ENTITY_LEFT:
			position.x = c2->rect.x - TROLL_WIDTH;
			break;
		}
	}
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

bool j1Troll::CleanUp() 
{
	LOG("Unloading Troll.");
	App->tex->UnLoad(sprites);
	collider->to_delete = true;
	path->Clear();
	return true;
}

bool j1Troll::Update(float dt) 
{
	position.y += GRAVITY;

	if (IsPointInCircle(App->entities->player->position.x, App->entities->player->position.y, position.x, position.y, TROLL_DETECTION_RANGE))
	{
		iPoint origin = {   (position.x + PATH_DISPLACEMENT_x),(position.y + PATH_DISPLACEMENT_y) };
		iPoint destination = { (App->entities->player->position.x + PLAYERWIDTH / 2), (App->entities->player->position.y + PLAYERHEIGHT - 20) };	//why 20?

		path = App->pathfind->FindPath(origin, destination, type);

		if (path != nullptr)
		{
			if (App->entities->player->player_hurted == false && path->breadcrumbs.count() != 0) 
			{
				Move(*path);

				if (IsPointInCircle(App->entities->player->position.x, App->entities->player->position.y, position.x, position.y, TROLL_ATTACK_RANGE))
				{
					if (App->entities->player->position.x < position.x)
					{
						App->audio->PlayFx(troll_attack);
						animation = &attack_left;
					}
					else if (App->entities->player->position.x > position.x)
					{
						App->audio->PlayFx(troll_attack);
						animation = &attack_right;
					}
				}
			}
			else { path->Clear(); }
		}

		if (App->collis->debug && path != nullptr) { App->pathfind->DrawPath(*path); }

		/*if (App->entities->player->position.x < position.x)	{ animation = &walk_left; }
		else if (App->entities->player->position.x > position.x)	{ animation = &walk_right; }*/

	}
	else
	{
		if (App->entities->player->position.x < position.x) { animation = &idle_left; }
		else if (App->entities->player->position.x > position.x) { animation = &idle_right; }
	}

	if (collider != nullptr) { collider->SetPos(position.x + ADDED_COLLIDER_WIDTH, position.y + ADDED_COLLIDER_HEIGHT); }

	if (App->entities->player->hitted == true) { animation = &idle_left; }

	Draw();

	return true;
}

void j1Troll::Move(Pathfinding& _path)
{
	direction = App->pathfind->CheckDirection(_path);

	if (direction == MoveTo::M_DOWN)
	{
		position.y += TROLL_SPEED;
		return;
	}

	if (direction == MoveTo::M_RIGHT)
	{
		animation = &walk_right;
		position.x += TROLL_SPEED;
		facing = Facing::RIGHT;
		return;
	}
	if (direction == MoveTo::M_LEFT)
	{
		animation = &walk_left;
		position.x -= TROLL_SPEED;
		facing = Facing::LEFT;
		return;
	}
	
}


bool j1Troll::Save(pugi::xml_node& data) const {

	pugi::xml_node pos = data.append_child("position");

	pos.append_attribute("x") = position.x;
	pos.append_attribute("y") = position.y;

	return true;
}

bool j1Troll::Load(pugi::xml_node&) {

	return true;
}