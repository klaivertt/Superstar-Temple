#ifndef TEXT_H
#define TEXT_H

#include "../../GameData.hpp"

typedef enum class FontType
{
	BOLD,
	LIGHT,
	MEDIUM,
	NORMAL,
	ITALIC,

	NB_OF_FONTS // Keep Last
}FontType;

void LoadFonts(void);
const sf::Font* GetFont(FontType _type);

typedef class Text
{
public:
	Text(FontType _fontType, sf::Vector2f _origin, sf::Color _color, unsigned int _size);
	Text(FontType _fontType, sf::Vector2f _origin, sf::Color _color, unsigned int _size, float _letterSpacing);
private:
	// Text base
	sf::Text text;
	sf::Vector2f origin = { 0.f,0.f };

	// Shadow base
	sf::Vector2f shadowOffset = { 0.f,0.f };
	sf::Text shadow;
	sf::Color shadowColor = sf::Color::Black;
	bool shadowEnable = false;

	void Create(FontType _fontType, sf::Vector2f _origin, sf::Color _color, unsigned int _size);
public:
	void SetString(std::string _string);
	void SetColor(sf::Color _color);
	void SetFontType(FontType _fontType);

public:
	// Set and enable the shadow of your text (Ignore if you don't want any)
	void CreateShadow(sf::Vector2f _offset, sf::Color _color);
	// Set and enable the shadow of your text (Ignore if you don't want any)
	void CreateShadow(float _x, float _y, sf::Color _color);
private:
	void EnableShadow(bool _enable);
	void SetShadowOffset(sf::Vector2f _offset);
	void SetShadowOffset(float _x, float _y);
	void SetShadowColor(sf::Color _color);

public:
	// Set the outline of your text (Ignore if you don't want any)
	void CreateOutline(sf::Color _color, float _thickness);
private:
	void SetOutlineThickness(float _thickness);
	void SetOutlineColor(sf::Color _color);

public:
	// Set position of Text
	void SetPosition(sf::Vector2f _pos);
	// Set position of Text
	void SetPosition(float _x, float _y);
	// The origin is a percentage of the text size, for example (0.5f, 0.5f) will set the origin to the center of the text, (0, 0) will set the origin to the top left corner and (1, 1) will set the origin to the bottom right corner.
	void SetOrigin(Vec2 _origin);

	void Draw(sf::RenderTarget& _render);
}Text;

#endif // !TEXT_H
