#pragma once

#include "GameData.hpp"

struct Game;

class DuelTrigger : public Actor
{
private:
	Game* game = nullptr;
	bool isTriggered = false;

public:
	DuelTrigger(GameData* _data, Vec2 _pos, Vec2 _size, Game* _game);

	virtual std::string GetClassName(void) override { return "DuelTrigger"; }
	virtual void OnTriggerEnter(ColEvent _col) override;
};