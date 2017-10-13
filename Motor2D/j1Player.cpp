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
	jump.speed = 0.1f;

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
	playercoll = App->collis->AddCollider({ position.x, position.y, 46, 65 }, COLLIDER_PLAYER, this);	//CHANGE POSITION!!!!!

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

	if(playercoll!=nullptr) //updates the collider to player's position
	playercoll->SetPos(position.x, position.y);


	// Draw everything

	App->render->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()));

	return true;
}

void j1Player::Jump()
{
		if (position.y - jump_increment > -90)//jump height
			position.y -= jump_speed;
		else
			landing = true;

}

void j1Player::OnCollision(Collider* c1, Collider* c2, CollisionDirection direction) {

	switch (c2->type) {
	case COLLIDER_GROUND:
		
		switch (direction) {
		case PLAYER_ABOVE:
 			position.y = c2->rect.y - 65;//player height
			break;
		case PLAYER_BELOW:
			position.y = c2->rect.y + c2->rect.h;
			break;
		case PLAYER_RIGHT:
			position.x = c2->rect.x + c2->rect.w ; 
			break;
		case PLAYER_LEFT:
			position.x = c2->rect.x - 46;//playr width
			break;
			//if (c1->rect.y + c1->rect.h >= c2->rect.y) { //player above collider ground
			//	position.y = c2->rect.y - 65;//playr height
			//}
			//else if (c1->rect.y < c2->rect.y + c2->rect.h) { // player below collider ground
			//	position.y = c2->rect.y + c2->rect.h;
			//}
			//
			//if (c1->rect.x + c1->rect.w >= c2->rect.x) {
			//	position.x = c2->rect.x - 46;//playr height
			//}
			//else if (c1->rect.x < c2->rect.x + c2->rect.w) {
			//	position.x = c2->rect.x + c2->rect.w + 46; //playr width
			//}
		}
		break;
	}
}