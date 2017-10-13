#include "j1Collisions.h"
#include "p2Defs.h"
#include "j1Module.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Player.h"
#include "p2Log.h"


#include "SDL\include\SDL.h"


j1Collisions::j1Collisions(): j1Module()
{
	name.create("collisions");

	matrix[COLLIDER_PLAYER][COLLIDER_GROUND] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;

	matrix[COLLIDER_GROUND][COLLIDER_GROUND] = false;
	matrix[COLLIDER_GROUND][COLLIDER_PLAYER] = true;
}

j1Collisions::~j1Collisions(){}

bool j1Collisions::PreUpdate() {

	for (uint i = 0; i < MAX_COLLIDERS; ++i)//prepares collider's array
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return true;
}


bool j1Collisions::CleanUp() {
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

	Collider* c1;
	Collider* c2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr)
			continue;
		if (colliders[i]->type == COLLIDER_PLAYER) {

			c1 = colliders[i];

			for (uint k = 0; k < MAX_COLLIDERS; ++k)
			{
				if (colliders[k] == nullptr)
					continue;
				
					c2 = colliders[k];

					if (c1->CheckCollision(c2->rect) == true)
					{

						c1->direction = c1->CheckDirection(c2->rect);
						if (matrix[c1->type][c2->type] && c1->callback)
							c1->callback->OnCollision(c1, c2, c1->direction);

						//if (matrix[c2->type][c1->type] && c2->callback)
						//	c2->callback->OnCollision(c2, c1);
					
				}
			}
		}
	}

	DebugDraw();

	return true;

}

void j1Collisions::DebugDraw() {

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
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

CollisionDirection Collider::CheckDirection(const SDL_Rect& r) 
{
		if ((rect.y < r.y + r.h) && (rect.y > r.y)) {
				return PLAYER_BELOW;
			}
		if ((rect.h + rect.y > r.y) && (rect.h + rect.y < r.y + r.h)) {
				return PLAYER_ABOVE;
			}
		
		if ((rect.x < r.x + r.w)&&(rect.x>r.x)) {
			return PLAYER_RIGHT;
		}
		if ((rect.x + rect.w > r.x)&&(rect.x+rect.w < r.x+r.w)) {
			return PLAYER_LEFT;
		}
		
		
	 
}
bool Collider::CheckCollision(const SDL_Rect& r) const {
	int margin = 0;
	return (rect.x < r.x + r.w + margin
		&&	rect.x + rect.w +margin> r.x
		&&	 rect.y < r.y + r.h +margin 
		&&	rect.h + rect.y +margin> r.y);

}


//void GroupUpColliders() {
//	for (uint i = 0; i < MAX_COLLIDERS; ++i){
//		if(collider)
//
//
//
//}
//}