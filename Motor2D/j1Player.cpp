#include "j1Player.h"
#include "p2Defs.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Animation.h"
#include "j1Input.h"

j1Player::j1Player() : j1Module()
{
	name.create("player");
	graphics = nullptr;

	idle.PushBack({ 0, 0, 63, 88 });
	idle.PushBack({ 63, 0, 63, 88 });
	idle.PushBack({ 126, 0, 63, 88 });
	idle.PushBack({ 189, 0, 63, 88 });
	idle.PushBack({ 252, 0, 63, 88 });
	idle.PushBack({ 317, 0, 63, 88 });
	idle.PushBack({ 382, 0, 63, 88 });
	idle.PushBack({ 446, 0, 63, 88 });
	idle.PushBack({ 510, 0, 63, 88 });
	idle.PushBack({ 572, 0, 63, 88 });	
	idle.loop = true;
	idle.speed = 0.07f;


	jump.PushBack({ 0, 88, 63, 88 });
	jump.PushBack({ 63, 88, 63, 88 });
	jump.PushBack({ 126, 88, 63, 88 });
	jump.PushBack({ 189, 88, 63, 88 });
	jump.PushBack({ 252, 88, 63, 88 });
	jump.PushBack({ 317, 88, 63, 88 });
	jump.PushBack({ 382, 88, 63, 88 });
	jump.PushBack({ 446, 88, 63, 88 });
	jump.PushBack({ 510, 88, 63, 88 });
	jump.loop = false;
	jump.speed = 0.05f;

	walk.PushBack({ 0, 177, 63, 88 });
	walk.PushBack({ 63, 177, 63, 88 });
	walk.PushBack({ 126, 177, 63, 88 });
	walk.PushBack({ 189, 177, 63, 88 });
	walk.PushBack({ 252, 177, 63, 88 });
	walk.PushBack({ 317, 177, 63, 88 });
	walk.PushBack({ 382, 177, 63, 88 });
	walk.PushBack({ 446, 177, 63, 88 });
	walk.PushBack({ 510, 177, 63, 88 });
	walk.loop = true;
	walk.speed = 0.06f;
}

j1Player::~j1Player()
{}

bool j1Player::Awake(pugi::xml_node& conf)
{
	return true;
}

bool j1Player::Start() 
{
	bool ret = true;
	LOG("Loading player.");

	graphics = App->tex->Load("textures/character_spritesheet.png");
	playercoll = App->collis->AddCollider({ position.x + 00, position.y, 00, 23 }, COLLIDER_PLAYER, this);	//CHANGE POSITION!!!!!

	if (!graphics)
	{
		LOG("Error loading player textures");
		ret = false;
	}
	
	position = { 0,0 };
	speed = 1.0f;
	current_animation = &idle;
	jump_speed = 8;

	return ret;
}

bool j1Player::CleanUp()
{
	LOG("Unloading player.");

	App->tex->UnLoad(graphics);

	return true;

}

bool j1Player::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {
		position.y -= speed;
	
	}
		
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
	
		position.y += speed;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)//pressed
	{
		position.x -= speed;
		if (current_animation != &jump)
		current_animation = &walk;
	}

	else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP || App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)//after pressed
	{		
		current_animation = &idle;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		position.x+= speed;
		if (current_animation != &jump)
		current_animation = &walk;
	}

	else if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP || App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
	{		
		current_animation = &idle;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) 
	{

	current_animation = &jump;
	jump_increment = position.y;
	}

	else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && landing == false)
	{
 	Jump();

	}

	else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
	{
		current_animation = &idle;
		landing = false;
	}


	// Draw everything

	App->render->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()));

	return true;
}

void j1Player::Jump() {

	
	if (position.y - jump_increment > -90)//jump height
		position.y -= jump_speed;
	else
		landing = true;

}