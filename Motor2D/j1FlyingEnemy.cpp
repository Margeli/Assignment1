#include "j1FlyingEnemy.h"
#include "j1App.h"
#include "j1Map.h"
#include "j1Textures.h"
#include "j1EntityManager.h"
#include "j1Player.h"
#include "j1Entity.h"
#include "j1Pathfinding.h"


#define GRAVITY 2
#define FLY_HEIGHT 50
#define FLY_WIDTH 40
#define FLY_SPEED 1
#define FLYING_ENEMY_DETECION_RANGE 500

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
	collider = App->collis->AddCollider({ position.x , position.y, FLY_WIDTH, FLY_HEIGHT }, COLLIDER_ENEMIE, App->entities);	// Should have the initial pos of enemies in a XML
	sprites = App->tex->Load("textures/Fly.png");
	animation = &fly_left;
	SetInitialPos();

	return true;
}

bool j1FlyingEnemy::IsPointInCircle(float playposX, float playposY, float enemposX, float enemposY, float radi) const	//Creates a circular detection area
{
	return ((playposX - enemposX)*(playposX - enemposX) + (playposY - enemposY)*(playposY - enemposY)) < radi*radi;
}

void j1FlyingEnemy::OnCollision(Collider* c1, Collider* c2)
{
	if (c2->type == COLLIDER_GROUND)
	{
		switch (c1->CheckDirection(c2->rect))
		{
		case ENTITY_ABOVE:
			position.y = c2->rect.y - FLY_HEIGHT;
			break;
		case ENTITY_BELOW:
			position.y = c2->rect.y + c2->rect.h;
			break;
		case ENTITY_RIGHT:
			position.x = c2->rect.x + c2->rect.w ;
			break;
		case ENTITY_LEFT:
			position.x = c2->rect.x - FLY_WIDTH;
			break;
		}
	}
}

void j1FlyingEnemy::LoadFlyAnimations()
{
	fly_right.LoadEnemyAnimations("fly_right", "fly");
	fly_left.LoadEnemyAnimations("fly_left", "fly");
}

bool j1FlyingEnemy::CleanUp()
{
	App->tex->UnLoad(sprites);
	if (collider)
		collider->to_delete = true;

	path->Clear();
	return true;
}

bool j1FlyingEnemy::Update(float dt)
{
	iPoint origin = { position.x +20, position.y +20 };
	iPoint destination = { App->entities->player->position.x + PLAYERWIDTH / 2, App->entities->player->position.y + PLAYERHEIGHT - 40, };
	path = App->pathfind->FindPath(origin, destination);


	if (App->entities->player->player_hurted == false && path->path.Count()!=0 ) { Move(*path); }
	if (facing == Facing::RIGHT) {
		animation = &fly_right;
	}
	else if (facing == Facing::LEFT) {
		animation = &fly_left;
	}

	/*
	


	if (IsPointInCircle(App->entities->player->position.x, App->entities->player->position.y, position.x, position.y, FLYING_ENEMY_DETECION_RANGE))

	{
		path = App->pathfind->FindPath(position, App->entities->player->position);
		App->pathfind->DrawPath(*path);
	}

	if (App->entities->player->position.x <= position.x) { animation = &fly_left; }
	else { animation = &fly_right; }

	position.x += rand() % 3;			
	position.y -= rand() % 3;
	position.x -= rand() % 3;
	position.y += rand() % 3;
	*/
	if (collider!= nullptr)
	collider->SetPos(position.x+10, position.y + 5);
	Draw();
	if (App->collis->debug){
		App->pathfind->DrawPath(*path);///
	}
	return true;
}
void j1FlyingEnemy::Move(Pathfinding& _path) {
	
	direction = App->pathfind->CheckDirection(_path);

	if (direction == MoveTo::M_DOWN) {
		position.y += FLY_SPEED;		
		return;
	}

	if (direction == MoveTo::M_UP) {
		position.y -= FLY_SPEED;		
		return;
	}
		
	if (direction == MoveTo::M_RIGHT) {	
		position.x += FLY_SPEED;
		facing = Facing::RIGHT;
		return;
	}
	if (direction == MoveTo::M_LEFT){
		position.x -= FLY_SPEED;	
		facing = Facing::LEFT;
		return;
	}	
}