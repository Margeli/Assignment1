#include "j1App.h"
#include "j1Textures.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Collisions.h"
#include "p2Log.h"
#include "j1Window.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1Player.h"
#include "p2Defs.h"
#include "j1Animation.h"

#include "SDL/include/SDL_timer.h"

#define SPEED 1
#define LIFES 5
#define PLAYERHEIGHT 65
#define PLAYERWIDTH 45

j1Player::j1Player() : j1Module()
{
	name.create("player");
	graphics = nullptr;
	lifes = LIFES;

	LoadPlayerAnimations();
		
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
	if (!graphics)	{
		LOG("Error loading player textures");
		ret = false;
	}

	lose_fx = App->audio->LoadFx("audio/fx/lose.wav");
	hurt_fx = App->audio->LoadFx("audio/fx/player_hurt.wav");
	die_fx = App->audio->LoadFx("audio/fx/player_death.wav");

	speed = SPEED;
	current_animation = &idle;
	jump_speed = 4.5f;
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
	if (lifes == 0) 
		Dead(); 
	
	if (points > max_score)	
		max_score = points;
	

	///////ATTACK MOVEMENT///////

	//if (App->input->GetMouseButtonDown(SDL_BUTTON_LEFT) == SDL_PRESSED)
	if(App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN)
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
	else if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_UP)
	{

		if (current_animation == &attack_right){
			attack_right.Reset();
			current_animation = &idle;
	}
		else if (current_animation == &attack_left) {
			attack_left.Reset();
			current_animation = &idleleft;
		}
	}
	else if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_REPEAT) { App->audio->PlayFx(sword_sound); }

	///////RIGHT MOVEMENT///////

	if ((App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	{
		//App->audio->PlayFx(playersteps);
		if (camera_movement) { App->render->camera.x -= App->render->camera_speed; }
		
		position.x += speed * 1.25f;
		if (current_animation != &jump)
			current_animation = &run;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT)
	{
		//App->audio->PlayFx(playersteps);

		if (camera_movement) {
			App->render->camera.x -= App->render->camera_speed;
		}
		position.x+= speed;
		if (current_animation != &jump)
		current_animation = &walk;
	}

	if ((App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)
	{
		current_animation = &walk;
	}

	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP || App->input->GetKey(SDL_SCANCODE_D) == KEY_UP)
	{
		current_animation = &idle;
	}

	///////LEFT MOVEMENT///////

	if ((App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	{
		//App->audio->PlayFx(playersteps);

		if (camera_movement) { App->render->camera.x -= App->render->camera_speed; }

		position.x -= speed * 1.0f;
		if (current_animation != &jump)
			current_animation = &run;
	}

	if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT)
	{
		//App->audio->PlayFx(playersteps);

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

	//---------JUMP MOVEMENT--------

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
	{
		if (current_animation == &jump)
			current_animation = &idle;
		else if (current_animation == &jumpleft)
			current_animation = &idleleft;		
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT))
	{
		if(current_animation == &jump)
			current_animation = &walkleft;
	}
	else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT))
	{		

		if (current_animation == &jump)
			current_animation = &walk;
	}

	if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
	{
		if (current_animation == &walkleft || current_animation == &idleleft)
			current_animation = &jumpleft;
		else if (current_animation == &walk || current_animation == &idle)
			current_animation = &jump;		

		if (jumping== false && landing ==false  )
 			can_jump = true;

		if (jumping == true && double_jump == true  ){
 			can_jump = true;
 			double_jump = false;		
		}		
	}

	if (can_jump)
	{
		App->audio->PlayFx(jump_sound);
		jump_pos = position.y;
		jumping = true;
		landing = false;
		can_jump = false;	
	}

	if (jumping) 
	{
   		if ((jump_pos -position.y < jump_limit) && !landing) { position.y -= jump_speed; }
		else { landing = true; }
	}	

	//----------------

	if (playercoll!=nullptr) { playercoll->SetPos(position.x, position.y + 5); }

	App->render->Blit(graphics, position.x, position.y, &(current_animation->GetCurrentFrame()));

	return true;
}

void j1Player::Dead(){
	
	App->audio->PlayFx(lose_fx, 0);
	App->scene2->SceneChange();
	 
	lifes = LIFES;
	points = 0;
}

void j1Player::LoseOneLife(iPoint respawn_pos) 
{
		App->audio->PlayFx(App->player->die_fx);
		lifes--;
		position = respawn_pos;
		App->render->camera.x = 0;
}

void j1Player::InitialPos() 
{
	position = { 50, 100 };
}

void j1Player::OnCollision(Collider* c1, Collider* c2)
{
	int margin = 2;
	switch (c2->type)
	{
	case COLLIDER_ENEMIE:
		if (App->scene2->active) 
		{
			LoseOneLife(App->scene2->initial_scene_pos);
			break;
		}
		else if (App->scene->active) {
			LoseOneLife(App->scene->initial_scene_pos);
			break;
		}
	case COLLIDER_GROUND:
		switch (c1->CheckDirection(c2->rect))
		{
		case PLAYER_ABOVE:
			position.y = c2->rect.y - PLAYERHEIGHT;
			double_jump = true;
			jumping = false;
			landing = false;
			break;
		case PLAYER_BELOW:
			position.y = c2->rect.y + c2->rect.h;
			jumping = false;
			landing = true;
			break;
		case PLAYER_RIGHT:
			position.x = c2->rect.x + c2->rect.w;
			break;
		case PLAYER_LEFT:
			position.x = c2->rect.x - PLAYERWIDTH - margin;
			break;
		}
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

void j1Player::LoadPlayerAnimations()
{
	attack_right.LoadAnimations("attack_right", this);
	attack_left.LoadAnimations("attack_left", this);
	death_right.LoadAnimations("death_right", this);
	death_left.LoadAnimations("death_left", this);
	idle.LoadAnimations("idle", this);
	idleleft.LoadAnimations("idleleft", this);
	jump.LoadAnimations("jump", this);
	jumpleft.LoadAnimations("jumpleft", this);
	walk.LoadAnimations("walk", this);
	walkleft.LoadAnimations("walkleft", this);
	run.LoadAnimations("run", this);
}