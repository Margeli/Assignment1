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

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
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

		c1 = colliders[i];

		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			if (colliders[k] == nullptr)
				continue;

			c2 = colliders[k];

			if (c1->CheckCollision(c2->rect) == true)
			{
				if (matrix[c1->type][c2->type] && c1->callback)
					c1->callback->OnCollision(c1, c2);

				if (matrix[c2->type][c1->type] && c2->callback)
					c2->callback->OnCollision(c2, c1);
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
		case COLLIDER_NONE:																//Only this case?? what about COLLIDER PLAYER and COLLIDER GROUND?
			App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
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

bool Collider::CheckCollision(const SDL_Rect& r) const
{
	return (rect.x < r.x + r.w &&
		rect.x + rect.w > r.x &&
		rect.y < r.y + r.h &&
		rect.h + rect.y > r.y);
}