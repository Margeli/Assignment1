#include "j1FlyingEnemy.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Textures.h"
#include "j1EntityManager.h"
#include "j1Player.h"
#include "j1Entity.h"
#include "j1Pathfinding.h"
#include "p2Log.h"
#include "j1Audio.h"

#define FLY_SPEED 2
#define FLY_HEIGHT 32
#define FLY_WIDTH 32
#define FLYING_ENEMY_DETECION_RANGE 500
#define COLLIDER_POS_X 5
#define COLLIDER_POS_Y 10
#define ORIGIN_POSITION 30

j1FlyingEnemy::j1FlyingEnemy(iPoint pos) : j1Entity(EntityTypes::FLY) 
{
	position = initial_pos = pos;		
	LoadFlyAnimations();
	collider = nullptr;
	sprites = nullptr;
	animation = nullptr;
}

bool j1FlyingEnemy::Start()
{
	bool ret = true;
	fposition = { (float)position.x, (float)position.y };
	collider = App->collis->AddCollider({ position.x + COLLIDER_POS_X, position.y+ COLLIDER_POS_Y, FLY_WIDTH, FLY_HEIGHT }, COLLIDER_ENEMIE, App->entities);	// Should have the initial pos of enemies in a XML
	sprites = App->tex->Load("textures/Fly.png");
	fly_death = App->audio->LoadFx("audio/fx/fly_death.wav");
	fly_attack = App->audio->LoadFx("audio/fx/fly_attack.wav");
	if (!sprites) { LOG("Error loading fly's textures.");  ret = false; }
	animation = &fly_left;
	SetInitialPos();

	return ret;
}

bool j1FlyingEnemy::IsPointInCircle(iPoint playpos, iPoint enempos, float radi) const	//Creates a circular detection area
{
	return ((playpos.x - enempos.x)*(playpos.x - enempos.x) + (playpos.y - enempos.y)*(playpos.y - enempos.y)) < radi*radi;
}

void j1FlyingEnemy::OnCollision(Collider* c1, Collider* c2)		
{
	CollisionDirection direction;
	if (c2->type == COLLIDER_PLAYER)
	{
		direction = c1->CheckDirection(c2->rect);
		if (direction == ENTITY_BELOW && !App->entities->player->player_hurted)
		{			
			App->audio->PlayFx(fly_death);
			App->entities->player->LittleJump();
			App->entities->player->points += 10;
			collider->to_delete = true;
			path->Clear();
			App->entities->DestroyEntity(this);
		}
	}
	/*else if (c2->type == COLLIDER_GROUND)
	{
		switch (c1->CheckDirection(c2->rect))
		{
		case ENTITY_ABOVE:
			fposition.y = c2->rect.y - FLY_HEIGHT;
			break;
		case ENTITY_BELOW:
			fposition.y = c2->rect.y + c2->rect.h;
			break;
		case ENTITY_RIGHT:
			fposition.x = c2->rect.x + c2->rect.w;
			break;
		case ENTITY_LEFT:
			fposition.x = c2->rect.x - FLY_WIDTH;
			break;
		}
	}*/
	
}

void j1FlyingEnemy::LoadFlyAnimations()
{
	fly_right.LoadEnemyAnimations("fly_right", "fly");
	fly_left.LoadEnemyAnimations("fly_left", "fly");
}

bool j1FlyingEnemy::CleanUp()
{
	App->tex->UnLoad(sprites);
	if (App->collis->CollidersNum() != 0)
		collider->to_delete = true;
	
	//path->Clear();
	return true;
}

bool j1FlyingEnemy::Update(float dt)	
{
	BROFILER_CATEGORY("EntityFLYUpdate", Profiler::Color::Bisque);
	if (paused) { Draw(); return true; }
	iPoint origin = { position.x+FLY_WIDTH/2, position.y + FLY_HEIGHT / 2 };
	iPoint destination = { App->entities->player->position.x + PLAYERWIDTH / 2, App->entities->player->position.y + PLAYERHEIGHT - 20, };
	if (IsPointInCircle(App->entities->player->position, position, FLYING_ENEMY_DETECION_RANGE)) {
		
		path = App->pathfind->FindPath(origin, destination, type);
	}
	if (path != NULL) {
		if (App->entities->player->player_hurted == false && path->breadcrumbs.count() != 0) {
			Move(*path, dt);
		}
		else { path->Clear(); }
	}
	
	if (facing == Facing::RIGHT) { animation = &fly_right; }
	else if (facing == Facing::LEFT) { animation = &fly_left; }

	if (collider != nullptr)
		collider->SetPos(fposition.x + COLLIDER_POS_X, fposition.y + COLLIDER_POS_Y);
	Draw();
	if (App->collis->debug && path!= NULL) {
		App->pathfind->DrawPath(*path);
	}
	return true;
}

void j1FlyingEnemy::Move(Pathfinding& _path, float dt) 
{
	speed = FLY_SPEED + FLY_SPEED* dt;

	direction = App->pathfind->CheckDirection(_path);

	if (direction == MoveTo::M_DOWN)
	{
		fposition.y += FLY_SPEED;		
		return;
	}

	if (direction == MoveTo::M_UP)
	{
		fposition.y -= FLY_SPEED;		
		return;
	}

	if (direction == MoveTo::M_RIGHT)
	{	
		fposition.x += FLY_SPEED;
		facing = Facing::RIGHT;
		return;
	}
	if (direction == MoveTo::M_LEFT)
	{
		fposition.x -= FLY_SPEED;	
		facing = Facing::LEFT;
		return;
	}	
	
} 

bool j1FlyingEnemy::Save(pugi::xml_node& data ) const {
	
	pugi::xml_node pos = data.append_child("position");

	pos.append_attribute("x") = position.x;
	pos.append_attribute("y") = position.y;

	return true;
}
bool j1FlyingEnemy::Load(pugi::xml_node& data) {

	return true;
}
