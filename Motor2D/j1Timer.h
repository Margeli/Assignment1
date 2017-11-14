#ifndef __j1TIMER_H__
#define __j1TIMER_H__

#define NUM_FPS_SAMPLES 64

#include "p2Defs.h"

class j1Timer
{
public:

	j1Timer();

	void Start();
	uint32 Read() const;
	float ReadSec() const;
	float CalcFPS(float dt);

private:
	uint32	started_at;
};

#endif //__j1TIMER_H__