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

#define JUMP_SPEED 6.5f
#define JUMP_LIMIT 50.0f

j1Player::j1Player() : j1Entity(EntityTypes::PLAYER)
{
	lifes = LIFES;
	LoadPlayerAnimations();
	name.create("player");
}

j1Player::~j1Player() 
{}

bool j1Player::Awake(pugi::xml_node& conf)
{ return true; }

bool j1Player::Start() 
{
	bool ret = true;
	LOG("Loading player.");
	
	speed = SPEED;
	animation = &idle_right;
	jump_speed = JUMP_SPEED;
	jump_limit = JUMP_LIMIT;

	fposition = {(float) position.x, (float)position.y };
	InitialPlayerPos();

	collider = App->collis->AddCollider({ position.x, position.y, 46, 60 }, COLLIDER_PLAYER, App->entities);	

	sprites = App->tex->Load("textures/character.png");
	if (!sprites) { LOG("Error loading player textures");  ret = false; }

	lose_fx = App->audio->LoadFx("audio/fx/lose.wav");
	die_fx = App->audio->LoadFx("audio/fx/player_death.wav");
	troll_death = App->audio->LoadFx("audio/fx/troll_death.wav");

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
	if (collider != nullptr) {	collider->to_delete = true; }
	
	return true; 
}

bool j1Player::Update(float dt)
{	
	BROFILER_CATEGORY("Player_Update", Profiler::Color::Azure);
	speed = SPEED + SPEED *dt;
	fposition.y += GRAVITY + GRAVITY*dt;
	if (use_input) 
	{
		if (App->input->GetKey(SDL_SCANCODE_F9) == KEY_DOWN ) { godmode = true; }
		if (App->input->GetKey(SDL_SCANCODE_F10) == KEY_DOWN ) { godmode = false; }

		if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_REPEAT) //----------------ATTACK MOVEMENT
		{
			App->audio->PlayFx(sword_sound);

			if (facing == Facing::LEFT) { animation = &attack_left; }
			else if (facing == Facing::RIGHT) { animation = &attack_right; }
		}
		else if (App->input->GetKey(SDL_SCANCODE_RETURN) == KEY_UP )
		{
			if (animation == &attack_right) { attack_right.Reset(); animation = &idle_right; }
			else if (animation == &attack_left) { attack_left.Reset(); animation = &idle_left; }
		}

		if (((App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)) //RUNING RIGHT
		{
			walking = true;
			fposition.x += speed * 1.25f;
			facing = Facing::RIGHT;

			if (camera_movement) { App->render->camera.x -= App->render->camera_speed; }
			if (animation != &jump_right) { animation = &run_right; }
		}
		
		if ((App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT))//-----------WALKING RIGHT
		{
			walking = true;
			fposition.x += speed;
			facing = Facing::RIGHT;

			if (camera_movement) { App->render->camera.x -= App->render->camera_speed; }
			if (animation != &jump_right) {animation = &walk_right; }
		}

		if (((App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) && App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_REPEAT)) 	//BETTER GAMEPLAY
			animation = &walk_right; walking = true;
		
		if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_UP || App->input->GetKey(SDL_SCANCODE_D) == KEY_UP ) 
			animation = &idle_right; walking = false; 
	
		if ((App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) && App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) 	//RUNNING LEFT
		{
			walking = true;
			fposition.x -= speed * 1.25f;
			facing = Facing::LEFT;
			if (camera_movement) { App->render->camera.x -= App->render->camera_speed; }
			if (animation != &jump_right) { animation = &run_right; }
		}
		
		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) //------------WALKING LEFT
		{
			walking = true;
			fposition.x -= speed;
			facing = Facing::LEFT;
			if (animation != &jump_right) { animation = &walk_left; }
		}

		if (App->input->GetKey(SDL_SCANCODE_LEFT) == KEY_UP || App->input->GetKey(SDL_SCANCODE_A) == KEY_UP)
		{
			walking = false;
			if (animation == &walk_left) { animation = &idle_left; }	
			else if (animation == &walk_right) { animation = &idle_right; }
		}			


		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			if (jumping == false && landing == false) { can_jump = true; }

			if (jumping == true && double_jump == true) { can_jump = true; double_jump = false; }

			if (facing == LEFT && can_jump) {animation = &jump_left; }
				
			else if (facing == RIGHT&& can_jump) { animation = &jump_right; }			
			
		}

		if (can_jump)
		{
			App->audio->PlayFx(jump_sound);
			jump_pos = fposition.y;
			jumping = true;
			landing = false;
			can_jump = false;
		}

		if (jumping)
		{
			if ((jump_pos - 50 - fposition.y < jump_limit) && !landing) { fposition.y -= jump_speed; }
			else { landing = true; }
		}
	}
	if (littlejump) {
		fposition.y -= jump_speed;
		littlejumphigh++;
		if (littlejumphigh == LITTLEJUMPHIGH) {
			littlejump = false;
		}
	}

	if (hitted && hit_time - SDL_GetTicks() > 1000) { hitted = false; } // 1 second of invulnerability  if hitted. 
	
	if (collider != nullptr) { collider->SetPos(fposition.x, fposition.y + 5); }

	if (player_hurted && animation->Finished() == true) {
		LoseOneLife(); }

	if (lifes < 1) { Dead(); }
		
	if (points > max_score) { max_score = points; }

	if (fposition.y >= BOTTOM_SCENE_LIMIT && lifes < 1) { App->audio->PlayFx(lose_fx, 0); }

	
	Draw();
	
	return true;
}

