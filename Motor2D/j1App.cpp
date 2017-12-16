#include <iostream> 
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Player.h"
#include "j1Window.h"
#include "j1Input.h"
#include "j1Render.h"
#include "j1Textures.h"
#include "j1Audio.h"
#include "j1Scene.h"
#include "j1Scene2.h"
#include "j1SceneMenu.h"
#include "j1Map.h"
#include "j1App.h"
#include "j1EntityManager.h"
#include "j1FadeToBlack.h"
#include "j1Pathfinding.h"
#include "Brofiler/Brofiler.h"

#define ONE_TIME_UNIT 1000



j1App::j1App(int argc, char* args[]) : argc(argc), args(args)
{
	frames = 0;
	want_to_save = want_to_load = false;

	input = new j1Input();
	win = new j1Window();	
	tex = new j1Textures();
	audio = new j1Audio();
	scene1 = new j1Scene();
	scene2 = new j1Scene2();
	menu = new j1SceneMenu();
	map = new j1Map();
	render = new j1Render();
	collis = new j1Collisions();
	entities = new j1EntityManager();
	pathfind = new j1Pathfinding();
	font = new j1Fonts();
	gui = new j1Gui();
	fade = new j1FadeToBlack();

	AddModule(input);
	AddModule(win);
	AddModule(tex);
	AddModule(audio);
	//---- Scenes
	AddModule(map);
	AddModule(scene1);
	AddModule(scene2);
	AddModule(menu);
	AddModule(pathfind);
	AddModule(font);
	//----Entities
		
	AddModule(entities);

	//GUI
	AddModule(gui);
	AddModule(fade);

	
	//------collider & render (should be the lasts to update)
	AddModule(collis);
	AddModule(render);
	

	load_game = "save_game.xml";
	save_game = "save_game.xml";

	PERF_PEEK(perf_timer);
}

j1App::~j1App()
{
	// release modules
	p2List_item<j1Module*>* item = modules.end;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->prev;
	}

	modules.clear();
}

void j1App::AddModule(j1Module* module)
{
	module->Init();
	modules.add(module);
}

// Called before render is available
bool j1App::Awake()
{
	pugi::xml_document	config_file;
	pugi::xml_node		config;
	pugi::xml_node		app_config;

	bool ret = false;
	
	pugi::xml_parse_result result = config_file.load_file(load_game.GetString());
	if (result)
	{
		LOG("save_game.xml loaded succesfully");
		savefilefound = true;
	}

	config = LoadConfig(config_file);

	if(config.empty() == false)
	{
		// self-config
		ret = true;
		app_config = config.child("app");
		title.create(app_config.child("title").child_value());
		organization.create(app_config.child("organization").child_value());
		frame_rate_cap_count = app_config.attribute("cap").as_uint();
	}

	if(ret == true)
	{
		p2List_item<j1Module*>* item;
		item = modules.start;

		while(item != NULL && ret == true)
		{
			ret = item->data->Awake(config.child(item->data->name.GetString()));
			item = item->next;
		}
	}

	if (App->render->vsync == true) { state = "ON"; }
	else { state = "OFF"; }

	return ret;
}

// Called before the first frame
bool j1App::Start()
{
	BROFILER_CATEGORY("App_Start", Profiler::Color::DarkCyan);

	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Start();
		item = item->next;
	}

	return ret;
}

// Called each loop iteration
bool j1App::Update()
{
	BROFILER_CATEGORY("App_GlobalUpdate", Profiler::Color::BlueViolet);

	bool ret = true;
	PrepareUpdate();

	if(input->GetWindowEvent(WE_QUIT) == true)
		ret = false;

	if(ret == true)
		ret = PreUpdate();

	if(ret == true)
		ret = DoUpdate();

	if(ret == true)
		ret = PostUpdate();

	FinishUpdate();
	return ret;
}

pugi::xml_node j1App::LoadConfig(pugi::xml_document& config_file) const
{
	pugi::xml_node ret;

	pugi::xml_parse_result result = config_file.load_file("config.xml");

	if(result == NULL)
		LOG("Could not load map xml file config.xml. pugi error: %s", result.description());
	else
		ret = config_file.child("config");

	return ret;
}

void j1App::PrepareUpdate()
{

	BROFILER_CATEGORY("App_PrepareUpdate", Profiler::Color::DarkCyan);
	frame_counter++;
	last_sec_frame_count++;

	dt = one_frame_time.ReadSec();
	if (dt > (float)frame_rate_cap_count / ONE_TIME_UNIT)
	{
		dt = (float)frame_rate_cap_count / ONE_TIME_UNIT;
	}
	one_frame_time.Start();
}

