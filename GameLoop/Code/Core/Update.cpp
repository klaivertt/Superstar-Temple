#include "Update.hpp"

#include "../../Tools/SceneManager.hpp"
#include "../../Tools/Miscellaneous/Inputs.hpp"
#include "../../Tools/Debug/DebugViewer.hpp"

void Update(GameData* _game, float _dt)
{
	_game->guiManager->Update(_dt);
	_game->inputs->UpdateInputs(_dt); // Update Inputs (KEEP FIRST)

	_game->debugViewer->Update(_dt);

	_game->manager->Update(_dt); // Update of the current scene

	//_game.events.clear(); // Keep last on loop
}