void j1Player::Dead()
{
	App->audio->PlayFx(lose_fx, 0);
	lifes = LIFES;
	points = 0;
	App->entities->SetEnemiesInitialPos();
	
	if (App->scene1->active) { 
		App->scene1->CleanUp();
		App->scene1->Start(); }
	else if (App->scene2->active) { App->scene2->SceneChange(); }
}

void j1Player::PlayerHurted() 
{
	player_hurted = true;
	hit_time = SDL_GetTicks();
	use_input = false;
	walking = false;
	App->audio->PlayFx(die_fx);
	
	if (facing == Facing::RIGHT) { animation = &death_right; }
	else if (facing == Facing::LEFT) { animation = &death_left; }
}

void j1Player::LoseOneLife() 
{
		if (App->scene1->active) { position = App->scene1->initial_scene_pos; }
		if (App->scene2->active) { position = App->scene2->initial_scene_pos; }

		SetInitialPos();
		App->entities->SetEnemiesInitialPos();
		App->render->camera.x = 0;
		animation->Reset();
		animation = &idle_right;
		lifes--;
		walking = false;
		hit_time = SDL_GetTicks();
		hitted = true;
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
	int margin =2;
	if (!hitted)
	{
		CollisionDirection direction = c1->CheckDirection(c2->rect);
		if (c2->type == COLLIDER_ENEMIE && direction != ENTITY_ABOVE)
		{
			if (player_hurted == false && godmode == false)
				PlayerHurted();
		}
		else if (c2->type == COLLIDER_GROUND)
		{
			direction = c1->CheckDirection(c2->rect);
			if (direction == NO_COLLISION) { return; }
			if (direction == ENTITY_ABOVE)
			{
				fposition.y = c2->rect.y - PLAYERHEIGHT;
				double_jump = true;
				jumping = false;
				landing = false;				
				if (walking == true) { App->audio->PlayFx(playersteps); }
			}
			if (direction == ENTITY_BELOW)
			{
				littlejump = false;
				fposition.y = c2->rect.y + c2->rect.h;
				jumping = false;
				landing = true;
			}
			if (direction == ENTITY_LEFT)
			{
				fposition.x = c2->rect.x - PLAYERWIDTH - margin;
			}
			if (direction == ENTITY_RIGHT)
			{
				fposition.x = c2->rect.x + c2->rect.w + margin;
			}
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
	idle_right.LoadPlayerAnimations("idle_right");
	idle_left.LoadPlayerAnimations("idle_left");
	jump_right.LoadPlayerAnimations("jump_right");
	jump_left.LoadPlayerAnimations("jump_left");
	walk_right.LoadPlayerAnimations("walk_right");
	walk_left.LoadPlayerAnimations("walk_left");
	run_right.LoadPlayerAnimations("run_right");
}

void j1Player::InitialPlayerPos() {

	if (App->scene1->active){		
		position = App->scene1->initial_scene_pos;
	}
	if (App->scene2->active){	
		position = App->scene2->initial_scene_pos;		
	}
}
void j1Player::LittleJump() {
	littlejump = true;
	littlejumphigh = 0;
}