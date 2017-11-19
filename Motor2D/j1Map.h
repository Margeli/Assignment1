#ifndef __j1MAP_H__
#define __j1MAP_H__

#include "PugiXml/src/pugixml.hpp"
#include "p2List.h"
#include "p2Point.h"
#include "j1Module.h"
#include "j1App.h"

#define GROUND_TILES 39	//nº of tiles that have the property ground in Tiled

enum EntityTypes;

struct Properties {
	struct Property
	{
		p2SString name;
		int value;
	};

	~Properties()
	{
		p2List_item<Property*>* item;
		item = list.start;

		while (item != NULL)
		{
			RELEASE(item->data);
			item = item->next;
		}

		list.clear();
	}

	int Get(const char* name, int default_value = 0) const;

	p2List<Property*>	list;
};


struct ObjectGroup {
	EntityTypes type;
	int x;
	int y;
};

struct Layer 
{
	p2SString name;
	uint width;
	uint height;
	float speed;
	
	uint* data=nullptr;
	Properties	properties;

	~Layer();

	inline uint Get(int x, int y) const
	{
		return data[(y*width) + x];
	}
};
	


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
	p2List<ObjectGroup*> objects;
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

	inline iPoint MapToWorld(int x, int y) const;
	iPoint WorldToMap(int x, int y) const;
	bool CreateWalkabilityMap(int& width, int& height, uchar** buffer, EntityTypes type) const;
	
	void PathDrawer();
	

private:
	bool LoadMap();
	bool LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set);
	bool LoadLayer(pugi::xml_node& node, Layer* layer);
	bool LoadProperties(pugi::xml_node& node, Properties& properties);
	bool LoadObjectGroup(pugi::xml_node& node, ObjectGroup* object);

	TileSet* GetTilesetFromTileId(int id) const;

	void PutMapColliders(int current_id, iPoint position);
	bool first_loop = true;

public:
	MapData data;

private:
	pugi::xml_document	map_file;
	p2SString			folder;
	bool				map_loaded;

};

#endif // __j1MAP_H__