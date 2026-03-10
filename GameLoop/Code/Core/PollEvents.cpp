#include "PollEvents.hpp"

#include "../../GameData.hpp"

#include "../States/Game/Game.hpp"

#include "../../Tools/Debug/DebugViewer.hpp"

void KeyPressed(GameData* _game, sf::Keyboard::Key _key);

void PollEvents(GameData* _game, sf::Event _event)
{
	_game->guiManager->ProcessEvent(_event);
	if (_event.type == sf::Event::Closed)
	{
		_game->render->close();
	}
	if (_event.type == sf::Event::KeyPressed)
	{
		KeyPressed(_game, _event.key.code);
	}

	_game->manager->PollEvents(_event);
}

void KeyPressed(GameData* _game, sf::Keyboard::Key _key)
{
	if (_key == sf::Keyboard::Escape)
	{
		_game->render->close();
	}

	if (_key == sf::Keyboard::K)
	{
		_game->debugViewer->ToggleFPS();
	}
}
