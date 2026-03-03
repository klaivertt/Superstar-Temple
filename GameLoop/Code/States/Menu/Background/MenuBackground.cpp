#include "MenuBackground.hpp"
#include "Tools/Physics/Motion.hpp"
#include "Tools/AssetsManager.hpp"

void MenuBackground::Load(sf::Vector2f _screenSize, std::function <const sf::Texture* (const std::string)> _getTexture)
{
	#pragma region Textures Load
	const std::string file = "Assets/Sprites/Menu/Background/";
	const float ScreenMulticator = 1.07f;

	for (int i = 0; i < (int)MenuBackgroundLayersEnum::COUNT; i++)
	{
		MenuBackgroundPart& background = this->backgroundParts[i];
		switch ((MenuBackgroundLayersEnum)i)
		{
		case MenuBackgroundLayersEnum::SKY:
			background.texture = _getTexture(file + "sky.png");
			break;
		case MenuBackgroundLayersEnum::CLOUD:
			background.texture = _getTexture(file + "loopcloud.png");
			break;
		case MenuBackgroundLayersEnum::CITY:
			background.texture = _getTexture(file + "city.png");
			break;
		case MenuBackgroundLayersEnum::WATER:
			background.texture = _getTexture(file + "water.png");
			break;
		case MenuBackgroundLayersEnum::GROUND:
			background.texture = _getTexture(file + "ground.png");
			break;
		}
	}
#pragma endregion

	#pragma region Shaders Load
	for (int i = 0; i < (int)MenuBackgroundShadersEnum::COUNT; i++)
	{
		switch ((MenuBackgroundShadersEnum)i)
		{
		case MenuBackgroundShadersEnum::WATER_WAVE:
			this->shaders[i].loadFromFile("Assets/Shaders/Distortion.frag", sf::Shader::Fragment);
			this->shaderTextures[i] = nullptr;
			break;
		}
	}
#pragma endregion

	const sf::Vector2f renderTextureSize = (sf::Vector2f)this->backgroundParts[(int)MenuBackgroundLayersEnum::SKY].texture->getSize();

	#pragma region RenderTexture
	this->position = { _screenSize.x / 2.f, _screenSize.y / 2.f };
	this->origin = { renderTextureSize.x / 2.f, renderTextureSize.y / 2.f };
	this->rotation = 0.f;
	this->scale = { _screenSize.x / (float)renderTextureSize.x , _screenSize.y / (float)renderTextureSize.y };
	if (this->scale.x < this->scale.y)this->scale = { this->scale.y, this->scale.y };
	else this->scale = { this->scale.x, this->scale.x };

	this->renderTexture.create((unsigned int)renderTextureSize.x, (unsigned int)renderTextureSize.y);
	this->renderTexture.setSmooth(true);
	this->renderSprite.setOrigin(this->origin);
	this->renderSprite.setPosition(this->position);
	this->renderSprite.setScale(scale);
#pragma endregion

	#pragma region BackgroundParts setup
	for (int i = 0; i < (int)MenuBackgroundLayersEnum::COUNT; i++)
	{
		MenuBackgroundPart& background = this->backgroundParts[i];

		// Sprites
		float rotation = 0.f;
		sf::Vector2f origin = { 0.5f, 0.5f };
		sf::Vector2f scale = { ScreenMulticator, ScreenMulticator };
		sf::Vector2f position = { renderTextureSize.x / 2.f, renderTextureSize.y/2 };
		sf::IntRect rect = { 0, 0, (int)background.texture->getSize().x, (int)background.texture->getSize().y };

		switch ((MenuBackgroundLayersEnum)i)
		{
		case MenuBackgroundLayersEnum::CLOUD:
			rect.width *= 2;
			break;
		}

		background.sprite.setTexture(*backgroundParts[i].texture);
		background.sprite.setTextureRect(rect);
		background.sprite.setOrigin(origin.x * rect.width, origin.y * rect.height);
		background.sprite.setScale(scale);
		background.sprite.setPosition(position);


		background.position = position;

		//Motion load
		switch ((MenuBackgroundLayersEnum)i)
		{
		case MenuBackgroundLayersEnum::CLOUD:
		{
			sf::Vector2f centerPoint = background.position;
			sf::Vector2f amplitude = { 0.f, 50.f };
			float duration = 60.f;
			MotionType motionType = MotionType::LINEAR;
			bool loop = true;

			background.motionArounds.push_back(MotionAround(centerPoint, amplitude, duration, MotionAroundType::VERTICAL_WAVE, motionType, loop));
		}
		}
	}
#pragma endregion
}

