#include "p2Defs.h"
#include "p2Log.h"
#include "j1Pathfinding.h"
#include "j1Textures.h"
#include "j1Render.h"
#include "j1Map.h"

j1Pathfinding::j1Pathfinding() : j1Module()
{
	name.create("pathfinding");
}

j1Pathfinding::~j1Pathfinding()
{
}
bool j1Pathfinding::Start()
{
	PathStep = App->tex->Load("textures/node.png");
	if (PathStep == nullptr) { int i = 10; }
	width = App->map->data.width;
	height = App->map->data.height;
	return true;
}
bool j1Pathfinding::CleanUp()
{
	ResetPath();
	path.Clear();	
	App->tex->UnLoad(PathStep);

	return true;
}

void j1Pathfinding::ResetPath() 
{
	frontier.Clear();
	visited.clear();
	breadcrumbs.clear();
}

// Utility: return true if pos is inside the map boundaries
bool j1Pathfinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width && pos.y >= 0 && pos.y <= (int)height);
}

void j1Pathfinding::CreatePath(const iPoint& origin, const iPoint& destination) 
{
	ResetPath();

	iPoint goal = App->map->WorldToMap(destination.x, destination.y);
	iPoint start = App->map->WorldToMap(origin.x, origin.y);

	if (CheckBoundaries(goal))  // Check if it's walkable
	{
		iPoint curr;
		frontier.Push(start, 0);

		while (frontier.Count() != 0) 
		{
			if (curr == goal)
				break;
			frontier.Pop(curr);

			iPoint neighbors[4];
			neighbors[0].create(curr.x + 1, curr.y + 0);
			neighbors[1].create(curr.x + 0, curr.y + 1);
			neighbors[2].create(curr.x - 1, curr.y + 0);
			neighbors[3].create(curr.x + 0, curr.y - 1);

			for (int i = 0; i < 4; i++) 
			{
				uint distance = neighbors[i].DistanceTo(goal);
				if (CheckBoundaries(neighbors[i])) 
				{
					if (visited.find(neighbors[i]) == -1) 
					{						
						frontier.Push(neighbors[i], distance);
						visited.add(neighbors[i]);
						breadcrumbs.add(curr);
					}
				}
			}
		}
		Path(goal);
	}
}

void j1Pathfinding::DrawPath() 
{
	for (uint i = 0; i < path.Count(); ++i)
	{
		iPoint pos = App->map->MapToWorld(path[i].x, path[i].y);
		App->render->Blit(PathStep, pos.x, pos.y);
	}
}

void j1Pathfinding::Path(iPoint goal) 
{
	path.Clear();
	iPoint curr = goal;
	p2List_item<iPoint>* BC_iterator = breadcrumbs.end;
	path.PushBack(curr);

	if (visited.find(curr) >= 0)
	{
		while (BC_iterator != breadcrumbs.start) 
		{
			curr = breadcrumbs[visited.find(curr)];
			path.PushBack(curr);
			BC_iterator = BC_iterator->prev;
		}
	}
}