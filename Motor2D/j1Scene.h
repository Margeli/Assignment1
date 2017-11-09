#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1Render.h"

#define GRAVITY 2
#define BOTTOM_SCENE_LIMIT 750
#define RIGHT_SCENE_LIMIT 3152

struct SDL_Texture;

class j1Scene : public j1Module
{
public:

	j1Scene();

	// Destructor
	virtual ~j1Scene();

	// Called before render is available
	bool Awake(pugi::xml_node&);

	// Called before the first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	// Called each loop iteration
	bool Update(float dt);

	// Called before all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();


	void SceneChange();
	bool fading = false;

	iPoint initial_scene_pos;
	

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;

private:
	void PlaceEnemies() const;

};

#endif // __j1SCENE_H__