
#include "Engine.h"
#include "Render.h"
#include "Textures.h"
#include "Map.h"
#include "Log.h"
#include "Physics.h"

#include <math.h>

Map::Map() : Module(), mapLoaded(false)
{
	name = "map";
}

// Destructor
Map::~Map()
{}

// Called before render is available
bool Map::Awake()
{
	name = "map";
	LOG("Loading Map Parser");

	return true;
}

bool Map::Start() {

	return true;
}

bool Map::Update(float dt)
{
	bool ret = true;

	if (mapLoaded) {

		// L07 TODO 5: Prepare the loop to draw all tiles in a layer + DrawTexture()
		// iterate all tiles in a layer
		for (const auto& mapLayer : mapData.layers) {
			//Check if the property Draw exist get the value, if it's true draw the lawyer
			if (mapLayer->properties.GetProperty("Draw") != NULL && mapLayer->properties.GetProperty("Draw")->value == true) {
				for (int i = 0; i < mapData.width; i++) {
					for (int j = 0; j < mapData.height; j++) {

						//Get the gid from tile
						int gid = mapLayer->Get(i, j);
						//Check if the gid is different from 0 - some tiles are empty
						if (gid != 0) {
							TileSet* tileSet = GetTilesetFromTileId(gid);
							if (tileSet != nullptr) {
								//Get the Rect from the tileSetTexture;
								SDL_Rect tileRect = tileSet->GetRect(gid);
								//Get the screen coordinates from the tile coordinates
								Vector2D mapCoord = MapToWorld(i, j);
								//Draw the texture
								Engine::GetInstance().render->DrawTexture(tileSet->texture, SDL_FLIP_NONE, mapCoord.getX(), mapCoord.getY(), &tileRect);
							}
						}
					}
				}
			}
		}
	}

	return ret;
}

// L09: TODO 2: Implement function to the Tileset based on a tile id
TileSet* Map::GetTilesetFromTileId(int gid) const
{
	TileSet* set = nullptr;

	for (const auto& tileset : mapData.tilesets) {
		if (gid >= tileset->firstGid && gid < (tileset->firstGid + tileset->tileCount)) {
			set = tileset;
			break;
		}
	}

	return set;
}

// Called before quitting
bool Map::CleanUp()
{
	LOG("Unloading map");

	// L06: TODO 2: Make sure you clean up any memory allocated from tilesets/map
	for (const auto& tileset : mapData.tilesets) {
		delete tileset;
	}
	mapData.tilesets.clear();

	// L07 TODO 2: clean up all layer data
	for (const auto& layer : mapData.layers)
	{
		delete layer;
	}
	mapData.layers.clear();

	return true;
}

