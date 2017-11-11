/*#include "j1App.h"
#include "j1Enemy.h"
#include "j1Collisions.h"
#include "j1Render.h"
#include "j1EntityManager.h"

j1Enemy::j1Enemy() 
{
}

j1Enemy::~j1Enemy()
{
	
}

const Collider* j1Enemy::GetCollider() const
{
	return enemie_collider;
}

void j1Enemy::Draw(SDL_Texture* sprites)
{
	if (enemie_collider != nullptr)
		enemie_collider->SetPos(position.x, position.y);

	if (animation != nullptr)
		App->render->Blit(sprites, position.x, position.y, &(animation->GetCurrentFrame()));
}


*/

