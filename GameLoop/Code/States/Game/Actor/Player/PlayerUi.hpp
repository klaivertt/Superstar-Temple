#ifndef PLAYER_UI_HPP
#define PLAYER_UI_HPP

#include "Tools/UI/AutoUi.hpp"
#include "Tools/Miscellaneous/Sprite.hpp"
class Player;


class PlayerUi : public AutoUi
{
private:
	//UI elements
	Sprite* healthBar;
	Sprite* healthBarBackground;
	Player& player;
	sf::Color cLow;
	sf::Color cMid;
	sf::Color cHigh;
public:
	PlayerUi(GameData* _data, Player* _player, Vec2 _position = Vec2(20.f, 20.f));
	~PlayerUi(void);
	virtual std::string GetClassName(void) override { return "PlayerUi"; }
	virtual void Update(float _dt) override;
	virtual void Draw(sf::RenderTarget* _render) override;
};


#endif // !PLAYER_UI_HPP
