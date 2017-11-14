#include "j1Timer.h"
#include "SDL\include\SDL_timer.h"

j1Timer::j1Timer()
{
	Start();
}

//Miliseconds when starting
void j1Timer::Start()
{
	started_at = SDL_GetTicks();	
}

//Current miliseconds
uint32 j1Timer::Read() const
{
	return SDL_GetTicks() - started_at;		
}

//Current seconds
float j1Timer::ReadSec() const
{
	return float(SDL_GetTicks() - started_at) / 1000.0f;	
}

float  j1Timer::CalcFPS(float dt)		//Not defined in header!
{
	float fps_samples[NUM_FPS_SAMPLES];
	int currentSample = 0;

	fps_samples[currentSample % NUM_FPS_SAMPLES] = 1.0f / dt;

	float fps = 0;

	for (int i = 0; i < NUM_FPS_SAMPLES; i++)
	{
		fps += fps_samples[i];
	}

	fps /= NUM_FPS_SAMPLES;
	return fps;
}