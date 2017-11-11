#include "j1Timer.h"
#include "SDL\include\SDL_timer.h"

uint64 j1Timer::frequency = 0;

j1Timer::j1Timer()
{
	if (frequency == 0)
		frequency = SDL_GetPerformanceFrequency();

	Start();
}

void j1Timer::Start()
{
	started_at = SDL_GetPerformanceCounter();		//PerfTimer
}

double j1Timer::ReadMs() const	//PerfTimer
{	
	return (SDL_GetPerformanceCounter() - started_at) / SDL_GetPerformanceFrequency();
}

uint64 j1Timer::ReadTicks() const	//PerfTimer
{
	return SDL_GetPerformanceCounter() - started_at;
}

float j1Timer::ReadSec() const	//Timer
{
	return SDL_GetTicks() - started_at;
}

void j1Timer::StartSec()	//Timer
{
	started_at = SDL_GetTicks();
}

uint32 j1Timer::Read() const	//Timer
{
	return SDL_GetTicks() - started_at;
}