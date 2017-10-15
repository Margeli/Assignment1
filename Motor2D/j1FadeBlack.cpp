#include <math.h>
#include "j1App.h"
#include "j1FadeBlack.h"
#include "j1Render.h"
#include "j1Player.h" 
#include "j1Audio.h"
#include "j1Window.h"
#include "p2Log.h"
#include "j1Scene.h" 
#include "j1Scene2.h"
#include "SDL/include/SDL_render.h"
#include "SDL/include/SDL_timer.h"

j1FadeBlack::j1FadeBlack()
{
	screen = { 0, 0, SCREEN_WIDTH * SCREEN_SIZE, SCREEN_HEIGHT * SCREEN_SIZE };
}

j1FadeBlack::~j1FadeBlack() {}

bool j1FadeBlack::Awake()
{
	LOG("Preparing Fade Screen");
	SDL_SetRenderDrawBlendMode(App->render->renderer, SDL_BLENDMODE_BLEND);
	return true;
}

bool j1FadeBlack::Update()
{
	if (current_step == fade_step::none)
		return true;

	Uint32 now = SDL_GetTicks() - start_time;
	float normalized = MIN(1.0f, (float)now / (float)total_time);

	switch (current_step)
	{
		case fade_step::fade_to_black:
		{
			if (now >= total_time)
			{
				App->render->camera.x = 0;
				App->render->camera.y = 0;

				App->audio->CleanUp();

				total_time += total_time;
				start_time = SDL_GetTicks();
				current_step = fade_step::fade_from_black;
			}
		} break;

		case fade_step::fade_from_black:
		{
			normalized = 1.0f - normalized;
			if (now >= total_time) 
			{
				current_step = fade_step::none;
				App->scene->fading = false;
			}
		} break;
	}

	SDL_SetRenderDrawColor(App->render->renderer, 0, 0, 0, (Uint8)(normalized * 255.0f));
	SDL_RenderFillRect(App->render->renderer, &screen);

	return true;
}

bool j1FadeBlack::FadeToBlack(j1Module* module_off, j1Module* module_on, float time)
{
	bool ret = false;

	if (current_step == fade_step::none)
	{
		current_step = fade_step::fade_to_black;
		start_time = SDL_GetTicks();
		total_time = 0;
		to_enable = module_on;
		to_disable = module_off;
		ret = true;
	}

	return ret;
}

bool j1FadeBlack::IsFading() const
{
	return current_step != fade_step::none;
}