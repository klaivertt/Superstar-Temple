#include "Tools/Map/Layer.hpp"

#include "Tools/Debug/Logger.hpp"
#include "Tools/Miscellaneous/Text.hpp"

namespace Layer
{
	Layer::Layer()
	{
		m_visibility = true;
	}

	Layer::Layer(sf::Vector2f _coord, bool _visibility)
	{
		m_visibility = _visibility;
		m_bakeSprite.setPosition(_coord);
	}

	Layer::~Layer()
	{
	}

	void Layer::Position(sf::Vector2f _position)
	{
		m_bakeSprite.setPosition(_position);
	}

	sf::Vector2f Layer::Position()
	{
		return m_bakeSprite.getPosition();
	}

	void Layer::Draw(sf::RenderTarget& _target)
	{
		if(m_visibility)
		{
			_target.draw(m_bakeSprite);
		}
	}

	TileLayer::TileLayer()
	{
		m_tileSet = nullptr;
		m_grid = nullptr;
		m_gridLength = 0;
	}

	TileLayer::TileLayer(sf::Vector2u _size)
	{
		m_size = _size;
		m_gridLength = m_size.x * m_size.y;
		m_tileSet = nullptr;
		m_grid = new unsigned[m_gridLength];
	}

	TileLayer::~TileLayer()
	{
		if (m_grid != nullptr)
		{
			delete[] m_grid;
		}
		m_grid = nullptr;
	}

	void TileLayer::MakeGrid(TileSet* _tileSet, sf::Vector2u _size)
	{
		if (_tileSet == nullptr)
		{
			Logger::Error("Tilset == null");
			return;
		}
		m_tileSet = _tileSet;
		m_size = _size;
		m_gridLength = m_size.x * m_size.y;
		if (m_grid != nullptr)
		{
			delete[] m_grid;
		}
		m_grid = new unsigned[m_gridLength];
	}

	void TileLayer::AddTileSet(TileSet* _tileSet)
	{
		if (_tileSet == nullptr)
		{
			Logger::Error("Tilset == null");
			return;
		}
		m_tileSet = _tileSet;
	}

	void TileLayer::SetGridElem(sf::Vector2u _coord, unsigned _val)
	{
		if (_coord.x < m_size.x && _coord.y < m_size.y)
		{
			m_grid[CoordToId(_coord)] = _val;
		}
#ifdef _DEBUG
		else
		{
			Logger::Error("Invalid coordinates");
		}
#endif // !_DEBUG
	}
	void TileLayer::SetGridElem(unsigned _id, unsigned _val)
	{
		if (_id < m_gridLength)
		{
			m_grid[_id] = _val;
		}
#ifdef _DEBUG
		else
		{
			Logger::Error("Invalid coordinates");
		}
#endif // !_DEBUG
	}
	void TileLayer::Bake()
	{
		m_bakeRender.create(m_size.x * m_tileSet->cellSize.x, m_size.y * m_tileSet->cellSize.y);
		m_bakeRender.clear(sf::Color::Transparent);
		for (int i = 0; i < m_gridLength; i++)
		{
			int u = m_grid[i] - 1;
			if(u >= 0)
			{
				sf::Vector2u position = IdToCoord(i);
				m_tileSet->sp.setRotation(0);
				m_tileSet->sp.setScale(1, 1);
				position *= m_tileSet->cellSize;
				sf::IntRect rect;

				rect.left = IdToCoord(u, *m_tileSet).x;
				rect.top = IdToCoord(u, *m_tileSet).y;
				rect.left *= m_tileSet->cellSize.x;
				rect.top *= m_tileSet->cellSize.y;

				rect.width = m_tileSet->cellSize.x;
				rect.height = m_tileSet->cellSize.y;
				m_tileSet->sp.setTextureRect(rect);
				m_tileSet->sp.setPosition(sf::Vector2f(position));
				m_bakeRender.draw(m_tileSet->sp);
			}
		}
		m_bakeRender.display();
		m_bakeSprite.setTexture(m_bakeRender.getTexture());
	}

	unsigned TileLayer::CoordToId(sf::Vector2u _coord)
	{
		unsigned id = _coord.y * m_size.x;
		id += _coord.x;
		return id;
	}

	sf::Vector2u TileLayer::IdToCoord(unsigned _id)
	{
		sf::Vector2u coord;
		coord.y = _id / m_size.x;
		coord.x = _id % m_size.x;
		return coord;
	}

	sf::Vector2u TileLayer::IdToCoord(unsigned _id, TileSet& _tileSet)
	{
		sf::Vector2u coord;
		coord.y = _id / _tileSet.collumns;
		coord.x = _id % _tileSet.collumns;
		return coord;
	}

	Object::Object(std::string _name, sf::Vector2f _position, sf::Vector2f _size, bool _visibility, CollidType _type, b2WorldId _world)
	{
		m_name = _name;
		m_size = _size;
		m_position = m_position;
		m_visibility = _visibility;
	}

	Object::~Object()
	{
	}

	void Object::Draw(sf::RenderTarget& _target)
	{
		sf::Text name;
		name.setFont(*GetFont(FontType::NORMAL));
		name.setString(m_name);
		sf::FloatRect rect = name.getGlobalBounds();
		name.setOrigin(rect.width / 2.f, rect.height / 2.f);
		name.setPosition(m_position);
	}

	ObjectLayer::ObjectLayer(sf::Vector2u _size)
	{
		m_size = _size;
	}

	ObjectLayer::~ObjectLayer()
	{
		ClearObjects();
	}

	void ObjectLayer::AddObject(Object* _obj)
	{
		m_objects.push_back(_obj);
	}

	void ObjectLayer::RemoveObject(int _id)
	{
		if (_id < m_objects.size())
		{
			delete m_objects[_id];
			m_objects[_id] = m_objects.back();
			m_objects.pop_back();
		}
#ifdef _DEBUG
		else
		{
			Logger::Error("Id " + std::to_string(_id) + " not allowed in layer " + m_name);
		}
#endif
	}

	void ObjectLayer::ClearObjects()
	{
		for (int i = 0; i < m_objects.size(); i++)
		{
			delete m_objects[i];
		}
		m_objects.clear();
	}

	void ObjectLayer::Bake()
	{
		m_bakeRender.create(m_size.x , m_size.y);
		m_bakeRender.clear(sf::Color::Black);
		for (int i = 0; i < m_objects.size(); i++)
		{
			m_objects[i]->Draw(m_bakeRender);
		}
		m_bakeRender.display();
	}

}
