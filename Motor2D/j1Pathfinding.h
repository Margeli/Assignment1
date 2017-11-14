#ifndef __j1PATHFINDING_H__
#define __j1PATHFINDING_H__

#include "j1Module.h"
#include "p2Point.h"
#include "p2DynArray.h"
#include "j1App.h"
#include "p2PQueue.h"

#define INVALID_WALK_CODE 255
#define DEFAULT_PATH_LENGTH 50
#define MAX_PATHS 50
struct SDL_Texture;

struct Pathfinding {
	p2DynArray<iPoint>	path;
	p2PQueue<iPoint>	frontier;
	p2List<iPoint>		visited;
	p2List<iPoint>		breadcrumbs;

	void Clear();

};


class j1Pathfinding : public j1Module
{
public:
	j1Pathfinding();
	~j1Pathfinding();

	bool Start();

	bool CleanUp();

	//Draws the path 
	void DrawPath(const Pathfinding& path)const ;

	// Main function to request a path from A to B
	void CreatePath(const iPoint& origin, const iPoint& destination, Pathfinding* path);		
	
	

	Pathfinding* AddPath(const iPoint& origin, const iPoint& destination);


private:

	//Resets the path list
	void ResetPath();

	// Utility: return true if pos is inside the map boundaries
	bool CheckBoundaries(const iPoint& pos) const;

	void Path(iPoint goal, Pathfinding& path);

	// size of the map
	uint				 width;
	uint				height;
	SDL_Texture*		PathStep = nullptr;
	
	
	Pathfinding*		paths = nullptr;

	
};



#endif // __j1PATHFINDING_H__