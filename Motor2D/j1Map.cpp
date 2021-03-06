#include "p2Defs.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Render.h"
#include "j1Window.h"
#include "j1Textures.h"
#include "j1Collisions.h"
#include "j1Map.h"
#include "Brofiler/Brofiler.h"
#include "j1EntityManager.h"
#include "j1Pathfinding.h"
#include <math.h>

j1Map::j1Map() : j1Module(), map_loaded(false)
{
	name.create("map");
}

j1Map::~j1Map()
{}

bool j1Map::Awake(pugi::xml_node& config)
{
	LOG("Loading Map Parser");
	bool ret = true;
	
	folder.create(config.child("folder").child_value());

	return ret;
}

bool j1Map::CreateWalkabilityMap(int& width, int& height, uchar** buffer, EntityTypes type) const
{
	bool ret = false;
	p2List_item<Layer*>* item;
	item = data.layers.start;

	for (item = data.layers.start; item != NULL; item = item->next)
	{
		Layer* layer = item->data;
		if (type == EntityTypes::FLY) {
			if (layer->properties.Get("ground", 0) == 0)
				continue;
		}
		if (type == EntityTypes::TROLL) {
			if (layer->properties.Get("Path", 0) == 0)
				continue;		
		}
		uchar* map = new uchar[layer->width*layer->height];
		memset(map, 1, layer->width*layer->height);

		for (int y = 0; y < data.height; ++y)
		{
			for (int x = 0; x < data.width; ++x)
			{
				int i = (y*layer->width) + x;

				int tile_id = layer->Get(x, y);
				TileSet* tileset = (tile_id > 0) ? GetTilesetFromTileId(tile_id) : NULL;

				if (tileset != NULL)
				{
					map[i] = (tile_id - tileset->firstgid) > 0 ? 0 : 1;
					/*TileType* ts = tileset->GetTileType(tile_id);
					if(ts != NULL)
					{
					map[i] = ts->properties.Get("walkable", 1);
					}*/
				}
			}
		}

		*buffer = map;
		width = data.width;
		height = data.height;
		ret = true;

		break;
	}

	return ret;
}


void j1Map::Draw()
{
	BROFILER_CATEGORY("Map_Drawing", Profiler::Color::DarkBlue);
	if (map_loaded == false) { LOG("Couldn't load map.");  return; }

	int tile_num;
	for (p2List_item<Layer*>* layer_iterator = data.layers.start; layer_iterator != nullptr; layer_iterator = layer_iterator->next) 
	{
		tile_num = 0;
		SDL_Texture* texture = data.tilesets.At(0)->data->texture;
		uint layer_height = layer_iterator->data->height;
		uint layer_width = layer_iterator->data->width;
		float layer_speed = layer_iterator->data->speed;
		int camera = -App->render->camera.x;
		for (int row = 0; row < layer_height; row++)
		{
			for (int column = 0; column < layer_width; column++)
			{
				uint id = layer_iterator->data->data[tile_num];
				iPoint position = MapToWorld(column, row);


				if (first_loop&&layer_iterator->data->properties.Get("ground", 0)) {
					PutMapColliders(id, position);
				}
				if ((position.x <= camera*layer_speed + SCREEN_WIDTH) &&( position.x>=camera*layer_speed- LEFT_LIMIT_RENDER))//need to improve (not all layers load at the same speed)
					App->render->Blit(texture, position.x, position.y, &data.tilesets.At(0)->data->GetTileRect(id), layer_speed);
				tile_num++;
			}
		}
	}
	
	first_loop = false;
}

 iPoint j1Map::WorldToMap(int x, int y) const 
{
	iPoint ret;

	ret.x = x / data.tile_width;
	ret.y = y / data.tile_height;

	return ret;
}

inline iPoint j1Map::MapToWorld(int x, int y) const
{
	iPoint ret;

	ret.x = x* App->map->data.tilesets.At(0)->data->tile_width;	
	ret.y = y* App->map->data.tilesets.At(0)->data->tile_height;

	return ret;
}




