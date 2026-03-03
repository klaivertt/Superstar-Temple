#include "BasicCube.hpp"

Cube::Cube(GameData* _data) : Actor(_data)
{
	position = Random(Vec2(0.f, 0.f), Vec2(SCREEN_W, SCREEN_H));

	shape.setSize(sf::Vector2f(50.f, 50.f));
	shape.setPosition(position.ToSFML());
	shape.setFillColor(sf::Color::White);

	lifeTime = Random(2.f, 5.f);
}

Cube::Cube(GameData* _data, std::string _name) : Actor(_data, _name)
{
	position = Random(Vec2(0.f, 0.f), Vec2(SCREEN_W, SCREEN_H));

	shape.setSize(sf::Vector2f(50.f, 50.f));
	shape.setPosition(position.ToSFML());
	shape.setFillColor(sf::Color::White);

	lifeTime = Random(2.f, 5.f);
}

void Cube::Update(float _dt)
{
	lifeTime -= _dt;
	if (lifeTime <= 0.f)
	{
		delete this;
	}
}

void Cube::Draw(sf::RenderTarget* _render)
{
	_render->draw(shape);
}
