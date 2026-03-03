#ifndef LAYER__H
#define LAYER__H

#include "Common.hpp"

namespace Layer
{
	enum CollidType
	{
		None,
		Collision,
		Trigger,
	};

	class Object
	{
	public:
		Object(std::string _name, sf::Vector2f _position, sf::Vector2f _size, bool _visibility, CollidType _type, b2WorldId _world);
		~Object();
	private:
		std::string m_name;
		sf::Vector2f m_size;
		sf::Vector2f m_position;
		bool m_visibility;
	};

	class Layer
	{
	public:
		Layer();
		Layer(sf::Vector2f _coord, bool _visibility);
		~Layer();
		sf::Vector2f m_position;
		bool m_visibility;
		virtual void Bake();
		virtual void Draw(sf::RenderTarget& _target);
	protected:
		sf::RenderTexture m_bakeRender;
		sf::Sprite m_bakeSprite;
	};

	class TileLayer : public Layer
	{
	public:
		TileLayer();
		TileLayer(sf::Vector2u _size);
		~TileLayer();
	
		sf::Sprite* m_tileSprite;
	
		sf::Vector2u m_tileSize;
		void MakeGrid(sf::Vector2u _size);
		void SetGridElem(sf::Vector2u _coord, unsigned _val);
		virtual void Bake() override;
		virtual void Draw(sf::RenderTarget& _target) override;
	private:
		unsigned CoordToId(sf::Vector2u _coord);
		sf::Vector2u IdToCoord(unsigned _id);
		unsigned* m_grid;
		unsigned m_gridLength;
		sf::Vector2u m_size;
	};

	class ObjectLayer : public Layer
	{
	public:
		ObjectLayer();
		~ObjectLayer();
		std::vector<Object*> m_objects;
		virtual void Bake() override;
		virtual void Draw(sf::RenderTarget& _target) override;

	};
}

#endif // !LAYER__H