void MenuBackground::Update(float _dt, sf::Vector2f _mousePosition)
{
	//Adapt position
	_mousePosition = _mousePosition - this->position;
	_mousePosition = sf::Vector2f(_mousePosition.x / this->scale.x, _mousePosition.y / this->scale.y);
	//_mousePosition = std::clamp(_mousePosition, sf::Vector2f(0,0), (sf::Vector2f)this->renderTexture.getSize());
	//rotation
	this->parralaxOffset = -_mousePosition;
	sf::Vector2f max = sf::Vector2f(this->renderTexture.getSize().x * this->scale.x, this->renderTexture.getSize().y * this->scale.y);
	sf::Vector2f min = -max;
	this->parralaxOffset.x = std::clamp(this->parralaxOffset.x, min.x, max.x);
	this->parralaxOffset.y = std::clamp(this->parralaxOffset.y, min.y, max.y);

	for (int i = 0; i < (int)MenuBackgroundLayersEnum::COUNT; i++)
	{
		MenuBackgroundPart& background = this->backgroundParts[i];

		switch ((MenuBackgroundLayersEnum)i)
		{
		case MenuBackgroundLayersEnum::SKY:
		{

		}
		break;
		case MenuBackgroundLayersEnum::CLOUD:
		{
			background.position.x -= 20.f * _dt;
			if (background.position.x <= -background.sprite.getGlobalBounds().width / 2.f)
			{
				background.position.x = 0.f;
			}

			background.motionArounds[0].SetCenterPoint({ background.position.x, background.motionArounds[0].GetCenterPoint().y });
			background.position.y = background.motionArounds[0].UpdateValue(_dt).y;
		}
		break;
		case MenuBackgroundLayersEnum::CITY:
		{

		}
		break;
		case MenuBackgroundLayersEnum::WATER:
		{

		}
		break;
		case MenuBackgroundLayersEnum::GROUND:
		{

		}
		break;
		}

		sf::Vector2f parralaxMovement = sf::Vector2f(this->parralaxOffset.x * this->parralaxOffsetMultiplicator[i].x,
													 this->parralaxOffset.y * this->parralaxOffsetMultiplicator[i].y);
		
		background.sprite.setPosition(background.position + parralaxMovement);
	}

	//render Sprite update
	this->renderSprite.setOrigin(this->origin);
	this->renderSprite.setPosition(this->position);
	this->renderSprite.setRotation(this->rotation);
	this->renderSprite.setScale(this->scale);
}

void MenuBackground::Draw(sf::RenderTarget& _render)
{
	this->renderTexture.clear(sf::Color::Transparent);
	for (int i = 0; i < (int)MenuBackgroundLayersEnum::COUNT; i++)
	{
		sf::Shader* shaderToUse = nullptr;
		switch (MenuBackgroundLayersEnum(i))
		{
		case MenuBackgroundLayersEnum::WATER:
			shaderToUse = &this->shaders[(int)MenuBackgroundShadersEnum::WATER_WAVE];
			shaderToUse->setUniform("timeElapsed", this->clock.getElapsedTime().asSeconds() * 0.25f);
			shaderToUse->setUniform("intensityX", 0.6f);
			shaderToUse->setUniform("intensityY", 0.6f);
			shaderToUse->setUniform("textureLoopX", true);
			shaderToUse->setUniform("textureLoopY", false);
			break;
		}

		if (shaderToUse)this->renderTexture.draw(backgroundParts[i].sprite, shaderToUse);
		else            this->renderTexture.draw(backgroundParts[i].sprite);

	}
	this->renderTexture.display();

	this->renderSprite.setTexture(this->renderTexture.getTexture());
	_render.draw(this->renderSprite);
}

sf::Vector2f MenuBackground::GetOrigin(bool _normalized)
{
	if (_normalized)
	{
		return { this->origin.x / this->renderTexture.getSize().x, this->origin.y / this->renderTexture.getSize().y };
	}
	else
	{
		return this->origin;
	}
}

void MenuBackground::SetOrigin(sf::Vector2f _origin, bool _normalized)
{
	if (_normalized)
	{
		this->origin = { _origin.x * this->renderTexture.getSize().x, _origin.y * this->renderTexture.getSize().y };
	}
	else
	{
		this->origin = _origin;
	}
}