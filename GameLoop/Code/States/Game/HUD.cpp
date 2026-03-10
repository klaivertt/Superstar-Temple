#include "HUD.hpp"

#include "Tools/Miscellaneous/Text.hpp"

HUD::HUD(GameData* _data, float& _timer) : timer(_timer)
{
	timerText = new Text(FontType::BOLD, { 0.5f, 0.5f }, sf::Color::White, 30);
	timerText->CreateShadow({ 2.f, 2.f }, sf::Color::Black);

	timer = _timer;


	timerText->SetString("00 : 00");
	timerText->SetPosition({ 900, 50 });
}

void HUD::Update(float _dt)
{
	minutes = (int)timer / 60;
	seconds = (int)timer % 60;

	if (seconds < 10)
	{
		if (minutes < 10)
		{
			timerText->SetString("0" + std::to_string((int)minutes) + " : 0" + std::to_string((int)seconds));
		}
		else
		{
			timerText->SetString(std::to_string((int)minutes) + " : 0" + std::to_string((int)seconds));
		}
	}
	else
	{
		if (minutes < 10)
		{
			timerText->SetString("0" + std::to_string((int)minutes) + " : " + std::to_string((int)seconds));
		}
		else
		{
			timerText->SetString(std::to_string((int)minutes) + " : " + std::to_string((int)seconds));
		}
	}
}

void HUD::draw(sf::RenderTarget* _render)
{
	timerText->Draw(*_render);
}