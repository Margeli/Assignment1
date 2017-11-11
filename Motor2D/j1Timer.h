#ifndef __j1TIMER_H__
#define __j1TIMER_H__

#include "p2Defs.h"

class j1Timer
{
public:
	j1Timer();

	void Start();
	void StartSec();
	double ReadMs() const;				
	uint64 ReadTicks() const;
	uint32 Read() const;
	float ReadSec() const;

private:
	uint64	started_at;
	static uint64 frequency;
};

#endif //__j1TIMER_H__