bool j1Map::CleanUp()
{
	LOG("Unloading map");

	p2List_item<TileSet*>* item;
	item = data.tilesets.start;

	while(item != NULL)
	{
		RELEASE(item->data);
		item = item->next;
	}
	data.tilesets.clear();

	p2List_item<Layer*>* item2;
	item2 = data.layers.start;
	while (item2 != NULL)
	{
		RELEASE(item2->data);
		item2 = item2->next;
	}
	data.layers.clear();

	
	p2List_item<ObjectGroup*>* item3;
	item3 = data.objects.start;
	while (item3 != NULL)
	{
		RELEASE(item3->data);
		item3 = item3->next;
	}
	data.objects.clear();


	first_loop = true;

	map_file.reset();

	return true;
}

bool j1Map::Load(const char* file_name)
{
	bool ret = true;
	p2SString tmp("%s%s", folder.GetString(), file_name);

	pugi::xml_parse_result result = map_file.load_file(tmp.GetString());

	if(result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", file_name, result.description());
		ret = false;
	}

	if(ret == true)
	{
		ret = LoadMap();
	}

	pugi::xml_node tileset;
	for(tileset = map_file.child("map").child("tileset"); tileset && ret; tileset = tileset.next_sibling("tileset"))
	{
		TileSet* set = new TileSet();

		if(ret == true)
		{
			ret = LoadTilesetDetails(tileset, set);
		}

		if(ret == true)
		{
			ret = LoadTilesetImage(tileset, set);
		}

		data.tilesets.add(set);
	}

	pugi::xml_node layer;
	for (layer = map_file.child("map").child("layer"); layer && ret; layer = layer.next_sibling("layer"))
	{
		
		Layer* map_layer = new Layer();

		
		if (ret == true)
		{
			ret = LoadLayer(layer, map_layer);
		}

		data.layers.add(map_layer);
	}

	pugi::xml_node object;
	for (object = map_file.child("map").child("objectgroup").child("object"); object&&ret; object = object.next_sibling("object"))
	{
		ObjectGroup* map_object = new ObjectGroup();
		if (ret == true)
		{
			ret = LoadObjectGroup(object, map_object);
		}

		data.objects.add(map_object);
	}

	if(ret == true)
	{
		LOG("Successfully parsed map XML file: %s", file_name);
		LOG("width: %d height: %d", data.width, data.height);
		LOG("tile_width: %d tile_height: %d", data.tile_width, data.tile_height);

		p2List_item<TileSet*>* item = data.tilesets.start;
		while(item != NULL)
		{
			TileSet* s = item->data;
			LOG("Tileset ----");
			LOG("name: %s firstgid: %d", s->name.GetString(), s->firstgid);
			LOG("tile width: %d tile height: %d", s->tile_width, s->tile_height);
			LOG("spacing: %d margin: %d", s->spacing, s->margin);
			item = item->next;
		}

		p2List_item<Layer*>* item_layer = data.layers.start;
		while(item_layer != NULL)
		{
			Layer* l = item_layer->data;
			LOG("Layer ----");
			LOG("name: %s", l->name.GetString());
			LOG("tile width: %d tile height: %d", l->width, l->height);
			item_layer = item_layer->next;
		}
	}

	map_loaded = ret;

	return ret;
}

