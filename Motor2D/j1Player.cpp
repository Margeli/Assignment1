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

	idle.PushBack({ 0, 0, 46, 65 });
	idle.PushBack({ 47, 0, 46, 65 });
	idle.PushBack({ 94, 0, 46, 65 });
	idle.PushBack({ 141, 0, 46, 65 });
	idle.PushBack({ 188, 0, 46, 65 });
	idle.PushBack({ 235, 0, 46, 65 });
	idle.PushBack({ 282, 0, 46, 65 });
	idle.PushBack({ 329, 0, 46, 65 });
	idle.PushBack({ 376, 0, 46, 65 });
	idle.PushBack({ 423, 0, 46, 65 });
	idle.loop = true;
	idle.speed = 0.07f;


	jump.PushBack({ 0, 65, 46, 65 });
	jump.PushBack({ 47, 65, 46, 65 });
	jump.PushBack({ 94, 65, 46, 65 });
	jump.PushBack({ 141, 65, 46, 65 });
	jump.PushBack({ 188, 65, 46, 65 });
	jump.PushBack({ 235, 65, 46, 65 });
	jump.PushBack({ 282, 65, 46, 65 });
	jump.PushBack({ 329, 65, 46, 65 });
	jump.PushBack({ 376, 65, 46, 65 });
	jump.PushBack({ 423, 65, 46, 65 });
	jump.loop = false;
	jump.speed = 0.06f;

	walk.PushBack({ 0, 130, 46, 65 });
	walk.PushBack({ 47, 130, 46, 65 });
	walk.PushBack({ 94, 130, 46, 65 });
	walk.PushBack({ 141, 130, 46, 65 });
	walk.PushBack({ 188, 130, 46, 65 });
	walk.PushBack({ 235, 130, 46, 65 });
	walk.PushBack({ 282, 130, 46, 65 });
	walk.PushBack({ 329, 130, 46, 65 });
	walk.PushBack({ 376, 130, 46, 65 });
	walk.PushBack({ 423, 130, 46, 65 });
	walk.loop = true;
	walk.speed = 0.06f;

	idleleft.PushBack({ 0, 195, 46, 65 });
	idleleft.PushBack({ 47, 195, 46, 65 });
	idleleft.PushBack({ 94, 195, 46, 65 });
	idleleft.PushBack({ 141, 195, 46, 65 });
	idleleft.PushBack({ 188, 195, 46, 65 });
	idleleft.PushBack({ 235, 195, 46, 65 });
	idleleft.PushBack({ 282, 195, 46, 65 });
	idleleft.PushBack({ 329, 195, 46, 65 });
	idleleft.PushBack({ 376, 195, 46, 65 });
	idleleft.PushBack({ 423, 195, 46, 65 });
	idleleft.loop = true;
	idleleft.speed = 0.07f;


	jumpleft.PushBack({ 0, 260, 46, 65 });
	jumpleft.PushBack({ 47, 260, 46, 65 });
	jumpleft.PushBack({ 94, 260, 46, 65 });
	jumpleft.PushBack({ 141, 260, 46, 65 });
	jumpleft.PushBack({ 188, 260, 46, 65 });
	jumpleft.PushBack({ 235, 260, 46, 65 });
	jumpleft.PushBack({ 282, 260, 46, 65 });
	jumpleft.PushBack({ 329, 260, 46, 65 });
	jumpleft.PushBack({ 376, 260, 46, 65 });
	jumpleft.PushBack({ 423, 260, 46, 65 });
	jumpleft.loop = false;
	jumpleft.speed = 0.06f;

	walkleft.PushBack({ 0, 326, 46, 65 });
	walkleft.PushBack({ 47, 326, 46, 65 });
	walkleft.PushBack({ 94, 326, 46, 65 });
	walkleft.PushBack({ 141, 326, 46, 65 });
	walkleft.PushBack({ 188, 326, 46, 65 });
	walkleft.PushBack({ 235, 326, 46, 65 });
	walkleft.PushBack({ 282, 326, 46, 65 });
	walkleft.PushBack({ 329, 326, 46, 65 });
	walkleft.PushBack({ 376, 326, 46, 65 });
	walkleft.PushBack({ 423, 326, 46, 65 });
	walkleft.loop = true;
	walkleft.speed = 0.06f;

	run.PushBack({ 0, 130, 46, 65 });
	run.PushBack({ 47, 130, 46, 65 });
	run.PushBack({ 94, 130, 46, 65 });
	run.PushBack({ 141, 130, 46, 65 });
	run.PushBack({ 188, 130, 46, 65 });
	run.PushBack({ 235, 130, 46, 65 });
	run.PushBack({ 282, 130, 46, 65 });
	run.PushBack({ 329, 130, 46, 65 });
	run.PushBack({ 376, 130, 46, 65 });
	run.PushBack({ 423, 130, 46, 65 });
	run.loop = true;
	run.speed = 2.0f;

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



	//graphics = App->tex->Load("textures/character_spritesheet.png");
	playercoll = App->collis->AddCollider({ position.x, position.y, 46, 60 }, COLLIDER_PLAYER, this);	//CHANGE POSITION!!!!!

	graphics = App->tex->Load("textures/character_spritesheet_left.png");




	if (!graphics)
	{
		LOG("Error loading player textures");
		ret = false;
	}
	
	position = { 200,100 };
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
	if (App->input->GetKey(SDL_SCANCODE_UP) == KEY_REPEAT) {position.y -= speed;}
		
	if (App->input->GetKey(SDL_SCANCODE_DOWN) == KEY_REPEAT) {
	
		position.y += speed;
	}
	
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		position.x -= speed;
		if (current_animation != &jump)
		current_animation = &walkleft;
	}
	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP || App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
	{
		if (current_animation = &walkleft)
			current_animation = &idleleft;
		else if (current_animation = &walk)
			current_animation = &idle;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		position.x+= speed;
		if (current_animation != &jump)
		current_animation = &walk;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP || App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
	{
		current_animation = &idle;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN) 
	{
		if (current_animation == &walkleft || current_animation == &idleleft)
			current_animation = &jumpleft;
		else if (current_animation == &walk || current_animation == &idle)
			current_animation = &jump;

		jump_increment = position.y;
	}

	else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && landing == false)
	{
		Jump();
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
	{
		if (current_animation == &jump)
			current_animation = &idle;
		else if (current_animation == &jumpleft)
			current_animation = &idleleft;
		landing = false;
	}

	if(playercoll!=nullptr) //updates the collider to player's position
	playercoll->SetPos(position.x, position.y+5);


	App->render->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()));

	return true;
}

void j1Player::Jump()
{
	if (position.y - jump_increment > -90)//jump height
	{
		position.y -= jump_speed;
	}
	else
			landing = true;
}


void j1Player::OnCollision(Collider* c1, Collider* c2, CollisionDirection direction) {
	int margin = 0;
	switch (c2->type) {
	case COLLIDER_GROUND:

		switch (direction) {

		case PLAYER_ABOVE:
			position.y = c2->rect.y - 65 - margin;//player height //margin
			break;
		case PLAYER_BELOW:
			position.y = c2->rect.y + c2->rect.h + margin;
			break;
		case PLAYER_RIGHT:
			position.x = c2->rect.x + c2->rect.w + margin;
			break;
		case PLAYER_LEFT:
			position.x = c2->rect.x - 46 - margin;//playr width
			break;

		}
		break;
	}
}

