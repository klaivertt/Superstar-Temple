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

	struct TileSet
	{
		sf::Sprite sp;
		sf::Vector2u size;
		sf::Vector2u cellSize;
		unsigned collumns;
	};

	class Object
	{
	public:
		Object(std::string _name, sf::Vector2f _position, sf::Vector2f _size, bool _visibility, CollidType _type, b2WorldId* _world);
		~Object();
		void Draw(sf::RenderTarget& _target);
	private:
		std::string m_name;
		sf::Vector2f m_size;
		sf::Vector2f m_position;
		b2BodyId m_body;
		bool m_visibility;
	};

	class Layer
	{
	public:
		Layer();
		Layer(sf::Vector2f _coord, bool _visibility);
		~Layer();
		void Position(sf::Vector2f _position);
		sf::Vector2f Position();
		bool m_visibility;
		virtual void Bake() {};
		virtual void Draw(sf::RenderTarget& _target);
		std::string m_name;
	protected:
		sf::RenderTexture m_bakeRender;
		sf::Sprite m_bakeSprite;
		sf::Vector2u m_size;
	};

	class TileLayer : public Layer
	{
	public:
		TileLayer();
		TileLayer(sf::Vector2u _size);
		~TileLayer();
	
		void MakeGrid(TileSet* _tileSet, sf::Vector2u _size);
		void AddTileSet(TileSet* _tileSet);
	
		void SetGridElem(sf::Vector2u _coord, unsigned _val);
		void SetGridElem(unsigned _id, unsigned _val);
		virtual void Bake() override;
	private:
		unsigned CoordToId(sf::Vector2u _coord);
		sf::Vector2u IdToCoord(unsigned _id);
		sf::Vector2u IdToCoord(unsigned _id, TileSet& _tileSet);
		sf::Vector2u m_size;
		unsigned* m_grid;
		unsigned m_gridLength;
		TileSet* m_tileSet;
	};

	class ObjectLayer : public Layer
	{
	public:
		ObjectLayer(sf::Vector2u _size);
		~ObjectLayer();
		void AddObject(Object* _obj);
		void RemoveObject(int _id);
		void ClearObjects();
		virtual void Bake() override;
	private:
		std::vector<Object*> m_objects;
	};
}

#endif // !LAYER__H
