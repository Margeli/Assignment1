#include "p2Defs.h"
#include "p2Log.h"
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

void j1Pathfinding::PathDrawer()
{
}

void j1Pathfinding::SetMap(uint width, uint height, uchar* data)
{
	this->width = width;			//take info from 1jMap
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

// Looks for a node in this list and returns it's list node or NULL
p2List_item<PathNode>* PathList::Finder(const iPoint& point) const
{
	p2List_item<PathNode>* item = list.start;
	while (item)
	{
		if (item->data.position == point)
			return item;
		item = item->next;
	}
	return NULL;
}

// Returns the Pathnode with lowest score in this list or NULL if empty
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

PathNode::PathNode() : g(-1), h(-1), position(-1, -1), parent(NULL)
{}

PathNode::PathNode(int g, int h, const iPoint& pos, const PathNode* parent) : g(g), h(h), position(pos), parent(parent)
{}

PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), position(node.position), parent(node.parent)
{}

uint PathNode::FindWalkableAdjacents(PathList& list_to_fill) const // Fills the PathList of all valid adjacent pathnodes
{
	iPoint cell;
	uint before = list_to_fill.list.count();

	cell.create(position.x, position.y + 1);			//UP
	if (App->pathfind->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	cell.create(position.x, position.y - 1);			//DOWN
	if (App->pathfind->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	cell.create(position.x + 1, position.y);			//RIGHT
	if (App->pathfind->IsWalkable(cell))
		list_to_fill.list.add(PathNode(-1, -1, cell, this));

	cell.create(position.x - 1, position.y);			//LEFT
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
	h = position.DistanceTo(destination);

	return g + h;
}

int j1Pathfinding::CreatePath(const iPoint& origin, const iPoint& destination) 
{

	int steps_number = 0;

	if (IsWalkable(destination) == false || IsWalkable(origin) == false) { return -1; }

	PathList OpenList;
	PathList ClosedList;
	PathList AdjacentsList;

	PathNode adjacentnode;
	PathNode auxiliarnode;
	PathNode backtracknode;
	PathNode originode(0, origin.DistanceTo(destination), origin, nullptr);

	OpenList.list.add(originode);

	while (OpenList.list.start)
	{
		p2List_item<PathNode>* lowest_score_node = OpenList.GetNodeLowestScore();
		auxiliarnode = lowest_score_node->data;
		ClosedList.list.add(auxiliarnode);
		OpenList.list.del(lowest_score_node);

		if (auxiliarnode.position == destination)
		{
			for (backtracknode = auxiliarnode; backtracknode.parent != nullptr; backtracknode = *backtracknode.parent)
			{
				last_path.PushBack(backtracknode.position);
				if (backtracknode.position == origin) { last_path.Flip(); }
			}
			continue;
		}
		auxiliarnode.FindWalkableAdjacents(AdjacentsList);

		for (int iterations = 0; iterations < AdjacentsList.list.count(); ++iterations)
		{
			adjacentnode = AdjacentsList.list[iterations];

			if (ClosedList.Finder(adjacentnode.position) == nullptr)
			{
				adjacentnode.CalculateF(destination);
				ClosedList.list.add(adjacentnode);

				if (OpenList.Finder(adjacentnode.position) != nullptr)
				{
					if (OpenList.Finder(adjacentnode.position)->data.g >= adjacentnode.g)
					{
						p2List_item<PathNode>* better_path_node = OpenList.Finder(adjacentnode.position);
						better_path_node->data.parent = adjacentnode.parent;
					}
				}
				else { OpenList.list.add(adjacentnode); }
			}
		}
	}
	return steps_number;


}
