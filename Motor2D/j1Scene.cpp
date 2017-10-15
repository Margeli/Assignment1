#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1Player.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene");
}

// Destructor
j1Scene::~j1Scene()
{}

// Called before render is available
bool j1Scene:: Awake(pugi::xml_node&)
{
	LOG("Loading Scene");
	bool ret = true;

	


	return ret;
}

// Called before the first frame
bool j1Scene::Start()
{
	App->map->Load("Map1.tmx");

	return true;
}

// Called each loop iteration
bool j1Scene::PreUpdate()
{
	return true;
}

// Called each loop iteration
bool j1Scene::Update(float dt)
{
	if(App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN)
		App->LoadGame();

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN)
		App->SaveGame();


	
	/*
	if (App->input->GetKey(SDL_SCANCODE_RIGHT) == KEY_REPEAT)
		App->render->camera.x += 1;*/

	App->player->position.y += GRAVITY;

	//-----CAMERA MOVEMENT----
	if (App->player->position.x > -App->render->camera.x + (3 * SCREEN_WIDTH / 5))
		App->player->camera_movement = true;
	
	else {
		App->player->camera_movement = false;
	}
	//-------------------------



	if (App->player->position.y >= 545)	//630
	{	
		App->player->position.y = 545;	
	}

	if (App->player->position.x <= 0 )
	{
		App->player->position.x = 0;
	}

	if (App->player->position.y <= 0)
	{
		App->player->position.y = 0;
	}

	if (App->player->position.x >= 3152 && App->player->position.y > 160)
	{
		App->player->position.x = 3152;	
	}
	else if (App->player->position.x >= 3152 && App->player->position.y < 160)
	{
		//FADE TO BLACK LEVEL 2
		LOG("ENDLVL1!");
	}


	//App->render->Blit(img, 0, 0);
	App->map->Draw();


	p2SString title("Map:%dx%d Tiles:%dx%d Tilesets:%d",
					App->map->data.width, App->map->data.height,
					App->map->data.tile_width, App->map->data.tile_height,
					App->map->data.tilesets.count());

	App->win->SetTitle(title.GetString());

	if (App->input->GetKey(SDL_SCANCODE_F3) == KEY_DOWN)
		SceneChange();

	return true;
}

// Called each loop iteration
bool j1Scene::PostUpdate()
{
	bool ret = true;

	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

// Called before quitting
bool j1Scene::CleanUp()
{
	LOG("Freeing scene");

	App->map->CleanUp();

	App->collis->CleanUp();

	

	return true;
}

bool j1Scene::Load(pugi::xml_node& data)
{
	pugi::xml_node activated = data.child("activated");
	
	bool scene_active = activated.attribute("true").as_bool();
	if ((scene_active == false)&& active) {

		SceneChange();
		//NEED TO PUT FADING
	
	}
	

	return true;
}
bool j1Scene::Save(pugi::xml_node& data) const
{
	pugi::xml_node activated = data.append_child("activated");

	activated.append_attribute("true") = active;

	return true;
}


void j1Scene::SceneChange() {
	
	App->scene2->active = true;
	App->scene->active = false;
	CleanUp();
	

	App->player->CleanUp();
	App->player->Start();
	App->render->camera = { 0,0 };
	App->scene2->Start();
	}
//
//
//bool j1Player::Load(pugi::xml_node& data)
//{
//	position.x = data.child("position").attribute("x").as_int();
//	position.y = data.child("position").attribute("y").as_int();
//
//	return true;
//
//}
//bool j1Player::Save(pugi::xml_node& data) const
//{
//	pugi::xml_node pos = data.append_child("position");
//
//	pos.append_attribute("x") = position.x;
//	pos.append_attribute("y") = position.y;
//
//
//	return true;
//}