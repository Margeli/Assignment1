#include "j1App.h"
#include "j1Enemy.h"
#include "j1Collisions.h"
#include "j1Render.h"

j1Enemy::j1Enemy(int x, int y) : position(x, y)
{}

j1Enemy::~j1Enemy()
{
	if (collider != nullptr)
		collider->to_delete = true;
}

const Collider* j1Enemy::GetCollider() const
{
	return collider;
}

void j1Enemy::Draw(SDL_Texture* sprites)
{
	if (collider != nullptr)
		collider->SetPos(position.x, position.y);

	if (animation != nullptr)
		App->render->Blit(sprites, position.x, position.y, &(animation->GetCurrentFrame()));
}

void j1Enemy::OnCollision(Collider* collider, int num_enemy)
{

}