bool j1Map::LoadMap()
{
	bool ret = true;
	pugi::xml_node map = map_file.child("map");

	if(map == NULL)
	{
		LOG("Error parsing map xml file: Cannot find 'map' tag.");
		ret = false;
	}
	else
	{
	
		data.tile_width = map.attribute("tilewidth").as_int();
		data.tile_height = map.attribute("tileheight").as_int();
		data.width = map.attribute("width").as_int();
		data.height = map.attribute("height").as_int();
		p2SString bg_color(map.attribute("backgroundcolor").as_string());

		data.background_color.r = 0;
		data.background_color.g = 0;
		data.background_color.b = 0;
		data.background_color.a = 0;

		if(bg_color.Length() > 0)
		{
			p2SString red, green, blue;
			bg_color.SubString(1, 2, red);
			bg_color.SubString(3, 4, green);
			bg_color.SubString(5, 6, blue);

			int v = 0;

			sscanf_s(red.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.r = v;

			sscanf_s(green.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.g = v;

			sscanf_s(blue.GetString(), "%x", &v);
			if(v >= 0 && v <= 255) data.background_color.b = v;
		}

		p2SString orientation(map.attribute("orientation").as_string());

		if(orientation == "orthogonal")
		{
			data.type = MAPTYPE_ORTHOGONAL;
		}
		else if(orientation == "isometric")
		{
			data.type = MAPTYPE_ISOMETRIC;
		}
		else if(orientation == "staggered")
		{
			data.type = MAPTYPE_STAGGERED;
		}
		else
		{
			data.type = MAPTYPE_UNKNOWN;
		}
	}

	return ret;
}

bool j1Map::LoadTilesetDetails(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	set->name.create(tileset_node.attribute("name").as_string());
	set->firstgid = tileset_node.attribute("firstgid").as_int();
	set->tile_width = tileset_node.attribute("tilewidth").as_int();
	set->tile_height = tileset_node.attribute("tileheight").as_int();
	set->margin = tileset_node.attribute("margin").as_int();
	set->spacing = tileset_node.attribute("spacing").as_int();


	pugi::xml_node offset = tileset_node.child("tileoffset");

	if (offset != NULL)
	{
		set->offset_x = offset.attribute("x").as_int();
		set->offset_y = offset.attribute("y").as_int();
	}
	else
	{
		set->offset_x = 0;
		set->offset_y = 0;
	}

	p2SString ground = " ground";
	set->ground_id_tiles = new uint[GROUND_TILES];
	memset(set->ground_id_tiles, 0, GROUND_TILES * sizeof(uint));	

	/*p2SString wall = " wall";
	set->wall_id_tiles = new uint[39];
	memset(set->wall_id_tiles, 0, 39 * sizeof(uint));	//COMMENTED IS FOR IF WE INCLUDE WALLS	
	uint j = 0;*/
	uint i = 0;
	
	for (pugi::xml_node& tile_node = tileset_node.child("tile"); tile_node; tile_node = tile_node.next_sibling("tile")) 
	{	
			if (tile_node.child("properties").child("property").attribute("value").as_bool())
			{
				bool is_ground = strcmp(ground.GetString(), tile_node.child("properties").child("property").attribute("name").as_string());
				//bool is_wall = strcmp(wall.GetString(), tile_node.child("properties").child("property").attribute("name").as_string());
				if (is_ground)
				{				
					set->ground_id_tiles[i] = tile_node.attribute("id").as_uint();
					i++;
				}
				/*if (is_wall)
				{
					set->wall_id_tiles[j] = tile_node.attribute("id").as_uint();
					j++;
				}*/
			}
	}
	

	return ret;
}

bool j1Map::LoadTilesetImage(pugi::xml_node& tileset_node, TileSet* set)
{
	bool ret = true;
	pugi::xml_node image = tileset_node.child("image");

	if(image == NULL)
	{
		LOG("Error parsing tileset xml file: Cannot find 'image' tag.");
		ret = false;
	}
	else
	{
		set->texture = App->tex->Load(PATH(folder.GetString(), image.attribute("source").as_string()));
		int w, h;
		SDL_QueryTexture(set->texture, NULL, NULL, &w, &h);
		set->tex_width = image.attribute("width").as_int();

		if(set->tex_width <= 0)
		{
			set->tex_width = w;
		}

		set->tex_height = image.attribute("height").as_int();

		if(set->tex_height <= 0)
		{
			set->tex_height = h;
		}

		set->num_tiles_width = set->tex_width / set->tile_width;
		set->num_tiles_height = set->tex_height / set->tile_height;
	}

	return ret;
}

bool j1Map::LoadLayer(pugi::xml_node& node, Layer* layer)
{
	layer->name = node.attribute("name").as_string();
	layer->height = node.attribute("height").as_uint();
	layer->width = node.attribute("width").as_uint();
	LoadProperties(node, layer->properties);
	

	pugi::xml_node prop = node.child("properties").first_child();	
	layer->speed = prop.attribute("value").as_float();	
	//prop = prop.next_sibling();
	//layer->initial_player_position.x = prop.attribute("value").as_int();
	//prop = prop.next_sibling();

	//layer->initial_player_position.y = prop.attribute("value").as_int();



	uint data_size = layer->width * layer->height;
	layer->data= new uint[data_size];

	memset(layer->data, 0, data_size*sizeof(uint));
	
	pugi::xml_node tile_node_iterator = node.child("data").child("tile");

	for (uint i = 0; i < data_size; i++) {
		layer->data[i] = tile_node_iterator.attribute("gid").as_uint();
		tile_node_iterator = tile_node_iterator.next_sibling();
	}
	return true;
}

bool j1Map::LoadObjectGroup(pugi::xml_node& node, ObjectGroup* object) {
	
	object->x = node.attribute("x").as_uint();
	object->y = node.attribute("y").as_uint();
	p2SString tpe = node.attribute("type").as_string();
	if (tpe == "Troll") {
		object->type = TROLL;	
	}
	if (tpe == "Troll2") {
		object->type = TROLL2;
	}
	if (tpe == "Troll3") {
		object->type = TROLL3;
	}
	if (tpe == "Fly") {
		object->type = FLY;
	}
	if (tpe == "Pickup") {
		object->type = COLLECT;
	}
	return true;
}

void j1Map::PutMapColliders(int current_id, iPoint position) 
{
	for (uint i = 0; i < GROUND_TILES; i++) 
	{ 
		if (93 == current_id - 1 || 85 == current_id - 1){
			App->collis->AddCollider({ 7+position.x, position.y,data.tilesets.At(0)->data->tile_width-7, data.tilesets.At(0)->data->tile_height }, COLLIDER_GROUND);
			break;
		}
		if (95 == current_id - 1 || 87 == current_id - 1) {		
			App->collis->AddCollider({ position.x, position.y,data.tilesets.At(0)->data->tile_width - 7, data.tilesets.At(0)->data->tile_height }, COLLIDER_GROUND);
			break;
		}
		if (data.tilesets.At(0)->data->ground_id_tiles[i] == current_id - 1)
		{

			App->collis->AddCollider({ position.x, position.y,data.tilesets.At(0)->data->tile_width, data.tilesets.At(0)->data->tile_height }, COLLIDER_GROUND);
			break;
		}
	}
}

TileSet* j1Map::GetTilesetFromTileId(int id) const
{
	p2List_item<TileSet*>* item = data.tilesets.start;
	TileSet* set = item->data;

	while (item)
	{
		if (id < item->data->firstgid)
		{
			set = item->prev->data;
			break;
		}
		set = item->data;
		item = item->next;
	}

	return set;
}

// Load a group of properties from a node and fill a list with it
bool j1Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = false;

	pugi::xml_node data = node.child("properties");

	if (data != NULL)
	{
		pugi::xml_node prop;

		for (prop = data.child("property"); prop; prop = prop.next_sibling("property"))
		{
			Properties::Property* p = new Properties::Property();

			p->name = prop.attribute("name").as_string();
			p->value = prop.attribute("value").as_float();

			properties.list.add(p);
		}
	}

	return ret;
}

SDL_Rect TileSet::GetTileRect(int id) const
{
	int relative_id = id - firstgid;
	SDL_Rect rect;
	rect.w = tile_width;
	rect.h = tile_height;
	rect.x = margin + ((rect.w + spacing) * (relative_id % num_tiles_width));
	rect.y = margin + ((rect.h + spacing) * (relative_id / num_tiles_width));
	return rect;
}

int Properties::Get(const char* value, int default_value) const
{
	p2List_item<Property*>* item = list.start;

	while (item)
	{
		if (item->data->name == value)
			return item->data->value;
		item = item->next;
	}

	return default_value;
}

Layer::~Layer() { RELEASE(data); }