#ifndef __j1APP_H__
#define __j1APP_H__

#include "p2List.h"
#include "j1Module.h"
#include "j1Timer.h"
#include "j1PerfTimer.h"
#include "PugiXml\src\pugixml.hpp"

// Modules
class j1Window;
class j1Input;
class j1Render;
class j1Textures;
class j1Audio;
class j1Scene;
class j1Scene2;
class j1Map;
class j1Collisions;
class j1EntityManager;
class j1Pathfinding;
class j1Timer;
class j1PerfTimer;

class j1App
{
public:

	// Constructor
	j1App(int argc, char* args[]);

	// Destructor
	virtual ~j1App();

	// Called before render is available
	bool Awake();

	// Called before the first frame
	bool Start();

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	// Add a new module to handle
	void AddModule(j1Module* module);

	// Exposing some properties for reading
	int GetArgc() const;
	const char* GetArgv(int index) const;
	const char* GetTitle() const;
	const char* GetOrganization() const;

	void LoadGame();
	void SaveGame() const;
	void GetSaveGames(p2List<p2SString>& list_to_fill) const;

private:

	// Load config file
	pugi::xml_node LoadConfig(pugi::xml_document&) const;

	// Call modules before each loop iteration
	void PrepareUpdate();

	// Call modules before each loop iteration
	void FinishUpdate();

	// Call modules before each loop iteration
	bool PreUpdate();

	// Call modules on each loop iteration
	bool DoUpdate();

	// Call modules after each loop iteration
	bool PostUpdate();

	// Load / Save
	bool LoadGameNow();
	bool SavegameNow() const;

public:

	// Modules
	j1Window*			win = nullptr;
	j1Input*			input = nullptr;
	j1Render*			render = nullptr;
	j1Textures*			tex = nullptr;
	j1Audio*			audio = nullptr;
	j1Scene*			scene1 = nullptr;
	j1Scene2*			scene2 = nullptr;
	j1Map*				map = nullptr;	
	j1Collisions*		collis = nullptr;
	j1EntityManager*	entities = nullptr;
	j1Pathfinding*		pathfind = nullptr;

	j1Timer				timer;
	j1PerfTimer			perf_timer;

private:

	p2List<j1Module*>	modules;
	uint				frames;
	int					argc;
	char**				args;

	p2SString			title;
	p2SString			organization;
	mutable p2SString				level;
	mutable p2SString 				state;
	mutable p2SString				cap;

	mutable bool		want_to_save;
	bool				want_to_load;
	p2SString			load_game;
	mutable p2SString	save_game;

	uint64				frame_counter = 0;
	uint32				last_frame_ms;

	float seconds_since_start;
	float actual_game_time;
	float FPS;
	float dt;

	j1Timer				last_frame_time;
	j1Timer				last_sec_frame_time;

	uint16_t			framerate_cap;
	int					capped_ms = -1;
};

extern j1App* App; 
#endif