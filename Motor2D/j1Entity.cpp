#include "j1Entity.h"
#include "j1App.h"
#include "j1Render.h"

j1Entity::j1Entity(EntityTypes type) : type(type)
{
}
void j1Entity::Draw() 
{
	position = { (int)fposition.x, (int)fposition.y };
	App->render->Blit(sprites, position.x, position.y, &(animation->GetCurrentFrame()));
}

void j1Entity::SetInitialPos() 
{
	position = initial_pos;
	fposition = { (float)position.x, (float)position.y };

}
