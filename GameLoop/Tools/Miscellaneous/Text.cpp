#include "Text.hpp"

sf::Font fonts[(int)FontType::NB_OF_FONTS];

void LoadFonts(void)
{
	for (int i = 0; i < (int)FontType::NB_OF_FONTS; i++)
	{
		char path[60] = "";
		sprintf_s(path, sizeof(path), "Assets/Fonts/%d.ttf", i);

		fonts[i].loadFromFile(path);
	}
}

const sf::Font* GetFont(FontType _type)
{
	return &fonts[(int)_type];
}

#pragma region Text Class

Text::Text(FontType _fontType, sf::Vector2f _origin, sf::Color _color, unsigned int _size)
{
	Create(_fontType, _origin, _color, _size);
}

Text::Text(FontType _fontType, sf::Vector2f _origin, sf::Color _color, unsigned int _size, float _letterSpacing)
{
	text.setLetterSpacing(_letterSpacing);
	Create(_fontType, _origin, _color, _size);
}

void Text::Create(FontType _fontType, sf::Vector2f _origin, sf::Color _color, unsigned int _size)
{
	text.setFont(fonts[(int)_fontType]);
	text.setFillColor(_color);

	shadow.setFont(fonts[(int)_fontType]);
	shadow.setFillColor(shadowColor);

	origin = _origin;
}

void Text::SetString(std::string _string)
{
	text.setString(_string);
	shadow.setString(_string);

	sf::FloatRect rect = text.getLocalBounds();
	text.setOrigin(sf::Vector2f(rect.width * origin.x, rect.height * origin.y));
	shadow.setOrigin(sf::Vector2f(rect.width * origin.x, rect.height * origin.y));
}

void Text::SetColor(sf::Color _color)
{
	text.setFillColor(_color);
}

void Text::SetFontType(FontType _fontType)
{
	text.setFont(fonts[(int)_fontType]);
	shadow.setFont(fonts[(int)_fontType]);
}

void Text::CreateShadow(sf::Vector2f _offset, sf::Color _color)
{
	CreateShadow(_offset.x, _offset.y, _color);
}

void Text::CreateShadow(float _x, float _y, sf::Color _color)
{
	SetShadowOffset(_x, _y);
	SetShadowColor(_color);
}

void Text::EnableShadow(bool _enable)
{
	shadowEnable = _enable;
}

void Text::SetShadowOffset(sf::Vector2f _offset)
{
	SetShadowOffset(_offset.x, _offset.y);
	
}

void Text::SetShadowOffset(float _x, float _y)
{
	shadowEnable = true;
	shadowOffset = { _x, _y };
	shadow.setPosition(text.getPosition().x + shadowOffset.x, text.getPosition().y + shadowOffset.y);
}

void Text::SetShadowColor(sf::Color _color)
{
	shadowEnable = true;
	shadowColor = _color;
	shadow.setFillColor(_color);
}

void Text::CreateOutline(sf::Color _color, float _thickness)
{
	SetOutlineThickness(_thickness);
	SetOutlineColor(_color);
}

void Text::SetOutlineThickness(float _thickness)
{
	text.setOutlineThickness(_thickness);
	shadow.setOutlineThickness(_thickness);
}

void Text::SetOutlineColor(sf::Color _color)
{
	text.setOutlineColor(_color);
	shadow.setOutlineColor(shadowColor);
}

void Text::SetPosition(sf::Vector2f _pos)
{
	SetPosition(_pos.x, _pos.y);
}

void Text::SetPosition(float _x, float _y)
{
	text.setPosition(_x, _y);
	shadow.setPosition(_x + shadowOffset.x, _y + shadowOffset.y);
}

void Text::SetOrigin(Vec2 _origin)
{
	origin = sf::Vector2f(_origin.x, _origin.y);
	sf::FloatRect rect = text.getLocalBounds();

	text.setOrigin(sf::Vector2f(rect.width * _origin.x, rect.height * _origin.y));
	shadow.setOrigin(sf::Vector2f(rect.width * _origin.x, rect.height * _origin.y));
}

void Text::Draw(sf::RenderTarget& _render)
{
	if (shadowEnable)
	{
		_render.draw(shadow);
	}
	_render.draw(text);
}

#pragma endregion
