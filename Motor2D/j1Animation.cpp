#include "j1Animation.h"

void Animation::PushBack(const SDL_Rect& rect) { 
	if (last_frame <= MAX_FRAMES) 
		frames[last_frame++] = rect; 
}

void Animation::LoadAnimations(p2SString name, j1Module* callback) {
	pugi::xml_parse_result result = anim_file.load_file("animations.xml");
	if (result != NULL)
	{
		pugi::xml_node anim_name = anim_file.child("animations").child(callback->name.GetString()).child(name.GetString());
		loop = anim_name.attribute("loop").as_bool();
		speed = anim_name.attribute("speed").as_float();
		for (pugi::xml_node animation = anim_name.child("animation"); animation; animation = animation.next_sibling("animation"))
		{
			PushBack({ animation.attribute("x").as_int(), animation.attribute("y").as_int(), animation.attribute("w").as_int(), animation.attribute("h").as_int() });
		}
	}
}

void Animation::LoadEnemyAnimations(p2SString name, p2SString enemy)
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