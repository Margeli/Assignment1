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

	idle.PushBack({ 0, 0, 178, 249 });
	idle.PushBack({ 180, 0, 178, 249 });
	idle.PushBack({ 360, 0, 178, 249 });
	idle.PushBack({ 540, 0, 178, 249 });
	idle.PushBack({ 360, 0, 178, 249 });
	idle.PushBack({ 180, 0, 178, 249 });
	idle.PushBack({ 180, 0, 178, 249 });
	idle.PushBack({ 360, 0, 178, 249 });
	idle.PushBack({ 540, 0, 178, 249 });
	idle.PushBack({ 720, 0, 178, 249 });
	//idle.PushBack({ 900, 0, 178, 249 });
	//idle.PushBack({ 1080, 0, 178, 249 });
	//idle.PushBack({ 1260, 0, 178, 249 });
	//idle.PushBack({ 1620, 0, 178, 249 });
	idle.loop = true;
	idle.speed = 0.07f;

	jump.PushBack({ 0, 250, 178, 249 });
	jump.PushBack({ 180, 250, 178, 249 });
	jump.PushBack({ 360, 250, 178, 249 });
	jump.PushBack({ 540, 250, 178, 249 });
	jump.PushBack({ 720, 250, 178, 249 });
	jump.PushBack({ 900, 250, 178, 249 });
	jump.PushBack({ 1080, 250, 178, 249 });
	jump.PushBack({ 1260, 250, 178, 249 });
	jump.PushBack({ 1620, 250, 178, 249 });
	jump.loop = false;
	jump.speed = 0.05f;

	walk.PushBack({ 0, 500, 178, 249 });
	walk.PushBack({ 180, 500, 178, 249 });
	walk.PushBack({ 360, 500, 178, 249 });
	walk.PushBack({ 540, 500, 178, 249 });
	walk.PushBack({ 720, 500, 178, 249 });
	walk.PushBack({ 900, 500, 178, 249 });
	walk.PushBack({ 1080, 500, 178, 249 });
	walk.PushBack({ 1260, 500, 178, 249 });
	walk.PushBack({ 1620, 500, 178, 249 });
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
		current_animation = &walk;
	}

	else if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP || App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)//after pressed
	{		
		current_animation = &idle;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		position.x+= speed;
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

	
	if (position.y - jump_increment > -100)
		position.y -= 10;
	else
		landing = true;

}