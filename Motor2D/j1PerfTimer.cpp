#include "j1PerfTimer.h"
#include "SDL\include\SDL_timer.h"

uint64 j1PerfTimer::frequency = 0;

j1PerfTimer::j1PerfTimer()
{
	if (frequency == 0)
		frequency = SDL_GetPerformanceFrequency();		//Ticks per second

	Start();
}

void j1PerfTimer::Start()
{
	started_at = SDL_GetPerformanceCounter();			//Starting value
}

double j1PerfTimer::ReadMs() const
{
	return 1000.0 * (double(ReadTicks()) / double(frequency));			//Counts miliseocnds 
}

uint64 j1PerfTimer::ReadTicks() const
{
	return SDL_GetPerformanceCounter() - started_at;		//Current value
}


