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
#include "Brofiler/Brofiler.h"

#include "j1EntityManager.h"

#include "SDL/include/SDL_timer.h"



j1Player::j1Player() : j1Entity(EntityTypes::PLAYER)
{


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

	collider = App->collis->AddCollider({ position.x, position.y, 46, 60 }, COLLIDER_PLAYER, App->entities);	
	sprites = App->tex->Load("textures/character.png");
	if (!sprites)	{
		LOG("Error loading player textures");
		ret = false;
	}

	lose_fx = App->audio->LoadFx("audio/fx/lose.wav");
	hurt_fx = App->audio->LoadFx("audio/fx/player_hurt.wav");
	die_fx = App->audio->LoadFx("audio/fx/player_death.wav");

	speed = SPEED;
	animation = &idle;
	jump_speed = 4.5f;
	jump_limit = 70.0f;

	if (sword_sound == 0)
		sword_sound = App->audio->LoadFx("audio/fx/sword_attack.wav");

	if (jump_sound == 0)
		jump_sound = App->audio->LoadFx("audio/fx/player_jump.wav");

	if (playersteps == 0)
		playersteps = App->audio->LoadFx("audio/fx/player_steps.wav");

	JumpReset();

	return ret;
}

bool j1Player::CleanUp()
{
	LOG("Unloading player.");

	App->tex->UnLoad(sprites);
	
	if (collider != nullptr)
		collider->to_delete = true;
	
	return true; 
}

bool j1Player::Update(float dt)
{
	BROFILER_CATEGORY("Player_Update", Profiler::Color::Azure);
	if (use_input) 
	{
		if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN) { godmode = true; }

		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_REPEAT) //----------------ATTACK MOVEMENT
		{
			App->audio->PlayFx(sword_sound);
			if (facing = RIGHT) { animation = &attack_right; }
			else if (facing = LEFT) { animation = &attack_left; }
		}
		else if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_UP )
		{
			if (animation == &attack_right) { attack_right.Reset(); animation = &idle; }
			else if (animation == &attack_left) { attack_left.Reset(); animation = &idleleft; }
		}

		if (((App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)) //RUNING RIGHT
		{
			position.x += speed * 1.25f;
			walking = true;
			facing = RIGHT;

			if (camera_movement) { App->render->camera.x -= App->render->camera_speed; }
			if (animation != &jump) { animation = &run; }
		}
		

		if ((App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT))//-----------WALKING RIGHT
		{
			position.x += speed;
			walking = true;
			facing = RIGHT;

			if (camera_movement) { App->render->camera.x -= App->render->camera_speed; }
			if (animation != &jump) {animation = &walk; }
		}

		if (((App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)) 	//BETTER GAMEPLAY
			animation = &walk;
		
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP || App->input->GetKey(SDL_SCANCODE_D) == KEY_UP ) 
			animation = &idle; walking = false; 
	
		if ((App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) 	//RUNNING LEFT
		{
			position.x -= speed * 1.00f;
			walking = true;
			facing = LEFT;
			if (camera_movement) { App->render->camera.x -= App->render->camera_speed; }
			if (animation != &jump) { animation = &run; }
		}
		
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) //------------WALKING LEFT
		{
			walking = true;
			position.x -= speed;
			facing = LEFT;
			if (animation != &jump) { animation = &walkleft; }
		}

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP || App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
		{
			walking = false;
			if (animation == &walkleft) { animation = &idleleft; }	
			else if (animation == &walk) { animation = &idle; }
		}

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_UP)
		{
			if (animation == &jump) { animation = &idle; }
			else if (animation == &jumpleft) { animation = &idleleft; }
		}

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT) )
		{
			if (animation == &jump) { animation = &walkleft; }
		}
		else if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN && (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT) )
		{
			if (animation == &jump) { animation = &walk; }
		}

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			if (animation == &walkleft || animation == &idleleft) {animation = &jumpleft; }
				
			else if (animation == &walk || animation == &idle) { animation = &jump; }
				
			if (jumping == false && landing == false) { can_jump = true; }
				
			if (jumping == true && double_jump == true) { can_jump = true; double_jump = false; }
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
			if ((jump_pos - position.y < jump_limit) && !landing) { position.y -= jump_speed; }
			else { landing = true; }
		}
	}//----------------------------

	if (hit && hit_time - SDL_GetTicks() > 1000) { hit = false; } // 1s of invulnerability  if hitted
	
	if (collider != nullptr) { collider->SetPos(position.x, position.y + 5); }

	if (player_hurted && animation->Finished() == true) { LoseOneLife(); }

	if (lifes < 1) { Dead(); }
		
	if (points > max_score) { max_score = points; }

	Draw();
	
	return true;
}

void j1Player::Dead()
{
	App->audio->PlayFx(lose_fx, 0);
	lifes = LIFES;
	points = 0;
	
	if (App->scene->active) { position = App->scene->initial_scene_pos; }
	else if (App->scene2->active) { App->scene2->SceneChange(); }
}

void j1Player::PlayerHurted() 
{
	player_hurted = true;
	hit_time = SDL_GetTicks();
	use_input = false;
	walking = false;
	App->audio->PlayFx(die_fx);

	if (facing == RIGHT) { animation = &death_right; }

	else if (facing == LEFT) { animation = &death_left; }
}


void j1Player::LoseOneLife() 
{
	
		if (App->scene->active) { position = App->scene->initial_scene_pos; }

		if (App->scene2->active) { position = App->scene2->initial_scene_pos; }

		App->render->camera.x = 0;
		animation->Reset();
		animation = &idle;
		lifes--;
		walking = false;
		hit_time = SDL_GetTicks();
		hit = true;
		player_hurted = false;
		use_input = true;
		JumpReset();
}


void j1Player::JumpReset() 
{
	jumping = false;
	landing = true;
	can_jump = false;
	double_jump = true;
}

void j1Player::OnCollision(Collider* c1, Collider* c2)
{
	int margin = 2;
	if (!hit) {
		switch (c2->type)
		{
		case COLLIDER_ENEMIE:
			if(player_hurted == false && godmode == false)
			PlayerHurted();
			break;
		case COLLIDER_GROUND:
			switch (c1->CheckDirection(c2->rect))
			{
			case PLAYER_ABOVE:
				position.y = c2->rect.y - PLAYERHEIGHT;
				double_jump = true;
				jumping = false;
				landing = false;
				if (walking) { App->audio->PlayFx(playersteps); }
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
	attack_right.LoadPlayerAnimations("attack_right");
	attack_left.LoadPlayerAnimations("attack_left");
	death_right.LoadPlayerAnimations("death_right");
	death_left.LoadPlayerAnimations("death_left");
	idle.LoadPlayerAnimations("idle");
	idleleft.LoadPlayerAnimations("idleleft");
	jump.LoadPlayerAnimations("jump");
	jumpleft.LoadPlayerAnimations("jumpleft");
	walk.LoadPlayerAnimations("walk");
	walkleft.LoadPlayerAnimations("walkleft");
	run.LoadPlayerAnimations("run");
}