void j1App::FinishUpdate()
{
	BROFILER_CATEGORY("App_FinishUpdate", Profiler::Color::DarkCyan);
	if(want_to_save == true)
		
		SavegameNow();

	if(want_to_load == true)
		LoadGameNow();

	if (last_sec_frame_time.Read() > ONE_TIME_UNIT)
	{
		last_sec_frame_time.Start();
		prev_last_sec_frame_count = last_sec_frame_count;
		last_sec_frame_count = 0;
	}
	PERF_START(perf_timer);

	last_frame_ms = one_frame_time.Read();
	frames_on_last_update = prev_last_sec_frame_count;

	float FPS = float(frame_counter) / timer.ReadSec();
	float seconds_since_start = timer.ReadSec();
	float delay_time = (ONE_TIME_UNIT / frame_rate_cap_count) - last_frame_ms;

	if (delay_time > (ONE_TIME_UNIT / frame_rate_cap_count))
		delay_time = (ONE_TIME_UNIT / frame_rate_cap_count);


	if (cap_on) {
		SDL_Delay(delay_time);
		cap = "ON";
	}
	else { cap = "OFF"; }
	if (render->vsync == true) { state = "ON"; }	else { state = "OFF"; }
	if (entities->player && delay_time) { static char title[50]; sprintf_s(title, 50, "CAVE KNIGHT"); App->win->SetTitle(title); }
}

// Call modules before each loop iteration
bool j1App::PreUpdate()
{
	BROFILER_CATEGORY("App_PreUpdate", Profiler::Color::Aqua);
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PreUpdate();
	}

	return ret;
}

// Call modules on each loop iteration
bool j1App::DoUpdate()
{
	BROFILER_CATEGORY("App_Update", Profiler::Color::Azure);
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.start;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->Update(dt);
	}

	

	return ret;
}

// Call modules after each loop iteration
bool j1App::PostUpdate()
{
	BROFILER_CATEGORY("App_PostUpdate", Profiler::Color::Brown);
	bool ret = true;
	p2List_item<j1Module*>* item;
	j1Module* pModule = NULL;

	for(item = modules.start; item != NULL && ret == true; item = item->next)
	{
		pModule = item->data;

		if(pModule->active == false) {
			continue;
		}

		ret = item->data->PostUpdate();
	}

	return ret;
}

// Called before quitting
bool j1App::CleanUp()
{
	BROFILER_CATEGORY("App_CleanUp", Profiler::Color::Cyan);
	bool ret = true;
	p2List_item<j1Module*>* item;
	item = modules.end;

	while(item != NULL && ret == true)
	{
		ret = item->data->CleanUp();
		item = item->prev;
	}

	return ret;
}

// ---------------------------------------
int j1App::GetArgc() const
{
	return argc;
}

// ---------------------------------------
const char* j1App::GetArgv(int index) const
{
	if(index < argc)
		return args[index];
	else
		return NULL;
}

// ---------------------------------------
const char* j1App::GetTitle() const
{
	return title.GetString();
}

// ---------------------------------------
const char* j1App::GetOrganization() const
{
	return organization.GetString();
}

// Load / Save
void j1App::LoadGame()
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list
	want_to_load = true;
}

// ---------------------------------------
void j1App::SaveGame() const
{
	// we should be checking if that file actually exist
	// from the "GetSaveGames" list ... should we overwrite ?

	want_to_save = true;
}

// ---------------------------------------
void j1App::GetSaveGames(p2List<p2SString>& list_to_fill) const
{
	// need to add functionality to file_system module for this to work
}

bool j1App::LoadGameNow()
{
	BROFILER_CATEGORY("LoadingGame", Profiler::Color::DarkCyan);

	bool ret = false;

	pugi::xml_document data;
	pugi::xml_node root;

	pugi::xml_parse_result result = data.load_file(load_game.GetString());

	if(result != NULL)
	{
		LOG("Loading new Game State from %s...", load_game.GetString());

		root = data.child("game_state");

		p2List_item<j1Module*>* item = modules.start;
		ret = true;

		while(item != NULL && ret == true)
		{
			ret = item->data->Load(root.child(item->data->name.GetString()));
			item = item->next;
		}

		data.reset();
		if(ret == true)
			LOG("...finished loading");
		else
			LOG("...loading process interrupted with error on module %s", (item != NULL) ? item->data->name.GetString() : "unknown");
	}
	else
		LOG("Could not parse game state xml file %s. pugi error: %s", load_game.GetString(), result.description());

	want_to_load = false;
	return ret;
}

bool j1App::SavegameNow() const
{
	BROFILER_CATEGORY("SavingGame", Profiler::Color::DarkCyan);

	bool ret = true;

	LOG("Saving Game State to %s...", save_game.GetString());

	// xml object were we will store all data
	pugi::xml_document data;
	pugi::xml_node root;
	
	root = data.append_child("game_state");

	p2List_item<j1Module*>* item = modules.start;

	while(item != NULL && ret == true)
	{
		ret = item->data->Save(root.append_child(item->data->name.GetString()));
		item = item->next;
	}

	if(ret == true)
	{
		data.save_file(save_game.GetString());
		LOG("... finished saving", );
	}
	else
		LOG("Save process halted from an error in module %s", (item != NULL) ? item->data->name.GetString() : "unknown");

	data.reset();
	want_to_save = false;
	return ret;
}

void j1App::PauseGame()
{
	pathfind->active = false;	
	fade->active = false;
	collis->active = false;
	//render->active = false;
	scene1->paused = true;
	scene2->paused = true;
	entities->paused = true;
}

void j1App::ResumeGame()
{
	pathfind->active = true;
	fade->active = true;
	collis->active = true;

	entities->paused = false;	
	scene1->paused = false;
	scene2->paused = false;	
}