#include "PlayerUi.hpp"
#include "Player.hpp"

PlayerUi::PlayerUi(GameData* _data, Player* _player, Vec2 _position) : AutoUi(_data), player(*_player)
{
	healthBar = new Sprite(data->assets->GetTexture("Assets/Sprites/Game/HpBar.png"));
	healthBarBackground = new Sprite(data->assets->GetTexture("Assets/Sprites/Game/HpContainer.png"));

	healthBarBackground->SetPosition(_position);
	healthBar->SetPosition(_position);

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

	sf::IntRect rect = healthBar->GetTextureRect();
	if (player.GetHealth() <= 0.f)
	{
		rect.width = 0.f;
	}
	else
	{
		rect.width = (player.GetHealth() / 100.f) * healthBar->GetTexture()->getSize().x;
	}
	healthBar->SetTextureRect(rect);
	
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
