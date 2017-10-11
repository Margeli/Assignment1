#include "j1Player.h"
#include "p2Defs.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"

j1Player::j1Player() : j1Module()
{



}

j1Player::~j1Player()
{}

bool j1Player::Awake(pugi::xml_node& conf) {


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