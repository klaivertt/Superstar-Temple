#include "Tools/Map/Map.hpp"

#include "JSON/json.h"
#include "JSON/json-forwards.h"
#include "Tools/Debug/Logger.hpp"
#include "Tools/Map/Layer.hpp"

Map::Map()
{
}

Map::Map(std::string _path)
{
	LoadMap(_path);
}

Map::~Map()
{
}

void Map::LoadMap(std::string _path)
{
	Json::Value jFile;
	std::fstream file(_path + "/Map.json", std::ios::in);
	if (file.is_open())
	{
		file >> jFile;
		if (!jFile.isNull())
		{
			m_mapSize.y = jFile["height"].asInt();
			m_mapSize.x = jFile["width"].asInt();
			Logger::Debug(Logger::Vec2(m_mapSize, "Map size : "), false);
			int numTileSet = jFile["tilesets"].size();
			for (int l = 0; l < numTileSet; l++)
			{
				Layer::TileSet* tempSet= new Layer::TileSet;
				tempSet->cellSize.x = jFile["tilesets"][l]["tilewidth"].asInt();
				tempSet->cellSize.y = jFile["tilesets"][l]["tileheight"].asInt();
				Logger::Debug(Logger::Vec2(tempSet->cellSize, "Map size : "), false);
				tempSet->size.x = jFile["tilesets"][l]["imagewidth"].asInt();
				tempSet->size.y = jFile["tilesets"][l]["imageheight"].asInt();
				tempSet->collumns = jFile["tilesets"][l]["columns"].asInt();
				sf::Texture* tempTex = new sf::Texture;
				tempTex->loadFromFile(_path+"/" + jFile["tilesets"][l]["image"].asString());
				tempSet->sp.setTexture(*tempTex, true);
				m_tileSets.push_back(tempSet);
			}
			int layersNum = jFile["layers"].size();
			for (int l = 0; l < layersNum; l++)
			{
				std::string type = jFile["layers"][l]["type"].asString();
				if (type == "tilelayer")
				{
					int w = jFile["layers"][l]["width"].asInt();
					int h = jFile["layers"][l]["height"].asInt();
					Layer::TileLayer* temp = new Layer::TileLayer(sf::Vector2u(w, h));
					temp->m_name = jFile["layers"][l]["name"].asString();
					int x = jFile["layers"][l]["x"].asInt();
					int y = jFile["layers"][l]["y"].asInt();
					temp->Position({ float (x),float (y) });
					temp->m_visibility = jFile["layers"][l]["visible"].asBool();
					int readLength = jFile["layers"][l]["data"].size();
					int tileSetID = jFile["layers"][l]["id"].asInt();
					temp->AddTileSet(m_tileSets[tileSetID-1]);
					for (int i = 0; i < readLength; i++)
					{
						temp->SetGridElem(i, jFile["layers"][l]["data"][i].asUInt());

					}
					temp->Bake();
					m_Layers.push_back(dynamic_cast<Layer::Layer*>(temp));
				}
				else if (type == "objectgroup")
				{
					Layer::ObjectLayer* temp = new Layer::ObjectLayer(m_mapSize);
					temp->m_name = jFile["layers"][l]["name"].asString();
					int x = jFile["layers"][l]["x"].asInt();
					int y = jFile["layers"][l]["y"].asInt();
					temp->Position({ float(x),float(y) });
					temp->m_visibility = jFile["layers"][l]["visible"].asBool();
					int readLength = jFile["layers"][l]["objects"].size();
					for (int i = 0; i < readLength; i++)
					{
						std::string oName = jFile["layers"][l]["objects"][i]["name"].asString();
						sf::Vector2f oPos;
						sf::Vector2f oSize;
						oPos.x = jFile["layers"][l]["objects"][i]["x"].asFloat();
						oPos.y = jFile["layers"][l]["objects"][i]["y"].asFloat();
						oSize.x = jFile["layers"][l]["objects"][i]["width"].asFloat();
						oSize.y = jFile["layers"][l]["objects"][i]["height"].asFloat();
						bool oVisible = jFile["layers"][l]["objects"][i]["visible"].asBool();
						Layer::CollidType type = Layer::CollidType::None;

						Layer::Object* tempObj = new Layer::Object(oName,oPos,oSize,oVisible, type, m_world);
						temp->AddObject(tempObj);

					}
					temp->Bake();
					m_Layers.push_back(dynamic_cast<Layer::Layer*>(temp));
				}
			}
		}
#ifdef _DEBUG
		else
		{
			Logger::Error("json empty!");
		}
#endif
	}
#ifdef _DEBUG
	else
	{
		Logger::Error("file " + _path + "/Map.json" + " doesn't exist!");
	}
#endif
}

void Map::Update(float _dt)
{

}

void Map::Draw(sf::RenderTarget& _target)
{
	for (int i = 0; i < m_Layers.size(); i++)
	{
		m_Layers[i]->Draw(_target);
	}
}

void Map::DrawLayer(sf::RenderTarget& _target, unsigned LayerID)
{
	if(LayerID< m_Layers.size())
	{
		m_Layers[LayerID]->Draw(_target);
	}
#ifdef _DEBUG
	else
	{
		Logger::Error("Layer "+ std::to_string(LayerID) + " not exist");
	}
#endif
}

unsigned Map::GetNumOfLayers()
{
	return m_Layers.size();
}
