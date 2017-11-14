#ifndef __j1SCENE_H__
#define __j1SCENE_H__

#include "j1Module.h"
#include "j1Render.h"

#define GRAVITY 2
#define BOTTOM_SCENE_LIMIT 750
#define RIGHT_SCENE_LIMIT 3152

struct SDL_Texture;
class Pathfinding;

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

	bool fading = false;

	iPoint initial_scene_pos;

	bool Load(pugi::xml_node&);
	bool Save(pugi::xml_node&) const;
	
	bool first_click = true;///
	iPoint origin, dest;///
	Pathfinding* path = nullptr;

private:
	void PlaceEnemies() const;
	

};

#endif // __j1SCENE_H__