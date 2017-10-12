	#include "j1Player.h"
#include "p2Defs.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "Animation.h"

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

	idle.PushBack({ 0, 250, 178, 249 });
	idle.PushBack({ 180, 250, 178, 249 });
	idle.PushBack({ 360, 250, 178, 249 });
	idle.PushBack({ 540, 250, 178, 249 });
	idle.PushBack({ 720, 250, 178, 249 });
	idle.PushBack({ 900, 250, 178, 249 });
	idle.PushBack({ 1080, 250, 178, 249 });
	idle.PushBack({ 1260, 250, 178, 249 });
	idle.PushBack({ 1620, 250, 178, 249 });
	idle.PushBack({ 1800, 250, 178, 249 });
	idle.loop = true;
	idle.speed = 0.1f;
}

j1Player::~j1Player()
{}

bool j1Player::Awake(pugi::xml_node& conf) {

	LOG("Loading player");

	graphics = App->tex->Load("game/textures/Character Sprites.png");
	


	return true;
}

bool j1Player::CleanUp()
{
	LOG("Unloading player");

	App->tex->UnLoad(graphics);

	return true;

}
bool j1Player::Update()
{

	return true;
}