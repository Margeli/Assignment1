#include "p2Defs.h"
#include "p2Log.h"
#include "j1Pathfinding.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Map.h"

j1Pathfinding::j1Pathfinding() : j1Module()
{
	name.create("pathfinding");
	

	
		path_root = nullptr;	
	
}

j1Pathfinding::~j1Pathfinding()
{
	
}
bool j1Pathfinding::Start()
{
	PathStep = App->tex->Load("textures/node.png");
	if (PathStep == nullptr) {
		return false;
	}
	path_root = new Pathfinding();
	width = App->map->data.width;
	height = App->map->data.height;
	return true;
}

bool j1Pathfinding::CleanUp()
{
	
		if (path_root != nullptr) {
			path_root->Clear();
			delete path_root;
			path_root = nullptr;
		}
	
	
	
	App->tex->UnLoad(PathStep);

	return true;
}



// Utility: return true if pos is inside the map boundaries
bool j1Pathfinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width && pos.y >= 0 && pos.y <= (int)height);
}

void j1Pathfinding::CreatePath(const iPoint& origin, const iPoint& destination, Pathfinding* path  ) 
{
	
	path->Clear();
	iPoint goal = App->map->WorldToMap(destination.x, destination.y);
	iPoint start = App->map->WorldToMap(origin.x, origin.y);
	if (CheckBoundaries(goal)) {// Check if it's walkable

		path->frontier.Push(start, 0);

		iPoint curr;
		while (path->frontier.Count() != 0) {
			if (curr == goal)
				break;
			path->frontier.Pop(curr);

			iPoint neighbors[4];
			neighbors[0].create(curr.x + 1, curr.y + 0);
			neighbors[1].create(curr.x + 0, curr.y + 1);
			neighbors[2].create(curr.x - 1, curr.y + 0);
			neighbors[3].create(curr.x + 0, curr.y - 1);

			for (int i = 0; i < 4; i++) {
				uint distance = neighbors[i].DistanceTo(goal);
				if (CheckBoundaries(neighbors[i])) {
					if (path->visited.find(neighbors[i]) == -1) {
						path->frontier.Push(neighbors[i], distance);
						path->visited.add(neighbors[i]);
						path->breadcrumbs.add(curr);
					}
				}
			}
		}
		Path(goal, *path);
	}
}
void j1Pathfinding::DrawPath(const Pathfinding& path) const {

	// Draw path
	for (uint i = 0; i < path.path.Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path.path[i].x, path.path[i].y);
		App->render->Blit(PathStep, pos.x, pos.y);
	}
}

void j1Pathfinding::Path(iPoint goal, Pathfinding& path) 
{
	path.path.Clear();
	iPoint curr = goal;
	p2List_item<iPoint>* BC_iterator = path.breadcrumbs.end;
	path.path.PushBack(curr);

	if (path.visited.find(curr) >= 0)
	{
		while (BC_iterator != path.breadcrumbs.start) 
		{
			curr = path.breadcrumbs[path.visited.find(curr)];
			path.path.PushBack(curr);
			BC_iterator = BC_iterator->prev;
		}
	}
}

Pathfinding* j1Pathfinding::FindPath(const iPoint& origin, const iPoint& destination) {

	Pathfinding* ret = nullptr;			
			
	ret = path_root;
	CreatePath(origin, destination, path_root);		
		
	return ret;
}
void Pathfinding::Clear() 
{
	frontier.Clear();
	visited.clear();
	breadcrumbs.clear();
	path.Clear();
}