#ifndef __j1PATHFINDING_H__
#define __j1PATHFINDING_H__

#include "j1Module.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "j1App.h"
#include "p2PQueue.h"

#define INVALID_WALK_CODE 255
#define DEFAULT_PATH_LENGTH 50
struct SDL_Texture;

class j1Pathfinding : public j1Module
{
public:
	j1Pathfinding();
	~j1Pathfinding();

	bool Start();

	bool CleanUp();

	//Draws the path 
	void DrawPath();

	//Resets the path list
	void ResetPath();

	// Main function to request a path from A to B
	void CreatePath(const iPoint& origin, const iPoint& destination);
		
	// Utility: return true if pos is inside the map boundaries
	bool CheckBoundaries(const iPoint& pos) const;
	

private:

	void Path(iPoint goal);

	// size of the map
	uint				 width;
	uint				height;
	SDL_Texture*		PathStep = nullptr;
	
	p2PQueue<iPoint>	frontier;
	p2List<iPoint>		visited;
	p2List<iPoint>		breadcrumbs;
	p2DynArray<iPoint>	path;
};



#endif // __j1PATHFINDING_H__