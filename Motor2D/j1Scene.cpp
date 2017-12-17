#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Map.h"
#include "j1SceneMenu.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1Player.h"
#include "j1EntityManager.h"
#include "j1Entity.h"
#include "Brofiler/Brofiler.h"
#include "j1Pathfinding.h"
#include "j1FadeToBlack.h"
#include "j1PlayerGui.h"

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
	if (App->menu->active == true) { active = false; }
	if (active == false) { LOG("Scene1 not active ."); }

	return ret;
}

bool j1Scene::Start()
{
	if (App->menu->active == true) { active = false; }
	if (App->scene2->active == true) { active = false; }
	
	if (active) 
	{
		if (App->map->Load("Map1.tmx")) 
		{
			int w, h;
			uchar* data = NULL;			
			if (App->map->CreateWalkabilityMap(w, h, &data, EntityTypes::FLY))
			{
				App->pathfind->SetMap(w, h, data, EntityTypes::FLY);
				RELEASE_ARRAY(data);
			}
			
			if (App->map->CreateWalkabilityMap(w, h, &data, EntityTypes::TROLL)) 
			{
				App->pathfind->SetMap(w, h, data, EntityTypes::TROLL);
				RELEASE_ARRAY(data);
			}
		}
		initial_scene_pos = { App->map->data.layers.At(2)->data->properties.Get("xpos"),
		App->map->data.layers.At(2)->data->properties.Get("ypos")}; //Gets the player position from the last layer loaded from Tiled
		
		mainsong = App->audio->PlayMusic("audio/music/main_song.ogg");

		PlaceEnemies();
		time.Start();
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
		App->entities->player->fposition = { (float)initial_scene_pos.x, (float)initial_scene_pos.y };
		App->render->camera.x = 0;
	}
	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN ) { App->cap_on = !App->cap_on; }
	if (App->entities->player->position.x <= -App->render->camera.x && App->entities->player) { App->entities->player->fposition.x++; }		

	//CAMERA MOVEMENT
	if ((App->entities->player->position.x > -App->render->camera.x + (3 * SCREEN_WIDTH / 5)) && (App->render->camera.x > CAMERA_LIMIT))
		App->entities->player->camera_movement = true;

	else { App->entities->player->camera_movement = false; }

	if (App->entities->player->fposition.y >= BOTTOM_SCENE_LIMIT && App->entities->player->player_hurted == false) {
		App->entities->player->LoseOneLife(); }
	
	if (App->entities->player->position.x >= RIGHT_SCENE_LIMIT && App->scene2->active != true)
	{ 
		LOG("End of level 1!");
		App->fade->FadeToBlack(App->scene1, App->scene2, 0.8f); 
		SceneChange(); 
	}
	App->map->Draw();

	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) {  App->fade->FadeToBlack(this, App->menu, 0.8f); SceneChangeMenu(); }

	return true;
}

bool j1Scene::PostUpdate()
{
	bool ret = true;
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		if (paused) 
		{
			App->ResumeGame();
			App->entities->player->playerGui->DestroyESCWindow();
			//while (ShowCursor(false) >= 0);
		}
		else 
		{
			App->entities->player->playerGui->CreateESCWindow();
			App->PauseGame();
		}
	}
	return ret;
}

bool j1Scene::CleanUp()
{
	LOG("Unloading scene 1.");

	App->map->CleanUp();
	App->collis->CleanUp();
	App->tex->CleanUp();
	App->entities->EnemiesCleanUp();
	if (App->entities->player)
	App->entities->player->CleanUp();
	App->pathfind->CleanUp();
	finish_time = time.ReadSec()+saved_time;
	time.SetZero();
	saved_time = 0;
	
	return true;
}

bool j1Scene::Load(pugi::xml_node& data)
{
	pugi::xml_node activated = data.child("activated");
	bool scene_active = activated.attribute("true").as_bool();
	if (active) {
		pugi::xml_node time = data.child("time");
		saved_time = time.attribute("value").as_uint();

		if (scene_active == false)  { SceneChange(); }
	}
	return true;
}

bool j1Scene::Save(pugi::xml_node& data) const
{
	pugi::xml_node activated = data.append_child("activated");
	activated.append_attribute("true") = active;

	uint currenttime = 0;
	if (active) currenttime = time.ReadSec()+saved_time;
	pugi::xml_node _time = data.append_child("time");
	_time.append_attribute("value") =currenttime ;

	return true;
}

void j1Scene::SceneChange() 
{
	App->scene2->active = true;
	App->scene1->active = false;

	App->scene2->time.AddTime(time);

	CleanUp();	
	App->entities->Start();
	App->scene2->Start();
	//App->entities->first_loop = true;
	
	App->collis->Start();
	App->render->SetCameraInitialPos();
	App->pathfind->Start();
	App->entities->player->fposition = { (float)initial_scene_pos.x, (float)initial_scene_pos.y };
}

void j1Scene::SceneChangeMenu()
{
	App->scene1->active = false;
	App->menu->active = true;

	CleanUp();
	App->render->SetCameraInitialPos();
	App->entities->CleanUp();
	App->entities->active = false;
	App->menu->Start();
	if (paused)
		App->ResumeGame();
}

void j1Scene::PlaceEnemies() const
{
	for (p2List_item<ObjectGroup*>* object = App->map->data.objects.start; object; object = object->next) 
	{App->entities->CreateEntity(object->data->type, { object->data->x, object->data->y });}
}

