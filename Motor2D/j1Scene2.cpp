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

#define RIGHT_SCENE_LIMIT_2 3168
#define WIN_IMAGE_MARGIN 995

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
		winning = App->tex->Load("textures/winning.png");
		winning_rect = { 0, 0, 1024, 770 };

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

		App->audio->PlayMusic("audio/music/main_song.ogg"); 
		PlaceEnemies();
		saved_time += App->scene1->finish_time;
		time.Start();
		
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
	if (win == false) { App->entities->player->position.y += GRAVITY; }

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

	if (App->entities->player->position.x >= RIGHT_SCENE_LIMIT_2 && !App->scene1->active)
	{
		LOG("End of level 2!");

		win = true;

		App->entities->player->animation = &App->entities->player->winning_anim;
		App->render->Blit(winning, RIGHT_SCENE_LIMIT_2 , 0, &winning_rect);	
		App->render->camera.x = -RIGHT_SCENE_LIMIT_2;
		App->entities->player->fposition = { RIGHT_SCENE_LIMIT_2 + 600, 255 };
		App->entities->EnemiesCleanUp();

		//Hide UI!

		if (App->input->GetKey(SDL_SCANCODE_SPACE) == KEY_DOWN)
		{
			App->fade->FadeToBlack(App->scene2, App->menu, 0.8f);
			SceneChangeMenu();
			win = false;

			//delete save_game.xml in here!
		}
	}

	return true;
}

bool j1Scene2::PostUpdate()
{
	bool ret = true;
	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN)
	{
		if (paused)
		{
			App->ResumeGame();
			App->entities->player->playerGui->DestroyESCWindow();
		}
		else
		{
			App->entities->player->playerGui->CreateESCWindow();
			App->PauseGame();
		}
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
	time.SetZero();
	saved_time = 0;

	return true;
}

bool j1Scene2::Load(pugi::xml_node& data)
{
	pugi::xml_node activated = data.child("activated");
	bool scene2_active = activated.attribute("true").as_bool();	

	if (active) {
		pugi::xml_node time = data.child("time");
		saved_time = time.attribute("value").as_uint();

		if (scene2_active == false) { SceneChange(); }
	}
	return true;
}

bool j1Scene2::Save(pugi::xml_node& data) const
{
	pugi::xml_node activated = data.append_child("activated");
	activated.append_attribute("true") = active;

	uint currenttime = 0;
	if (active) currenttime = time.ReadSec()+saved_time;
	pugi::xml_node _time = data.append_child("time");
	_time.append_attribute("value") = currenttime;
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

	if (paused)
		App->ResumeGame();
}

void j1Scene2::PlaceEnemies() const
{
	for (p2List_item<ObjectGroup*>* object = App->map->data.objects.start; object; object = object->next) {
		App->entities->CreateEntity(object->data->type, { object->data->x, object->data->y });
	}
}