#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collisions.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Audio.h"
#include "j1Player.h"
#include "p2Defs.h"
#include "j1Animation.h"

#include "SDL/include/SDL_timer.h"

#define SPEED 1

j1Player::j1Player() : j1Module()
{
	name.create("player");
	graphics = nullptr;

	attack_right.PushBack({0,0,0,0});
	attack_right.loop = false;
	attack_right.speed = 0.07f;

	attack_left.PushBack({ 0,0,0,0 });
	attack_left.loop = false;
	attack_left.speed = 0.07f;

	death_right.PushBack({ 0,0,0,0 });
	death_right.loop = false;
	death_right.speed = 0.07f;

	death_left.PushBack({ 0,0,0,0 });
	death_left.loop = false;
	death_left.speed = 0.07f;

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

	playercoll = App->collis->AddCollider({ position.x, position.y, 46, 60 }, COLLIDER_PLAYER, this);	
	graphics = App->tex->Load("textures/character.png");

	if (!graphics)
	{
		LOG("Error loading player textures");
		ret = false;
	}
	
	speed = SPEED;
	current_animation = &idle;
	jump_speed = 5.0f;
	jump_limit = 70.0f;

	if (sword_sound == 0)
		sword_sound = App->audio->LoadFx("audio/fx/sword_attack.wav");

	if (jump_sound == 0)
		jump_sound = App->audio->LoadFx("audio/fx/player_jump.wav");

	if (playersteps == 0)
		playersteps = App->audio->LoadFx("audio/fx/player_steps.wav");

	return ret;
}

bool j1Player::CleanUp()
{
	LOG("Unloading player.");

	App->tex->UnLoad(graphics);
	
	if (playercoll != nullptr)
		playercoll->to_delete = true;

	return true; 
}

bool j1Player::Update(float dt)
{
	if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == SDL_PRESSED)
	{
		App->audio->PlayFx(sword_sound);

		if (current_animation == &walk || current_animation == &idle)
		{
			current_animation = &attack_right;
		}

		else if (current_animation == &walkleft || current_animation == &idleleft)
		{
			current_animation = &attack_left;
		}
	}
	else if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == SDL_RELEASED)
	{
		if (current_animation == &attack_right)
			current_animation = &idle;
		else if (current_animation == &attack_left)
			current_animation = &idleleft;
	}
	 
	//-------------RIGHT

	if ((App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	{
		App->audio->PlayFx(playersteps);
		if (camera_movement) { App->render->camera.x -= App->render->camera_speed; }
		
		position.x += speed * 1.25f;
		if (current_animation != &jump)
			current_animation = &run;

	}
	

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		App->audio->PlayFx(playersteps);

		if (camera_movement) {
			App->render->camera.x -= App->render->camera_speed;
		}
		position.x+= speed;
		if (current_animation != &jump)
		current_animation = &walk;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP || App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
	{
		current_animation = &idle;
	}

	//-------------LEFT
	if ((App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	{
		if (camera_movement) { App->render->camera.x -= App->render->camera_speed; }

		position.x -= speed * 1.0f;
		if (current_animation != &jump)
			current_animation = &run;
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		App->audio->PlayFx(playersteps);

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


	//-------------JUMP


	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
	{
		if (current_animation == &jump)
			current_animation = &idle;
		else if (current_animation == &jumpleft)
			current_animation = &idleleft;		

	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT))
	{
		App->audio->PlayFx(jump_sound);

		if(current_animation == &jump)
		current_animation = &walkleft;
	}
	else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT && (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT))
	{
		App->audio->PlayFx(jump_sound);

		if (current_animation == &jump)
			current_animation = &walk;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		App->audio->PlayFx(jump_sound);

		if (current_animation == &walkleft || current_animation == &idleleft)
			current_animation = &jumpleft;
		else if (current_animation == &walk || current_animation == &idle)
			current_animation = &jump;

		

		if (jumping== false)
			can_jump = true;

		if (jumping && double_jump) {
			can_jump = true;
			double_jump = false;
			jump_pos = position.y;
		}

		
	}

	if (can_jump) {

		jump_pos = position.y;
		jumping = true;
		can_jump = false;
	}

	if (jumping) {

   		if (jump_pos -position.y   < jump_limit) {
			position.y -= jump_speed;
			
		}
		else{
			jumping = false;
			
			if (!double_jump)
				double_jump = true;
		}

	}	
	//--------

	if (playercoll!=nullptr) { playercoll->SetPos(position.x, position.y + 5); }

	App->render->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()));

	return true;
}

void j1Player::InitialPos() 
{
	position = { 50,100 };
}

//void j1Player::Landing()
//{	
//}

void j1Player::OnCollision(Collider* c1, Collider* c2) {
	int margin = 0;
	switch (c2->type) {
	case COLLIDER_GROUND:

		switch (c1->CheckDirection(c2->rect)) {

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
		//landing = true;
		break;
	}
}

bool j1Player::Load(pugi::xml_node& data)
{
	position.x = data.child("position").attribute("x").as_int();
	position.y = data.child("position").attribute("y").as_int();

	return true;
}

bool j1Player::Save(pugi::xml_node& data) const
{
	pugi::xml_node pos= data.append_child("position");

	pos.append_attribute("x") = position.x;
	pos.append_attribute("y") = position.y;

	return true;
}