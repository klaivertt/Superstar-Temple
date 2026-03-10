#ifndef GAME_H
#define GAME_H

#include "../../GameLoop/Tools/Scene.hpp"
#include "../../GameLoop/Tools/SceneManager.hpp"
#include "../../GameLoop/GameData.hpp"

#include "../../GameLoop/Tools/Miscellaneous/Inputs.hpp"
#include "Tools/Physics/Physics.hpp"

class Animation;
class Player;
class PlayerUi;
class Key;
class Box;
class Button;
class FireTrap;
class SpikeTrap;
class Door;
class Map;
class Duel;
class DuelTrigger;


struct Game : public Scene
{
protected:

	Player* player = nullptr;
	Player* player2 = nullptr;
	Key* key = nullptr;
	PlayerUi* playerUi = nullptr;
	PlayerUi* player2Ui = nullptr;
	std::map<int, Door*> doorsById;
	Map* mappy = nullptr;
	sf::View playerOneView;
	sf::View playerTwoView;
	sf::RectangleShape separator;
	DuelTrigger* duelTrigger = nullptr;
	//temp ground
	b2BodyId groundBody = b2BodyId();
	b2ShapeId groundShape = b2ShapeId();
	Duel* duel = nullptr;

private:
	int fpsLimit = 60;
	bool vSync = true;

public:
	Game(GameData* _data);

	void Load(void) override;
	void Update(float _dt) override;
	void Draw(sf::RenderTarget* _render) override;
	void Destroy(void) override;
	void StartDuel(void);

	virtual void OnPressedDebugKey(Input _input);

	void ResetScene(void);
};

#endif // !GAME_H