#include "Sprite.hpp"

Sprite::Sprite(sf::Texture* _texture, Vec2 _origin)
{
	texture = _texture;
	origin = _origin;

	sprite.setTexture(*texture, true);
}

Sprite::Sprite(std::string _path, Vec2 _origin)
{
	GameData* data = GameData::GetInstance();

	texture = data->assets->GetTexture(_path);
	origin = _origin;
}

Sprite::Sprite(void)
{
	data = GameData::GetInstance();
}

void Sprite::SetPosition(Vec2 _position)
{
	position = _position;
	sprite.setPosition(sf::Vector2f(position.x, position.y));
}

void Sprite::SetOrigin(Vec2 _origin)
{
	origin = _origin;

	sf::FloatRect rect = sprite.getLocalBounds();
	sprite.setOrigin(sf::Vector2f(rect.width * origin.x, rect.height * origin.y));
}

void Sprite::SetScale(Vec2 _scale)
{
	scale = _scale;
	sprite.setScale(sf::Vector2f(scale.x, scale.y));
}

void Sprite::SetRotation(float _rotation)
{
	rotation = _rotation;
	sprite.setRotation(rotation);
}

void Sprite::SetTexture(sf::Texture* _texture)
{
	texture = _texture;
	sprite.setTexture(*texture, true);
	SetOrigin(origin);
}

void Sprite::SetTexture(std::string _path)
{
	GameData* data = GameData::GetInstance();
	texture = data->assets->GetTexture(_path);
	sprite.setTexture(*texture, true);
	SetOrigin(origin);
}

Vec2 Sprite::GetPosition(void)
{
	return position;
}

Vec2 Sprite::GetOrigin(void)
{
	return origin;
}

Vec2 Sprite::GetScale(void)
{
	return scale;
}

float Sprite::GetRotation(void)
{
	return rotation;
}

sf::Texture* Sprite::GetTexture(void)
{
	return texture;
}

sf::FloatRect Sprite::GetGlobalBounds(void)
{
	return sprite.getGlobalBounds();
}

sf::FloatRect Sprite::GetLocalBounds(void)
{
	return sprite.getLocalBounds();
}

sf::IntRect Sprite::GetTextureRect(void)
{
	return sprite.getTextureRect();
}

void Sprite::SetTextureRect(sf::IntRect _rect)
{
	sprite.setTextureRect(_rect);
	SetOrigin(origin);
}

sf::Color Sprite::GetColor(void)
{
	return sprite.getColor();
}

void Sprite::SetColor(sf::Color _color)
{
	sprite.setColor(_color);
}

void Sprite::SetShader(std::string _path)
{
	if (shader.loadFromFile(_path, sf::Shader::Fragment))
	{
	renderStates.shader = &shader;
	renderStates.transform = sf::Transform::Identity;
	renderStates.texture = NULL;

	isUsingShader = true;
	}
	else
	{
		Logger::Error("Failed to Load shader at " + _path);
	}
}

void Sprite::SetUseShader(bool _use)
{
	isUsingShader = _use;
}

void Sprite::Draw(sf::RenderTarget* _render)
{
	if (isUsingShader)
	{
		_render->draw(sprite, renderStates);
	}
	else
	{
		_render->draw(sprite);
	}
}
