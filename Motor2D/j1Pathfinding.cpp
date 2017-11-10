#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Pathfinding.h"

j1Pathfinding::j1Pathfinding() : j1Module(), map(NULL), last_path(DEFAULT_PATH_LENGTH), width(0), height(0)
{
	name.create("pathfinding");
}

j1Pathfinding::~j1Pathfinding()
{
	RELEASE_ARRAY(map);
}

bool j1Pathfinding::CleanUp()
{
	LOG("Freeing pathfinding library.");

	last_path.Clear();
	RELEASE_ARRAY(map);
	return true;
}

void j1Pathfinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;
	this->height = height;

	RELEASE_ARRAY(map);
	map = new uchar[width*height];
	memcpy(map, data, width*height);
}

// Utility: return true if pos is inside the map boundaries
bool j1Pathfinding::CheckBoundaries(const iPoint& pos) const
{
	return (pos.x >= 0 && pos.x <= (int)width && pos.y >= 0 && pos.y <= (int)height);
}

// Utility: returns true if the tile is walkable
bool j1Pathfinding::IsWalkable(const iPoint& pos) const
{
	uchar t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}

// Utility: return the walkability value of a tile
uchar j1Pathfinding::GetTileAt(const iPoint& pos) const
{
	if (CheckBoundaries(pos))
		return map[(pos.y*width) + pos.x];

	return INVALID_WALK_CODE;
}

// To request all tiles involved in the last generated path
const p2DynArray<iPoint>* j1Pathfinding::GetLastPath() const
{
	return &last_path;
}

// PathList ------------------------------------------------------------------------
// Looks for a node in this list and returns it's list node or NULL
// ---------------------------------------------------------------------------------
p2List_item<PathNode>* PathList::Find(const iPoint& point) const
{
	p2List_item<PathNode>* item = list.start;
	while (item)
	{
		if (item->data.pos == point)
			return item;
		item = item->next;
	}
	return NULL;
}

// PathList ------------------------------------------------------------------------
// Returns the Pathnode with lowest score in this list or NULL if empty
// ---------------------------------------------------------------------------------
p2List_item<PathNode>* PathList::GetNodeLowestScore() const
{
	p2List_item<PathNode>* ret = NULL;
	int min = 65535;

	p2List_item<PathNode>* item = list.end;
	while (item)
	{
		if (item->data.Score() < min)
		{
			min = item->data.Score();
			ret = item;
		}
		item = item->prev;
	}
	return ret;
}

// PathNode -------------------------------------------------------------------------
// Convenient constructors
// ----------------------------------------------------------------------------------
PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

 

uint PathNode::FindWalkableAdjacents(PathList& list_to_fill) const // Fills the PathList of all valid adjacent pathnodes
{
	iPoint cell;
	uint before = list_to_fill.list.count();

	cell.create(pos.x, pos.y + 1);			//UP
	if (App->pathfind->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	cell.create(pos.x, pos.y - 1);			//DOWN
	if (App->pathfind->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	cell.create(pos.x + 1, pos.y);			//RIGHT
	if (App->pathfind->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	cell.create(pos.x - 1, pos.y);			//LEFT
	if (App->pathfind->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	return list_to_fill.list.count();
}

int PathNode::Score() const	//Calculates this tile score
{
	return g + h;
}

int PathNode::CalculateF(const iPoint& destination) // Calculate the F for a specific destination tile
{
	g = parent->g + 1;
	h = pos.DistanceTo(destination);

	return g + h;
}

int j1Pathfinding::CreatePath(const iPoint& origin, const iPoint& destination) 
{
	// TODO 1: if origin or destination are not walkable, return -1
	//return number of steps in the creation of the path or -1

	if (IsWalkable(destination) == false || IsWalkable(origin) == false) { return -1; }

	// TODO 2: Create two lists: open, close
	// Add the origin tile to open
	// Iterate while we have tile in the open list

	PathList open;
	PathList close;

	PathNode originode(0, origin.DistanceTo(destination), origin, nullptr);
	open.list.add(originode);
	PathNode move;


	while (open.list.start)
	{
		// TODO 3: Move the lowest score cell from open list to the closed list

		p2List_item<PathNode>* lowest_score_cell = open.GetNodeLowestScore();
		move = lowest_score_cell->data;
		close.list.add(move);

		if (move.pos == destination)
		{
			for (PathNode backtrack = move; backtrack.parent != nullptr; backtrack = *backtrack.parent)
			{

			}
		}

	}


	// TODO 4: If we just added the destination, we are done!
	// Backtrack to create the final path
	// Use the Pathnode::parent and Flip() the path when you are finish

	// TODO 5: Fill a list of all adjancent nodes

	// TODO 6: Iterate adjancent nodes:
	// ignore nodes in the closed list
	// If it is NOT found, calculate its F and add it to the open list
	// If it is already in the open list, check if it is a better path (compare G)
	// If it is a better path, Update the parent

	return -1;
}
