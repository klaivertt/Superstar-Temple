#pragma once

#include "GameData.hpp"

#include "Tools/Miscellaneous/Inputs.hpp"
#include "Tools/Miscellaneous/Sprite.hpp"
#include "../../../Tools/Miscellaneous/Text.hpp"

struct PlayerData
{
	Sprite spriteBullet;
	bool isShooting = false;
	float timerShoot = 0.f;
};

class Duel : Actor
{
protected:
	Sprite spriteP1;
	Sprite spriteP2;
	sf::RectangleShape rect[2];

	PlayerData player[2];
	Text* text;

	int winner = -1;
	float timer = 0.f;
	float timerTransition = 2.f;

	bool isFinished = false;
	bool isWinScreen = false;

public:
	Duel(GameData* _data);
	~Duel(void);

	virtual void Update(float _dt) override;
	virtual void Draw(sf::RenderTarget* _render) override;

	void StartDuel(float _dt);
	void EndDuel(float _dt, int _player);

	// Note: to be notified when this interactable is destroyed, use
	// the inherited  onDestroyed  delegate (defined on Actor).

	// Uncomment the function if you want to use them
	// virtual void OnCollisionHit(ColEvent _col) override;
	// virtual void OnTriggerEnter(ColEvent _col) override;
	// virtual void OnTriggerExit(ColEvent _col) override;
};