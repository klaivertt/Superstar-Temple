#ifndef MAP__H
#define MAP__H

#include "Common.hpp"
namespace Layer
{
	class TileSet;
	class Layer;
}
//Map creator Masked-Bxy
//to use this module make file with inside map named "Map.json" and the texture
//_path = path of folder map;

class Map
{
public:
	Map();
	Map(std::string _path, b2WorldId* _world);
	~Map();
	void LoadMap(std::string _path, b2WorldId* _world);
	void Update(float _dt);
	void Draw(sf::RenderTarget& _target);
	void DrawLayer(sf::RenderTarget& _target, unsigned LayerID);
	unsigned GetNumOfLayers();
	std::vector<sf::Vector2f> m_playerSpawn;
private:
	std::vector<Layer::TileSet*> m_tileSets;
	std::vector<Layer::Layer*> m_Layers;
	sf::Vector2u m_mapSize;
	b2WorldId* m_world;
};

#endif