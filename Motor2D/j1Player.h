#ifndef __j1PLAYER_H__
#define __j1PLAYER_H__

#include "j1Module.h"
#include "p2Point.h"

class SDL_Texture;

class j1Player : public j1Module
{
public:

	j1Player();

	// Destructor
	virtual ~j1Player();

	// Called before render is available
	bool Awake(pugi::xml_node& conf);

	// Called each loop iteration
	bool Update();

	// Called before quitting
	bool CleanUp();

	

	SDL_Texture* graphics = nullptr;

};


#endif // __j1PLAYER_H__