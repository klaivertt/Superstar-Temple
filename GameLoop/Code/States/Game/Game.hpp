#ifndef GAME_H
#define GAME_H

#include "../../GameLoop/Tools/Scene.hpp"
#include "../../GameLoop/Tools/SceneManager.hpp"
#include "../../GameLoop/GameData.hpp"

#include "../../GameLoop/Tools/Miscellaneous/Inputs.hpp"
#include "Tools/Physics/Physics.hpp"

class HUD;
class Animation;
class Player;
class PlayerUi;
class Key;
class Box;
class Button;
class FireTrap;
class SpikeTrap;
class Door;

struct Game : public Scene
{
protected:

	Player* player = nullptr;
	Key* key = nullptr;
	Box* box = nullptr;
	Button* button = nullptr;
	Button* fireButton = nullptr;
	Button* spikeButton = nullptr;
	FireTrap* fireTrap = nullptr;
	SpikeTrap* spikeTrap = nullptr;
	PlayerUi* playerUi = nullptr;
	Door* door = nullptr;

	float timer = 0.f;
	HUD* hud = nullptr;

	//temp ground
	b2BodyId groundBody = b2BodyId();
	b2ShapeId groundShape = b2ShapeId();

private:
	int fpsLimit = 60;
	bool vSync = true;

public:
	Game(GameData* _data);

	void Load(void) override;
	void Update(float _dt) override;
	void Draw(sf::RenderTarget* _render) override;
	void Destroy(void) override;

	virtual void OnPressedDebugKey(Input _input);

	void ResetScene(void);
};

#endif // !GAME_H