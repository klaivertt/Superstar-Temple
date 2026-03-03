#include "Key.hpp"

Key::Key(GameData* _data) : Interactable(_data)
{
	texture.loadFromFile("Assets/Sprites/Game/Interactable/Key.png");
	sprite.setTexture(texture);
	pos = sf::Vector2f(500, 800);
	sprite.setPosition(pos);
}

void Key::Update(float _dt)
{

}

void Key::Draw(sf::RenderTarget* _render)
{
	_render->draw(sprite);
}

void Key::OnCollisionEnter(ColEvent _col)
{

}

void Key::OnCollisionExit(ColEvent _col)
{

}
