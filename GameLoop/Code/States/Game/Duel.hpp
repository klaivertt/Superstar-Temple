#pragma once

#include "GameData.hpp"

#include "Tools/Miscellaneous/Inputs.hpp"
#include "Tools/Miscellaneous/Sprite.hpp"

struct PlayerData
{
	sf::Sprite spriteBullet;
	bool isShooting = false;
};

class Duel : Actor
{
protected:
	sf::Sprite spriteP1;
	sf::Sprite spriteP2;
	sf::RectangleShape rect[2];

	PlayerData player[2];
	float timer = 0.f;
	bool isDuelStarted = false;

public:
	Duel(GameData* _data);
	~Duel(void);

	virtual void Update(float _dt) override;
	virtual void Draw(sf::RenderTarget* _render) override;

	// Note: to be notified when this interactable is destroyed, use
	// the inherited  onDestroyed  delegate (defined on Actor).

	// Uncomment the function if you want to use them
	// virtual void OnCollisionHit(ColEvent _col) override;
	// virtual void OnTriggerEnter(ColEvent _col) override;
	// virtual void OnTriggerExit(ColEvent _col) override;
};