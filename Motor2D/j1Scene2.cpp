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
#include "j1PathFinding.h"
#include "Brofiler/Brofiler.h"
#include "j1FadeToBlack.h"
#include "j1PlayerGui.h"

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

	if (active == false) { LOG("Scene 2 not active"); }
	if (App->menu->active == true) { active = false; }
	if (App->scene1->active == true) { active = false; }

	return ret;
}

bool j1Scene2::Start()
{
	if (active) {
		if (paused) { App->map->Draw(); return true; }
		if (App->map->Load("Map2.tmx")) {
			int w, h;
			uchar* data = NULL;
			if (App->map->CreateWalkabilityMap(w, h, &data, EntityTypes::FLY)) {//creates walkabilty map for flying enemies
				App->pathfind->SetMap(w, h, data, EntityTypes::FLY);
				RELEASE_ARRAY(data);
			}
			if (App->map->CreateWalkabilityMap(w, h, &data, EntityTypes::TROLL)) {//creates walkability map for ground enemies
				App->pathfind->SetMap(w, h, data, EntityTypes::TROLL);
				RELEASE_ARRAY(data);
			}
		}
		initial_scene_pos = { App->map->data.layers.At(2)->data->properties.Get("xpos"),
			App->map->data.layers.At(2)->data->properties.Get("ypos") }; //Gets the player position from the last layer loaded from Tiled
		// Should have the initial pos of enemies in a XML

		App->audio->PlayMusic("audio/music/music_sadpiano.ogg");
		PlaceEnemies();
	}
	return true;
}

bool j1Scene2::PreUpdate()
{
	return true;
}

bool j1Scene2::Update(float dt)
{
	BROFILER_CATEGORY("Scene2_Update", Profiler::Color::Azure);
	App->entities->player->position.y += GRAVITY;

	if (App->input->GetKey(SDL_SCANCODE_F6) == KEY_DOWN) { App->LoadGame(); }

	if (App->input->GetKey(SDL_SCANCODE_F5) == KEY_DOWN) { App->SaveGame(); }

	if (App->input->GetKey(SDL_SCANCODE_F1) == KEY_DOWN) { App->fade->FadeToBlack(App->scene2, App->scene1, 0.9f); SceneChange(); }

	if (App->input->GetKey(SDL_SCANCODE_F2) == KEY_DOWN)
	{
		App->entities->player->position = initial_scene_pos; 	App->render->camera.x = 0;
	}
	if (App->input->GetKey(SDL_SCANCODE_F11) == KEY_DOWN) { App->cap_on = !App->cap_on; }

	if (App->entities->player->position.x <= -App->render->camera.x) { App->entities->player->fposition.x++; }

	//CAMERA MOVEMENT
	if (App->entities->player->position.x > -App->render->camera.x + (3 * SCREEN_WIDTH / 5) && (App->render->camera.x> CAMERA_LIMIT)) {
		App->entities->player->camera_movement = true; }

	else { App->entities->player->camera_movement = false; }

	if (App->entities->player->position.y >= BOTTOM_SCENE_LIMIT && App->entities->player->player_hurted == false) { App->entities->player->PlayerHurted(); }
	if ((App->entities->player->position.x < HIGH_LIMIT || App->entities->player->position.y < HIGH_LIMIT)&& App->entities->player->player_hurted == false) {
		App->entities->player->fposition = { (float)initial_scene_pos.x,(float) initial_scene_pos.y };
	}
	App->map->Draw();

	if (App->input->GetKey(SDL_SCANCODE_M) == KEY_DOWN) { App->fade->FadeToBlack(this, App->menu, 0.8f); SceneChangeMenu(); }


	return true;
}

bool j1Scene2::PostUpdate()
{
	bool ret = true;
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		App->PauseGame();
		App->entities->player->playerGui->CreateESCWindow();
	}
	return ret;
}

bool j1Scene2::CleanUp()
{
	LOG("Freeing scene2");

	App->map->CleanUp();
	App->collis->CleanUp();
	App->tex->CleanUp();
	App->entities->EnemiesCleanUp();
	if (App->entities->player)
		App->entities->player->CleanUp();
	App->pathfind->CleanUp();
	
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
	App->scene1->active = true;
	App->scene2->active = false;

	CleanUp();
	
	App->entities->Start();
	App->scene1->Start();	
	
	App->collis->Start();
	App->render->SetCameraInitialPos();
	App->pathfind->Start();
	App->entities->player->fposition = { (float)initial_scene_pos.x, (float)initial_scene_pos.y };
}
void j1Scene2::SceneChangeMenu()
{
	App->menu->active = true;
	App->scene2->active = false;

	CleanUp();
	App->render->SetCameraInitialPos();
	App->entities->CleanUp();
	App->entities->active = false;
	App->menu->Start();
}

void j1Scene2::PlaceEnemies() const
{
	for (p2List_item<ObjectGroup*>* object = App->map->data.objects.start; object; object = object->next) {
		App->entities->CreateEntity(object->data->type, { object->data->x, object->data->y });
	}
}