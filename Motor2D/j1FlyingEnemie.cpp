#include "j1FlyingEnemie.h"
#include "p2Defs.h"
#include "j1Render.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Textures.h"
#include "j1Animation.h"
#include "j1Audio.h"

j1FlyingEnemie::j1FlyingEnemie()
{
	name.create("Fly");
	graphics = nullptr;

	fly_right.PushBack({ 0, 0, 0, 0 });
	fly_right.loop = true;
	fly_right.speed = 0.07f;

	fly_left.PushBack({ 0, 0, 0, 0 });
	fly_left.loop = true;
	fly_left.speed = 0.07f;
}

j1FlyingEnemie::~j1FlyingEnemie()
{
}

bool j1FlyingEnemie::Awake(pugi::xml_node& conf)
{
	return true;
}

bool j1FlyingEnemie::Start()
{
	bool ret = true;

	graphics = App->tex->Load("textures/Fly.png");
	enemie_collider = App->collis->AddCollider({ position.x, position.y, 0, 0 }, COLLIDER_ENEMIE, this);

	if (!graphics) { LOG("Error loading Fly sprites.");  ret = false; }

	current_animation = &fly_left;

	if (fly_sound == 0)
		fly_sound = App->audio->LoadFx("audio/fx/troll_death.wav");


	return ret;
}

bool j1FlyingEnemie::CleanUp()
{
	LOG("Unloading Troll 1.");

	App->tex->UnLoad(graphics);

	if (enemie_collider != nullptr)
	{
		enemie_collider->to_delete = true;
	}

	return true;
}

bool j1FlyingEnemie::Update(float dt)
{
	//IA
	//Pathfinding applied to platformers.

	return true;
}

void j1FlyingEnemie::OnCollision(Collider* c1, Collider* c2)
{
	int margin = 0;
	
}



void j1FlyingEnemie::initialpos()
{
	position = { 100, 100 };
}