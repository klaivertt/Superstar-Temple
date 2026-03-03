#ifndef MAP__H
#define MAP__H

#include "Common.hpp"

//Map creator Masked-Bxy

class Map
{
public:
	Map();
	Map(std::string _path);
	~Map();
	void LoadMap(std::string _path);
	void Update(float _dt);
	void Draw(sf::RenderTarget& _target);
	b2WorldId m_world;
private:
	unsigned* m_tab;
	std::vector<sf::Texture> m_tileTexture;
	sf::Sprite m_sprite;
	sf::Vector2i m_mapSize;
	unsigned m_tabSize;
};

#endif