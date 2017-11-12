#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1App.h"

#define GROUND_TILES 39	//nº of tiles that have the property ground in Tiled

struct Layer 
{
	p2SString name;
	uint width;
	uint height;
	float speed;
	iPoint initial_player_position;
	iPoint initial_enemie_position;
	uint* data=nullptr;

	~Layer();
};
	
inline iPoint GetXYfromTile(int x, int y);

struct TileSet
{
	SDL_Rect GetTileRect(int id) const;

	SDL_Texture*		texture = nullptr;
	p2SString			name;
	int					firstgid;
	int					margin;
	int					spacing;
	int					tile_width;
	int					tile_height;
	int					tex_width;
	int					tex_height;
	int					num_tiles_width;
	int					num_tiles_height;
	int					offset_x;
	int					offset_y;
	uint*				ground_id_tiles = nullptr;
	uint*				wall_id_tiles = nullptr;
};

enum MapTypes
{
	MAPTYPE_UNKNOWN = 0,
	MAPTYPE_ORTHOGONAL,
	MAPTYPE_ISOMETRIC,
	MAPTYPE_STAGGERED
};

struct MapData
{
	int					width;
	int					height;
	int					tile_width;
	int					tile_height;
	SDL_Color			background_color;
	MapTypes			type;
	p2List<TileSet*>	tilesets;
	p2List<Layer*>		layers;

};

class j1Map : public j1Module
{
public:
	j1Map();

	virtual ~j1Map();

	bool Awake(pugi::xml_node& conf);

	void Draw();

	bool CleanUp();

	bool Load(const char* path);

	iPoint MapToWorld(int x, int y) const;

	void PathDrawer();

private:
	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, Layer* layer);

	void PutMapColliders(int current_id, iPoint position);
	bool first_loop = true;

public:
	MapData data;

private:
	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;
	SDL_Texture*		node = nullptr;
};

#endif // __j1MAP_H__