// Load new map
bool Map::Load(std::string path, std::string fileName)
{
	for (const auto& layer : collisions) {
		Engine::GetInstance().physics->DeletePhysBody(layer);
	}

	collisions.clear();
	posBonfire.clear();
	posPoison.clear();
	posEnemy.clear();
	mapData.layers.clear();

	bool ret = false;

	// Assigns the name of the map file and the path
	mapFileName = fileName;
	mapPath = path;
	std::string mapPathName = mapPath + mapFileName;

	pugi::xml_document mapFileXML;
	pugi::xml_parse_result result = mapFileXML.load_file(mapPathName.c_str());

	if (result == NULL)
	{
		LOG("Could not load map xml file %s. pugi error: %s", mapPathName.c_str(), result.description());
		ret = false;
	}
	else {

		mapData.width = mapFileXML.child("map").attribute("width").as_int();
		mapData.height = mapFileXML.child("map").attribute("height").as_int();
		mapData.tileWidth = mapFileXML.child("map").attribute("tilewidth").as_int();
		mapData.tileHeight = mapFileXML.child("map").attribute("tileheight").as_int();

		//Iterate the Tileset
		for (pugi::xml_node tilesetNode = mapFileXML.child("map").child("tileset"); tilesetNode != NULL; tilesetNode = tilesetNode.next_sibling("tileset"))
		{
			//Load Tileset attributes
			TileSet* tileSet = new TileSet();
			tileSet->firstGid = tilesetNode.attribute("firstgid").as_int();
			tileSet->name = tilesetNode.attribute("name").as_string();
			tileSet->tileWidth = tilesetNode.attribute("tilewidth").as_int();
			tileSet->tileHeight = tilesetNode.attribute("tileheight").as_int();
			tileSet->spacing = tilesetNode.attribute("spacing").as_int();
			tileSet->margin = tilesetNode.attribute("margin").as_int();
			tileSet->tileCount = tilesetNode.attribute("tilecount").as_int();
			tileSet->columns = tilesetNode.attribute("columns").as_int();

			//Load the tileset image
			std::string imgName = tilesetNode.child("image").attribute("source").as_string();
			tileSet->texture = Engine::GetInstance().textures->Load((mapPath + imgName).c_str());

			mapData.tilesets.push_back(tileSet);
		}

		// L07: TODO 3: Iterate all layers in the TMX and load each of them
		for (pugi::xml_node layerNode = mapFileXML.child("map").child("layer"); layerNode != NULL; layerNode = layerNode.next_sibling("layer")) {

			// L07: TODO 4: Implement the load of a single layer 
			//Load the attributes and saved in a new MapLayer
			MapLayer* mapLayer = new MapLayer();
			mapLayer->id = layerNode.attribute("id").as_int();
			mapLayer->name = layerNode.attribute("name").as_string();
			mapLayer->width = layerNode.attribute("width").as_int();
			mapLayer->height = layerNode.attribute("height").as_int();

			//L09: TODO 6 Call Load Layer Properties
			LoadProperties(layerNode, mapLayer->properties);

			//Iterate over all the tiles and assign the values in the data array
			for (pugi::xml_node tileNode = layerNode.child("data").child("tile"); tileNode != NULL; tileNode = tileNode.next_sibling("tile")) {
				mapLayer->tiles.push_back(tileNode.attribute("gid").as_int());
			}

			//add the layer to the map
			mapData.layers.push_back(mapLayer);
		}

		//Collisions
		for (pugi::xml_node tileNode = mapFileXML.child("map").child("objectgroup").child("object"); tileNode != NULL; tileNode = tileNode.next_sibling("object")) {
			int h;
			std::string p = tileNode.child("properties").child("property").attribute("value").as_string();
			if (p == "PLATFORM")
				h = 1;
			else
				h = tileNode.attribute("height").as_int();

			PhysBody* pb = Engine::GetInstance().physics.get()->CreateRectangle(tileNode.attribute("x").as_int() + (tileNode.attribute("width").as_int() / 2), tileNode.attribute("y").as_int() + (tileNode.attribute("height").as_int() / 2), tileNode.attribute("width").as_int(), h, STATIC);
			if (p == "WALL")
				pb->ctype = ColliderType::WALL;
			else if (p == "PLATFORM")
				pb->ctype = ColliderType::GROUND;
			else if (p == "DIE")
				pb->ctype = ColliderType::DIE;
			else if (p == "NEW")
				pb->ctype = ColliderType::NEW;
			else if (p == "LOAD")
				pb->ctype = ColliderType::LOAD;
			else if (p == "NEXTLVL")
				pb->ctype = ColliderType::NEXTLVL;
			else if (p == "WIN")
				pb->ctype = ColliderType::WIN;
			else
				pb->ctype = ColliderType::GROUND;

			collisions.push_back(pb);
		}


		for (const auto& mapLayer : mapData.layers) {
			//Check if the property Draw exist get the value, if it's true draw the lawyer
			for (int i = 0; i < mapData.width; i++) {
				for (int j = 0; j < mapData.height; j++) {

					//Get the gid from tile
					int gid = mapLayer->Get(i, j);
					//Check if the gid is different from 0 - some tiles are empty
					if (gid != 0) {

						if (gid == 1025) {
							Vector2D mapCoord = MapToWorld(i, j);
							PhysBody* c1 = Engine::GetInstance().physics.get()->CreateRectangle(mapCoord.getX() + 8, mapCoord.getY() + 8, 16, 16, STATIC);
							c1->ctype = ColliderType::GROUND;
						}
						else if (gid == 2) {
							Vector2D mapCoord = { (float)i * 8, (float)j * 8 };
							posBonfire.push_back(mapCoord);
						}
						else if (gid == 3) {
							Vector2D mapCoord = { (float)i * 8, (float)j * 8 };
							posPoison.push_back(mapCoord);
						}
						else if (gid == 4) { // Evil Wizard
							Vector2D mapCoord = { (float)i * 8, (float)j * 8 };
							posEnemy.insert({ mapCoord, 1 });
						}
						else if (gid == 5) { // Bat
							Vector2D mapCoord = { (float)i * 8, (float)j * 8 };
							posEnemy.insert({ mapCoord, 2 });
						}
						else if (gid == 6) { // Coin
							Vector2D mapCoord = { (float)i * 8, (float)j * 8 };
							posItems.insert({ mapCoord, 1 });
						}
						else if (gid == 7) { // Fire
							Vector2D mapCoord = { (float)i * 8, (float)j * 8 };
							posItems.insert({ mapCoord, 2 });
						}
						else if (gid == 8) { // Health
							Vector2D mapCoord = { (float)i * 8, (float)j * 8 };
							posItems.insert({ mapCoord, 3 });
						}
					}
				}
			}
		}

		ret = true;

		// LOG all the data loaded iterate all tilesetsand LOG everything
		if (ret == true)
		{
			LOG("Successfully parsed map XML file :%s", fileName.c_str());
			LOG("width : %d height : %d", mapData.width, mapData.height);
			LOG("tile_width : %d tile_height : %d", mapData.tileWidth, mapData.tileHeight);

			LOG("Tilesets----");

			//iterate the tilesets
			for (const auto& tileset : mapData.tilesets) {
				LOG("name : %s firstgid : %d", tileset->name.c_str(), tileset->firstGid);
				LOG("tile width : %d tile height : %d", tileset->tileWidth, tileset->tileHeight);
				LOG("spacing : %d margin : %d", tileset->spacing, tileset->margin);
			}

			LOG("Layers----");

			for (const auto& layer : mapData.layers) {
				LOG("id : %d name : %s", layer->id, layer->name.c_str());
				LOG("Layer width : %d Layer height : %d", layer->width, layer->height);
			}
		}
		else {
			LOG("Error while parsing map file: %s", mapPathName.c_str());
		}

		if (mapFileXML) mapFileXML.reset();

	}

	mapLoaded = ret;
	return ret;
}

