#include "PlayerUi.hpp"
#include "Player.hpp"

PlayerUi::PlayerUi(GameData* _data, Player* _player) : AutoUi(_data), player(*_player)
{
	healthBar = new Sprite(data->assets->GetTexture("Assets/Sprites/Game/HpBar.png"));
	healthBarBackground = new Sprite(data->assets->GetTexture("Assets/Sprites/Game/HpContainer.png"));

	healthBarBackground->SetPosition({ 20.f, 20.f });
	healthBar->SetPosition({ 20.f, 20.f });

	cLow = sf::Color(200, 30, 30);
	cMid = sf::Color(255, 200, 40);
	cHigh = sf::Color(40, 220, 80);
}

PlayerUi::~PlayerUi(void)
{
	delete healthBar;
	delete healthBarBackground;
}

void PlayerUi::Update(float _dt)
{

	healthBar->SetScale(Vec2(player.GetHealth() / 100.f, 1.f));

	//lerp color from green to red
	if (player.GetHealth() > 50.f)
	{
		healthBar->SetColor(LerpColor(cMid, cHigh, (player.GetHealth() - 50.f) / 50.f));
	}
	else
	{
		healthBar->SetColor(LerpColor(cLow, cMid, player.GetHealth() / 50.f));
	}
}

void PlayerUi::Draw(sf::RenderTarget* _render)
{
	healthBarBackground->Draw(_render);
	healthBar->Draw(_render);
}
