#include "j1Troll.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Player.h"
#include "j1Entity.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1Audio.h"

#define TROLL_ATTACK_RANGE 150
#define TROLL_DETECTION_RANGE 250
#define TROLL_SPEED 3.00f
#define ADDED_COLLIDER_WIDTH 15
#define ADDED_COLLIDER_HEIGHT 50
#define TROLL_HEIGHT 100
#define TROLL_WIDTH 50
#define PATH_DISPLACEMENT_x 40
#define PATH_DISPLACEMENT_y 70
#define T_GRAVITY 1

j1Troll::j1Troll(iPoint pos) : j1Entity(EntityTypes::TROLL)		// Should have the initial pos of enemies in a XML
{
	LoadTrollAnimations();	
	position = initial_pos = pos;
	collider = nullptr;
	sprites = nullptr;
	animation = nullptr;
}

bool j1Troll::Start() 
{
	bool ret = true;
	fposition = { (float)position.x, (float)position.y };
	collider = App->collis->AddCollider({ position.x + ADDED_COLLIDER_WIDTH, position.y+ ADDED_COLLIDER_HEIGHT, 54, 50 }, COLLIDER_ENEMIE, App->entities);
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
	return ret;
}

bool j1Troll::IsPointInCircle(float playposX, float playposY, float enemposX, float enemposY, float radi) const
{
	return ((playposX - enemposX)*(playposX - enemposX) + (playposY - enemposY)*(playposY - enemposY)) < radi*radi;
}

void j1Troll::troll_dead()
{
	dead = true;
	App->entities->player->points += 10;
	App->audio->PlayFx(troll_death);
	death_pos = fposition;
	if (path != NULL) { path->Clear(); }
	collider->to_delete = true;
	if (facing == Facing::LEFT) { animation = &death_left; }
	else if (facing == Facing::RIGHT) { animation = &death_right; }
}

void j1Troll::OnCollision(Collider* c1, Collider* c2)
{
	CollisionDirection direction; 
	if (c2->type == COLLIDER_PLAYER)
	{
		direction = c1->CheckDirection(c2->rect);
		if (direction == ENTITY_BELOW && !App->entities->player->player_hurted) { 
			troll_dead();
			App->entities->player->LittleJump();
		}
		
	}

	else if (c2->type == COLLIDER_GROUND)
	{
		direction = c1->CheckDirection(c2->rect);
		if (direction == ENTITY_ABOVE)
		{
			fposition.y = c2->rect.y - TROLL_HEIGHT;
		}
		else if (direction == ENTITY_BELOW)
		{
			fposition.y = c2->rect.y + c2->rect.h;
		}
		else if (direction == ENTITY_RIGHT)
		{
			fposition.x = c2->rect.x + c2->rect.w;
		}
		else if (direction == ENTITY_LEFT)
		{
			fposition.x = c2->rect.x - TROLL_WIDTH;
		}
	}
}

void j1Troll::LoadTrollAnimations()		
{
	pause_right.LoadEnemyAnimations("pause_right", "troll");
	pause_left.LoadEnemyAnimations("pause_left", "troll");
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
	if (App->collis->CollidersNum()!=0)
		collider->to_delete = true;
	
	//path->Clear();
	return true;
}

bool j1Troll::Update(float dt) 
{

	BROFILER_CATEGORY("EntityTROLLUpdate", Profiler::Color::Bisque);
	
	if (paused) {
		if (facing == LEFT)		
			animation = &pause_left;
		else
			animation = &pause_right;
		Draw();
		return true;
	}
	fposition.y += T_GRAVITY + T_GRAVITY*dt;
	
	if (dead == true) 
	{ // when troll dies
		if (animation == &death_left || animation == &death_right)
		{
			
			if (animation->Finished() == true)
			{

				App->entities->DestroyEntity(this);
				
				return true;
			}

			else
			{
				fposition = death_pos;
				Draw();
				return true;
			}
		}
	}
	if (facing == LEFT) { animation = &idle_left; }
	else if (facing == RIGHT) { animation = &idle_right; }
	if (IsPointInCircle(App->entities->player->position.x, App->entities->player->position.y, position.x, position.y, TROLL_DETECTION_RANGE))
	{
		
		iPoint origin = { (int)(position.x + PATH_DISPLACEMENT_x),(int)(position.y + PATH_DISPLACEMENT_y) };
		iPoint destination = { (int)(App->entities->player->position.x + PLAYERWIDTH / 2), (int)(App->entities->player->position.y + PLAYERHEIGHT - 20) };	//why 20?
	
		
		if (path != nullptr) {
			path->Clear();
		}
			path = App->pathfind->FindPath(origin, destination, type);
		if (path->breadcrumbs.start != nullptr)//cheak if path is created
			{
				if (App->entities->player->player_hurted == false)
				{
					Move(*path, dt);
					if (IsPointInCircle(App->entities->player->position.x, App->entities->player->position.y, position.x, position.y, TROLL_ATTACK_RANGE))
					{

						if (facing == LEFT) {
							App->audio->PlayFx(troll_attack);
							animation = &attack_left;
						}
						else if (facing == RIGHT) {
							App->audio->PlayFx(troll_attack);
							animation = &attack_right;
						}
					}
				}
				else { path->Clear(); }
			}
		

		else
		{
			if (facing == LEFT) { animation = &idle_left; }
			else if (facing == RIGHT) { animation = &idle_right; }
		}
	}

	if (collider != nullptr) { collider->SetPos(fposition.x + ADDED_COLLIDER_WIDTH, fposition.y + ADDED_COLLIDER_HEIGHT); }

	if (App->entities->player->hitted == true) { animation = &idle_left; }

	Draw();
	if (App->collis->debug && path != NULL) {
		App->pathfind->DrawPath(*path); }

	return true;
}

void j1Troll::Move(Pathfinding& _path, float dt)
{
	speed = TROLL_SPEED + TROLL_SPEED* dt;
	direction = App->pathfind->CheckDirection(_path);

	if (direction == MoveTo::M_DOWN)
	{
		fposition.y += speed;
		return;
	}

	if (direction == MoveTo::M_RIGHT)
	{
	animation = &walk_right;
		fposition.x += speed;
		facing = Facing::RIGHT;
		return;
	}
	if (direction == MoveTo::M_LEFT)
	{
		animation = &walk_left;
		fposition.x -= speed;
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