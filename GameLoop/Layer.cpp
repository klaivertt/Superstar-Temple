#include "Layer.hpp"

namespace Layer
{
	Layer::Layer()
	{
		m_visibility = true;
		m_position = sf::Vector2f();
	}

	Layer::Layer(sf::Vector2f _coord, bool _visibility)
	{
		m_visibility = _visibility;
		m_position = _coord;
	}

	Layer::~Layer()
	{
	}

	void Layer::Draw(sf::RenderTarget& _target)
	{
		_target.draw(m_bakeSprite);
	}

	TileLayer::TileLayer()
	{
		m_tileSprite = nullptr;
		m_grid = nullptr;
		m_gridLength = 0;
	}

	TileLayer::TileLayer(sf::Vector2u _size)
	{
		m_size = _size;
		m_gridLength = m_size.x * m_size.y;
		m_tileSprite = nullptr;
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

	void TileLayer::MakeGrid(sf::Vector2u _size)
	{
		m_size = _size;
		m_gridLength = m_size.x * m_size.y;
		if (m_grid != nullptr)
		{
			delete[] m_grid;
		}
		m_grid = new unsigned[m_gridLength];
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
			std::cout << "[ERROR]Invalid coordinates" << "\033[0;93m" << std::endl;
		}
#endif // !_DEBUG
	}
	void TileLayer::Bake()
	{
		m_bakeRender.create(m_size.x * m_tileSize.x, m_size.y * m_tileSize.y);
		m_bakeRender.clear(sf::Color::Black);
		for (int i = 0; i < m_gridLength; i++)
		{
			m_grid[i];
			sf::Vector2u position = IdToCoord(i);
			m_tileSprite->setRotation(0);
			m_tileSprite->setScale(1,1);
			position *= m_tileSize;
			sf::IntRect rect;
			rect.left = position.x;
			rect.top = position.y;
			rect.width = m_tileSize.x;
			rect.height = m_tileSize.y;
			m_tileSprite->setTextureRect(rect);
			m_tileSprite->setPosition(sf::Vector2f(position));
			m_bakeRender.draw(*m_tileSprite);
		}
		m_bakeRender.display();
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

	ObjectLayer::ObjectLayer()
	{
	}

	ObjectLayer::~ObjectLayer()
	{
	}

}
