#ifndef PLAYER_UI_HPP
#define PLAYER_UI_HPP

#include "Tools/UI/AutoUi.hpp"
class Player;


class PlayerUi : public AutoUi
{
private:
	//UI elements
	Sprite healthBar;
	Sprite healthBarBackground;
public:
	PlayerUi(GameData* _data, Player* _player);
	~PlayerUi(void);
	virtual std::string GetClassName(void) override { return "PlayerUi"; }
	virtual void Update(float _dt) override;
	virtual void Draw(sf::RenderTarget* _render) override;
};


#endif // !PLAYER_UI_HPP
