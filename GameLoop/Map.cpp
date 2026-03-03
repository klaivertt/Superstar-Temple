#include "Map.hpp"
#include "JSON/json.h"
#include "JSON/json-forwards.h"

Map::Map()
{
}

Map::Map(std::string _path)
{
}

Map::~Map()
{
}

void Map::LoadMap(std::string _path)
{
	Json::Value jFile;
	std::fstream file(_path + "Map.json", std::ios::in);
	if (file.is_open())
	{
		file >> jFile;
		if (!jFile.isNull())
		{
			m_mapSize.y = jFile["height"].asInt();
			m_mapSize.x = jFile["width"].asInt();
			m_tabSize = m_mapSize.y * m_mapSize.x;
			m_tab = new unsigned[m_mapSize.y * m_mapSize.x];

			int layersNum = jFile["layers"].size();
			for (int l = 0; l < layersNum; l++)
			{
				std::string type = jFile["layers"][l]["type"].asString();
				if (type == "tilelayer")
				{
					for (unsigned i = 0; i < m_tabSize; i++)
					{
						m_tab[i] = (unsigned)jFile["layers"][l]["data"][i].asInt();
					}
				}
			}
		}
#ifdef _DEBUG
		else
		{
			std::cout << "json empty!" << "\033[0;93m" << std::endl;
		}
#endif
	}
#ifdef _DEBUG
	else
	{
		std::cout << "file " << _path << "doesn't exist!" << "\033[0;93m" << std::endl;
	}
#endif
}

void Map::Update(float _dt)
{

}

void Map::Draw(sf::RenderTarget& _target)
{

}