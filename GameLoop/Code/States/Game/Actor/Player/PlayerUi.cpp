#include "PlayerUi.hpp"
#include "Player.hpp"

PlayerUi::PlayerUi(GameData* _data, Player* _player) : AutoUi(_data)
{
	healthBar = new Sprite(data->assets->GetTexture("health_bar"));
}

PlayerUi::~PlayerUi(void)
{
}

void PlayerUi::Update(float _dt)
{
}

void PlayerUi::Draw(sf::RenderTarget* _render)
{
}
