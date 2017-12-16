#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1Render.h"
#include "j1Timer.h"


#define BOTTOM_SCENE_LIMIT 800
#define HIGH_LIMIT -15
#define RIGHT_SCENE_LIMIT 3152

struct SDL_Texture;




class j1Scene : public j1Module
{
public:

	j1Scene();

	virtual ~j1Scene();

	bool Awake(pugi::xml_node&);

	bool Start();

	bool PreUpdate();

	bool Update(float dt);

	bool PostUpdate();

	bool CleanUp();

	void SceneChange();

	void SceneChangeMenu();

	bool paused = false;

	iPoint initial_scene_pos;

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

	uint mainsong;

	j1Timer time;
	uint saved_time = 0;
	uint finish_time=0;
	

private:
	void PlaceEnemies() const;
	

};

#endif // __j1SCENE_H__