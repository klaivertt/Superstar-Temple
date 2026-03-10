#include "HUD.hpp"

#include "Tools/Miscellaneous/Text.hpp"

HUD::HUD(GameData* _data, float& _timer) : timer(_timer)
{
	timerText = new Text(FontType::BOLD, { 0.5f, 0.5f }, sf::Color::White, 30);
	timerText->CreateShadow({ 2.f, 2.f }, sf::Color::Black);

	timer = _timer;
	timerText->SetString(std::to_string((int)timer / 60) + " : " + std::to_string((int)timer % 60));
	timerText->SetPosition({ 900, 50 });
}

void HUD::Update(float _dt)
{
	timerText->SetString(std::to_string((int)timer / 60) + " : " + std::to_string((int)timer % 60));
}

void HUD::draw(sf::RenderTarget* _render)
{
	timerText->Draw(*_render);
}