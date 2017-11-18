#include "j1Collisions.h"
#include "p2Defs.h"
#include "j1Module.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Player.h"
#include "p2Log.h"
#include "j1FlyingEnemy.h"
#include "Brofiler/Brofiler.h"

#include "j1Map.h"

#include "SDL\include\SDL.h"

j1Collisions::j1Collisions(): j1Module()
{

	for (uint i = 0; i < MAX_COLLIDERS; ++i) {
		colliders[i] = nullptr;	
	}
	name.create("collisions");

	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_PLAYER][COLLIDER_GROUND] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_ENEMIE] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_ATTACK] = false;
	
	matrix[COLLIDER_GROUND][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_GROUND][COLLIDER_GROUND] = false;
	matrix[COLLIDER_GROUND][COLLIDER_ENEMIE] = true;
	matrix[COLLIDER_GROUND][COLLIDER_ATTACK] = false;

	matrix[COLLIDER_ENEMIE][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_ENEMIE][COLLIDER_GROUND] = true;
	matrix[COLLIDER_ENEMIE][COLLIDER_ENEMIE] = false;
	matrix[COLLIDER_ENEMIE][COLLIDER_ATTACK] = true;

	matrix[COLLIDER_ATTACK][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_ATTACK][COLLIDER_GROUND] = false;
	matrix[COLLIDER_ATTACK][COLLIDER_ENEMIE] = true;
	matrix[COLLIDER_ATTACK][COLLIDER_ATTACK] = false;

	matrix[COLLIDER_PICKUP][COLLIDER_PLAYER] = true;
	matrix[COLLIDER_PICKUP][COLLIDER_GROUND] = true;
	matrix[COLLIDER_PICKUP][COLLIDER_ENEMIE] = false;
	matrix[COLLIDER_PICKUP][COLLIDER_ATTACK] = false;
}

j1Collisions::~j1Collisions(){}

bool j1Collisions::PreUpdate()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)//prepares collider's array
	{
		if (colliders[i] != nullptr) {
			if (colliders[i]->to_delete ) {

				delete colliders[i];
				colliders[i] = nullptr;
			}
		}
	}

	return true;
}

bool j1Collisions::CleanUp()
{
	LOG("Freeing all colliders");

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}

bool j1Collisions::Update(float dt) {

	BROFILER_CATEGORY("Collisions_Update", Profiler::Color::Azure);
	Collider* collider1;
	Collider* collider2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;
		if (colliders[i]->type == COLLIDER_PLAYER || colliders[i]->type == COLLIDER_ENEMIE || colliders[i]->type == COLLIDER_PICKUP)
		{

			collider1 = colliders[i]; // player collider or enemie

			for (uint k = 0; k < MAX_COLLIDERS; ++k)
			{
				if (colliders[k] == nullptr || i==k) // if collider is nullptr or the player collider itself
					continue;
				
					collider2 = colliders[k];

					if (collider1->CheckCollision(collider2->rect) == true)
					{						
						if (matrix[collider1->type][collider2->type] && collider1->callback)
							collider1->callback->OnCollision(collider1, collider2);					
				}
			}
		}
	}

	DebugDraw();

	return true;
}

void j1Collisions::DebugDraw() {

	if (App->input->GetKey(SDL_SCANCODE_F8) == KEY_DOWN)
		debug = !debug;

	if (debug == false)
		return;

	Uint8 alpha = 80;
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;

		switch (colliders[i]->type)
		{
		case COLLIDER_NONE:		//white														
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
			break;
		case COLLIDER_GROUND:	//blue															
			App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
			break;
		case COLLIDER_PLAYER:	//green															
			App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
			break;
		case COLLIDER_ENEMIE:	//red
			App->render->DrawQuad(colliders[i]->rect, 255, 0, 0, alpha);	
			break;
		case COLLIDER_PICKUP:		//pink
			App->render->DrawQuad(colliders[i]->rect, 200, 0, 200, alpha);
		}
	}
}

Collider* j1Collisions::AddCollider(SDL_Rect rect, COLLIDER_TYPE type, j1Module* callback)
{
	Collider* ret = nullptr;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		
		if (colliders[i] == nullptr)
		{
			ret = colliders[i] = new Collider(rect, type, callback);
			break;
		}
	}
	return ret;
}

bool j1Collisions::EraseCollider(Collider* collider)
{
	if (collider != nullptr)
	{
		for (uint i = 0; i < MAX_COLLIDERS; ++i)
		{
			if (colliders[i] == collider)
			{
				collider->to_delete = true;
				break;
			}
		}
	}
	return false;
}

CollisionDirection Collider::CheckDirection(const SDL_Rect& r) const {

	uint right_surface, left_surface;
	uint up_surface, down_surface;	
	right_surface = left_surface =2;// to evite some problems when it collides, giving priority to left and right 
	up_surface = 4;// giving more priority to up

	if (r.y >= rect.y) { //excluding below
		if (r.x <= rect.x) { // excluding left
			up_surface += (r.x + r.w) - rect.x;
			right_surface += (rect.y + rect.h) - r.y;
			if(right_surface>=App->map->data.tile_height) { return ENTITY_RIGHT; }
			if (up_surface > right_surface) { return ENTITY_ABOVE; } // compares with which side the player has more surface with the collider and return its direction; 
			if (right_surface > up_surface) { return ENTITY_RIGHT; }
		}
		if (r.x > rect.x) { // excluding right
			up_surface += (rect.x + rect.w) - r.x;
			left_surface += (rect.y + rect.h) - r.y;
			if (left_surface >= App->map->data.tile_height) { return ENTITY_LEFT; }
			if (up_surface > left_surface) { return ENTITY_ABOVE; }
			if (left_surface > up_surface) { return ENTITY_LEFT; }
		}
	}
	if (r.y < rect.y) { //excluding above
		if (r.x <= rect.x) { // excluding left
			down_surface = (r.x + r.w) - rect.x;
			right_surface += (r.y + r.h) - rect.y;	
			if (right_surface >= App->map->data.tile_height) { return ENTITY_RIGHT; }
			if (down_surface > right_surface) { return ENTITY_BELOW; }
			if (right_surface > down_surface) { return ENTITY_RIGHT; }
		}
		if (r.x > rect.x) { // excluding right
			down_surface = (rect.x + rect.w) - r.x;
			left_surface += (r.y + r.h) - rect.y;		
			if (left_surface >= App->map->data.tile_height) { return ENTITY_LEFT; }
			if (down_surface > left_surface) { return ENTITY_BELOW; }
			if (left_surface > down_surface) { return ENTITY_LEFT; }
		}
	}	 
}

bool Collider::CheckCollision(const SDL_Rect& r) const {
	int margin = 0;
	return (rect.x < r.x + r.w + margin
		&&	rect.x + rect.w +margin> r.x
		&&	 rect.y < r.y + r.h +margin 
		&&	rect.h + rect.y +margin> r.y);

}
