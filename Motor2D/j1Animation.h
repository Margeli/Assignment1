#ifndef __j1ANIMATION_H__
#define __j1ANIMATION_H__

#include "SDL/include/SDL_rect.h"
#include "PugiXml/src/pugixml.hpp"
#include "j1Module.h"
#include "p2SString.h"

#define MAX_FRAMES 20

class Animation
{
public:
	bool loop = true;
	float speed = 1.0f;
	SDL_Rect frames[MAX_FRAMES];


private:
	float current_frame = 0;
	int last_frame = 0;
	int loops = 0;

public:
	void PushBack(const SDL_Rect& rect);

	SDL_Rect& GetCurrentFrame()
	{
		if (this) 
		{
			current_frame += speed;
			if (current_frame >= last_frame)
			{
				current_frame = (loop) ? 0.0f : last_frame - 1;
				loops++;
			}
			return frames[(int)current_frame];
		}
	}

	bool Finished() const {
		return loops > 0;}
	void Reset() {current_frame = 0; loops = 0;}

	void LoadPlayerAnimations(p2SString name);
	void LoadEnemyAnimations(p2SString name, p2SString enemy);
	
private:
	pugi::xml_document	anim_file;
};

#endif