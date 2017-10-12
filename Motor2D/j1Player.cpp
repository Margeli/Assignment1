#include "j1Player.h"
#include "p2Defs.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Animation.h"

j1Player::j1Player() : j1Module()
{
	graphics = nullptr;

	idle.PushBack({ 0, 0, 178, 249 });
	idle.PushBack({ 180, 0, 178, 249 });
	idle.PushBack({ 360, 0, 178, 249 });
	idle.PushBack({ 540, 0, 178, 249 });
	idle.PushBack({ 720, 0, 178, 249 });
	idle.PushBack({ 900, 0, 178, 249 });
	idle.PushBack({ 1080, 0, 178, 249 });
	idle.PushBack({ 1260, 0, 178, 249 });
	idle.PushBack({ 1620, 0, 178, 249 });
	idle.PushBack({ 1800, 0, 178, 249 });
	idle.loop = true;
	idle.speed = 0.1f;

	jump.PushBack({ 0, 250, 178, 249 });
	jump.PushBack({ 180, 250, 178, 249 });
	jump.PushBack({ 360, 250, 178, 249 });
	jump.PushBack({ 540, 250, 178, 249 });
	jump.PushBack({ 720, 250, 178, 249 });
	jump.PushBack({ 900, 250, 178, 249 });
	jump.PushBack({ 1080, 250, 178, 249 });
	jump.PushBack({ 1260, 250, 178, 249 });
	jump.PushBack({ 1620, 250, 178, 249 });
	jump.PushBack({ 1800, 250, 178, 249 });
	jump.loop = true;
	jump.speed = 0.1f;

	walk.PushBack({ 0, 500, 178, 249 });
	walk.PushBack({ 180, 500, 178, 249 });
	walk.PushBack({ 360, 500, 178, 249 });
	walk.PushBack({ 540, 500, 178, 249 });
	walk.PushBack({ 720, 500, 178, 249 });
	walk.PushBack({ 900, 500, 178, 249 });
	walk.PushBack({ 1080, 500, 178, 249 });
	walk.PushBack({ 1260, 500, 178, 249 });
	walk.PushBack({ 1620, 500, 178, 249 });
	walk.PushBack({ 1800, 500, 178, 249 });
	walk.loop = true;
	walk.speed = 0.1f;

	
}

j1Player::~j1Player()
{}

bool j1Player::Start()
{
	bool ret = true;
	LOG("Loading player.");

	graphics = App->tex->Load("textures/character_spritesheet.png");

	if (!graphics)
	{
		LOG("Error loading player textures");
		ret = false;
	}
	
	position = { 0,0 };
	current_animation = &idle;

	return ret;
}

bool j1Player::CleanUp()
{
	LOG("Unloading player.");

	App->tex->UnLoad(graphics);

	return true;

}
bool j1Player::Update()
{
	// Draw everything

	App->render->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()));

	return true;
}