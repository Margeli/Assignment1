#include "j1Collisions.h"
#include "j1Module.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Player.h"
#include "p2Log.h"

#include "SDL\include\SDL.h"

j1Collisions::j1Collisions() 
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i) { colliders[i] = nullptr; }
		
	matrix[COLLIDER_PLAYER][COLLIDER_GROUND] = true;
	matrix[COLLIDER_PLAYER][COLLIDER_PLAYER] = false;
	matrix[COLLIDER_GROUND][COLLIDER_GROUND] = false;
	matrix[COLLIDER_GROUND][COLLIDER_PLAYER] = true;

}

j1Collisions::~j1Collisions()
{
}

bool j1Collisions::PreUpdate()
{
	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] != nullptr && colliders[i]->to_delete == true)
		{
			delete colliders[i];
			colliders[i] = nullptr;
		}
	}

	return 1;
}

bool j1Collisions::UpDate()
{
	Collider* Collider_1;
	Collider* Collider_2;

	for (uint i = 0; i < MAX_COLLIDERS; ++i)
	{
		if (colliders[i] == nullptr) continue;

		Collider_1 = colliders[i];


		for (uint k = i + 1; k < MAX_COLLIDERS; ++k)
		{
			if (colliders[k] == nullptr) continue;

			Collider_2 = colliders[k];

			if (Collider_1->CheckCollision(Collider_2->rect) == true)
			{
				if (Collider_1->to_delete == false && Collider_2->to_delete != true)
				{
					if (matrix[Collider_1->type][Collider_2->type] && Collider_1->callback)
					{
						Collider_1->callback->OnCollision(Collider_1, Collider_2);
					}
	
					if (Collider_1->to_delete == false) 
					{
						if (matrix[Collider_2->type][Collider_1->type] && Collider_2->callback)
						{
							Collider_2->callback->OnCollision(Collider_2, Collider_1);
						}
					}
				}
			}
		}
	}

	DebugDraw();

	return 1;
}

void j1Collisions::DebugDraw()
{
	if (debug == false) {
		return;
	}
	else {

		Uint8 alpha = 80;
		for (uint i = 0; i < MAX_COLLIDERS; ++i)
		{
			if (colliders[i] == nullptr)
				continue;

			switch (colliders[i]->type)
			{
			case COLLIDER_NONE:
				App->render->DrawQuad(colliders[i]->rect, 255, 255, 255, alpha);
				break;
			case COLLIDER_GROUND:
				App->render->DrawQuad(colliders[i]->rect, 0, 0, 255, alpha);
				break;
			case COLLIDER_PLAYER:
				App->render->DrawQuad(colliders[i]->rect, 0, 255, 0, alpha);
				break;
			}
		}
	}
}

bool j1Collisions::CheckCollision(const SDL_Rect & r)
{
	return false;
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
