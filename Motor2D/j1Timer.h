#ifndef __j1TIMER_H__
#define __j1TIMER_H__

#include "p2Defs.h"

class j1Timer
{
public:

	j1Timer();

	void Start();
	uint32 Read() const;
	float ReadSec() const;
	void SetZero();
	bool IsZero()const;
	void SubstractTime(j1Timer timer_to_copy_from);

private:
	uint32	started_at;
};

#endif //__j1TIMER_H__