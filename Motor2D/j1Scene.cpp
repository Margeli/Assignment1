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
#include "j1EntityManager.h"
#include "Brofiler/Brofiler.h"

j1Scene::j1Scene() : j1Module()
{
	name.create("scene1");
}

j1Scene::~j1Scene()
{}

bool j1Scene:: Awake(pugi::xml_node&)
{
	LOG("Loading Scene 1");
	bool ret = true;
	if (App->scene1->active == false) { LOG("Unable to load Scene 1."); ret = false; }

	return ret;
}

bool j1Scene::Start()
{
	if (active) 
	{
		App->map->Load("Map1.tmx");
		initial_scene_pos = App->map->data.layers.At(2)->data->initial_player_position; //Gets the player position from the last layer loaded from Tiled
		// Should have the initial pos of enemies in a XML
		App->entities->player->position = initial_scene_pos;		
		App->audio->PlayMusic("audio/music/music_sadpiano.ogg");
		PlaceEnemies();
	}
	return true;
}

bool j1Scene::PreUpdate()
{
	return true;
}

bool j1Scene::Update(float dt)
{
	BROFILER_CATEGORY("Scene1_Update", Profiler::Color::Chocolate);
	App->entities->player->position.y += GRAVITY;

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) { App->LoadGame(); }

	if(App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) { App->SaveGame(); }
		
	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN || App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		App->entities->player->position = initial_scene_pos;		App->render->camera.x = 0;
	}

	if (App->entities->player->position.x <= -App->render->camera.x) { App->entities->player->position.x++; }

	//CAMERA MOVEMENT
	if ((App->entities->player->position.x > -App->render->camera.x + (3 * SCREEN_WIDTH / 5)) && (App->render->camera.x > CAMERA_LIMIT))
		App->entities->player->camera_movement = true;

	else { App->entities->player->camera_movement = false; }

	if (App->entities->player->position.y >= BOTTOM_SCENE_LIMIT) { App->entities->player->PlayerHurted();}
	
	if (App->entities->player->position.x >= RIGHT_SCENE_LIMIT)
	{ 
		LOG("End of level 1!");
		SceneChange();
	}

	App->map->Draw();

	return true;
}

bool j1Scene::PostUpdate()
{
	bool ret = true;
	if(App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN) { ret = false; }

	return ret;
}

bool j1Scene::CleanUp()
{
	LOG("Unloading scene 1.");

	App->map->CleanUp();
	App->collis->CleanUp();
	App->tex->CleanUp();
	App->entities->CleanUp();

	return true;
}

bool j1Scene::Load(pugi::xml_node& data)
{
	pugi::xml_node activated = data.child("activated");

	bool scene_active = activated.attribute("true").as_bool();

	if ((scene_active == false) && active) { SceneChange(); }			

	return true;
}

bool j1Scene::Save(pugi::xml_node& data) const
{
	pugi::xml_node activated = data.append_child("activated");

	activated.append_attribute("true") = active;

	return true;
}

void j1Scene::SceneChange() 
{
	App->scene2->active = true;
	App->scene1->active = false;

	CleanUp();	
	
	App->entities->Start();
	App->collis->Start();
	App->render->camera = { 0,0 };
	App->scene2->Start();
}

void j1Scene::PlaceEnemies() const{

	App->entities->CreateEntity(TROLL, { 250, 514 });
	App->entities->CreateEntity(FLY, { 400, 100 });

	//App->enemies->AddEnemy(TROLL, 800, 420);
	
}

