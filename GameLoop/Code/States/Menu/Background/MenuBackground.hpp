#ifndef MENU_BACKGROUND_H
#define MENU_BACKGROUND_H

#include "Common.hpp"
#include "Tools/Physics/Motion.hpp"

#include <functional>

enum class MenuBackgroundLayersEnum
{
	SKY,
	CLOUD,
	CITY,
	WATER,
	GROUND,
	COUNT,
	NONE,
};

enum class MenuBackgroundShadersEnum
{
	WATER_WAVE,
	COUNT,
	NONE,
};

struct MenuBackgroundPart
{
	const sf::Texture* texture;
	sf::Sprite sprite;

	//List de ValueMotion et MotionAround
	std::vector<MotionAround> motionArounds;

	sf::Vector2f position;
};

class MenuBackground
{
public:
	MenuBackground() = default;
	void Load(sf::Vector2f _screenSize, std::function <const sf::Texture* (const std::string)> _getTexture);
	void Update(float _dt, sf::Vector2f _mousePosition);
	void Draw(sf::RenderTarget& _render);

#pragma region Values

	// Getters
	sf::Vector2f GetOrigin(bool _normalized);
	sf::Vector2f GetParralaxOffset(void) { return this->parralaxOffset; }
	// Setters
	void SetOrigin(sf::Vector2f _origin, bool _normalized);
public:
	sf::Vector2f position;
	sf::Vector2f scale;
	float rotation;
private:
	sf::Vector2f origin;
#pragma endregion
private:
	sf::RenderTexture renderTexture;
	sf::Sprite renderSprite;

	MenuBackgroundPart backgroundParts[(int)MenuBackgroundLayersEnum::COUNT];
	sf::Vector2f parralaxOffset = sf::Vector2f(0.f, 0.f);
	sf::Vector2f parralaxOffsetMultiplicator[(int)MenuBackgroundLayersEnum::COUNT] =
	{
		sf::Vector2f(0.f, 0.f),		//SKY
		sf::Vector2f(0.02f, 0.f/*0.01f*/),	//CLOUD
		sf::Vector2f(0.04f, 0.f/*0.02f*/),	//CITY
		sf::Vector2f(0.05f, 0.f/*0.03f*/),	//WATER
		sf::Vector2f(0.07f, 0.f/*0.04f*/),	//GROUND
	};

	sf::Shader shaders[(int)MenuBackgroundShadersEnum::COUNT];
	const sf::Texture* shaderTextures[(int)MenuBackgroundShadersEnum::COUNT];
	sf::Clock clock;

	std::vector<MotionAround> cameraMotionsAround;

};

#endif // !MENU_H