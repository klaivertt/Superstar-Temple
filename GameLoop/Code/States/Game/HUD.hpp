#pragma once

#include "GameData.hpp"

#include "Tools/Miscellaneous/Sprite.hpp"

class Text;

class HUD
{
public:
	HUD(GameData* _data, float& _timer);
	~HUD();
	void Update(float _dt);
	void draw(sf::RenderTarget* _render);

	Text* timerText;
	float& timer;

	Sprite sprite;
};