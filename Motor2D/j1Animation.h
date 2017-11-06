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
	float current_frame;
	int last_frame = 0;
	int loops = 0;

public:
	void PushBack(const SDL_Rect& rect) { if (last_frame <= MAX_FRAMES) frames[last_frame++] = rect;}

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

	bool Finished() const {return loops > 0;}
	void Reset() {current_frame = 0; loops = 0;}

	void LoadAnimations(p2SString name, j1Module* callback)
	{
		pugi::xml_parse_result result = anim_file.load_file("animations.xml");
		if (result != NULL)
		{			 
			pugi::xml_node anim_name = anim_file.child("animations").child(callback->name.GetString()).child(name.GetString());
			loop = anim_name.attribute("loop").as_bool();
			speed = anim_name.attribute("speed").as_float();
			for (pugi::xml_node animation= anim_name.child("animation"); animation; animation = animation.next_sibling("animation")) 
			{				
				PushBack({ animation.attribute("x").as_int(), animation.attribute("y").as_int(), animation.attribute("w").as_int(), animation.attribute("h").as_int() });
			}
		}	
	}
	void LoadEnemyAnimations(p2SString name, p2SString enemy)
	{
		pugi::xml_parse_result result = anim_file.load_file("animations.xml");
		if (result != NULL)
		{
			pugi::xml_node anim_name = anim_file.child("animations").child("enemies").child(enemy.GetString()).child(name.GetString());
			loop = anim_name.attribute("loop").as_bool();
			speed = anim_name.attribute("speed").as_float();
			for (pugi::xml_node animation = anim_name.child("animation"); animation; animation = animation.next_sibling("animation"))
			{
				PushBack({ animation.attribute("x").as_int(), animation.attribute("y").as_int(), animation.attribute("w").as_int(), animation.attribute("h").as_int() });
			}
		}
	}

private:
	pugi::xml_document	anim_file;
};

#endif