// L07: TODO 8: Create a method that translates x,y coordinates from map positions to world positions
Vector2D Map::MapToWorld(int x, int y) const
{
	Vector2D ret;

	ret.setX(x * mapData.tileWidth);
	ret.setY(y * mapData.tileHeight);

	return ret;
}

Vector2D Map::WorldToMap(int x, int y) {

	Vector2D ret(0, 0);

	ret.setX(x / mapData.tileWidth);
	ret.setY(y / mapData.tileHeight);

	return ret;
}

// L09: TODO 6: Load a group of properties from a node and fill a list with it
bool Map::LoadProperties(pugi::xml_node& node, Properties& properties)
{
	bool ret = false;

	for (pugi::xml_node propertieNode = node.child("properties").child("property"); propertieNode; propertieNode = propertieNode.next_sibling("property"))
	{
		Properties::Property* p = new Properties::Property();
		p->name = propertieNode.attribute("name").as_string();
		p->value = propertieNode.attribute("value").as_bool(); // (!!) I'm assuming that all values are bool !!

		properties.propertyList.push_back(p);
	}

	return ret;
}

MapLayer* Map::GetNavigationLayer() {
	for (const auto& layer : mapData.layers) {
		if (layer->properties.GetProperty("Navigation") != NULL &&
			layer->properties.GetProperty("Navigation")->value) {
			return layer;
		}
	}

	return nullptr;
}

// L09: TODO 7: Implement a method to get the value of a custom property
Properties::Property* Properties::GetProperty(const char* name)
{
	for (const auto& property : propertyList) {
		if (property->name == name) {
			return property;
		}
	}

	return nullptr;
}

