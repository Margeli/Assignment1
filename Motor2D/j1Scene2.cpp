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
#include "j1Enemies.h"

j1Scene2::j1Scene2() : j1Module()
{
	name.create("scene2");
}

j1Scene2::~j1Scene2()
{}

bool j1Scene2::Awake(pugi::xml_node&)
{
	LOG("Loading Scene2");
	bool ret = true;

	if (App->scene->active == true) { active = false; }

	return ret;
}

bool j1Scene2::Start()
{
	if (active) 
	{
		App->map->Load("Map2.tmx");
		initial_scene_pos = App->map->data.layers.At(2)->data->initial_player_position; //Gets the player position from the last layer loaded from Tiled
		// Should have the initial pos of enemies in a XML
		App->player->position= initial_scene_pos; 	
		App->audio->PlayMusic("audio/music/music_sadpiano.ogg");
	}

	return true;
}

bool j1Scene2::PreUpdate()
{
	return true;
}

bool j1Scene2::Update(float dt)
{
	App->player->position.y += GRAVITY;

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) { App->LoadGame(); }

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) { App->SaveGame(); }

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN)
	{
		SceneChange();
		App->player->position.x = 50;
		App->player->position.y = 100;
		App->render->camera.x = 0;
	}

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		App->player->position.x = 0;
		App->player->position.y = 100;
		App->render->camera.x = 0;
	}
	if (App->player->position.x <= -App->render->camera.x) { App->player->position.x++; }
	if (App->player->position.x > -App->render->camera.x + (3 * SCREEN_WIDTH / 5) && (App->render->camera.x> CAMERA_LIMIT)) { App->player->camera_movement = true; }

	else { App->player->camera_movement = false; }

	if (App->player->position.y >= 750)
	{
		App->player->LoseOneLife(initial_scene_pos);
		
	}

	if (App->player->position.x <= 35) { App->player->position.x = 35; }

	if (App->player->position.y <= 0) { App->player->position.y = 0; }	

	App->map->Draw();

	p2SString title("Level 2 | Lives: %d  Points: %d  Max Score: %d  | Map:%dx%d Tiles:%dx%d Tilesets:%d",
		App->player->lifes, App->player->points, App->player->max_score,
		App->map->data.width, App->map->data.height,
		App->map->data.tile_width, App->map->data.tile_height,
		App->map->data.tilesets.count());

	App->win->SetTitle(title.GetString());

	
	return true;
}

bool j1Scene2::PostUpdate()
{
	bool ret = true;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
		ret = false;

	return ret;
}

bool j1Scene2::CleanUp()
{
	LOG("Freeing scene2");

	App->map->CleanUp();
	App->collis->CleanUp();
	App->tex->CleanUp();
	App->player->CleanUp();

	return true;
}

bool j1Scene2::Load(pugi::xml_node& data)
{
	pugi::xml_node activated = data.child("activated");

	bool scene2_active = activated.attribute("true").as_bool();

	if (scene2_active == false && active) { SceneChange(); }															

	return true;
}

bool j1Scene2::Save(pugi::xml_node& data) const
{
	pugi::xml_node activated = data.append_child("activated");

	activated.append_attribute("true") = active;

	return true;
}

void j1Scene2::SceneChange() 
{
	App->scene->active = true;
	App->scene2->active = false;

	CleanUp();
	
	App->player->Start();
	
	App->render->camera = { 0,0 };
	App->scene->Start();
}

void j1Scene2::PlaceEnemies() const
{
	App->enemies->AddEnemy(TROLL, 100, 100);
}