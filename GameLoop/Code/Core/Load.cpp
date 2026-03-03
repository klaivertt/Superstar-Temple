#include "Load.hpp"

#include "../States/Game/Game.hpp"
#include "../States/Menu/Menu.hpp"
#include "../States/Gameover/Gameover.hpp"
#include "../States/LevelEditor/LevelEditor.hpp"

#include "../../Tools/SceneManager.hpp"
#include "../../Tools/Miscellaneous/Inputs.hpp"
#include "../../Tools/Miscellaneous/Text.hpp"
#include "../../Tools/Debug/DebugViewer.hpp"

void CreateInputs(GameData* _game);

void Load(GameData* _game)
{
	_game->debugViewer = new Debug::DebugViewer();

	LoadFonts();

	_game->manager->Create<Game>("Game");
	_game->manager->Create<Menu>("Menu");
	_game->manager->Create<Gameover>("Gameover");
	_game->manager->Create<LevelEditor>("LevelEditor");

	CreateInputs(_game);

	_game->manager->LoadScene("Game");
	//_game->manager->LoadScene("LevelEditor");

	Logger::Success("Everything loaded successfully !");
}

void CreateInputs(GameData* _game)
{
	Inputs* inputs = _game->inputs;

	// All axis inputs
	Input input;
	input.key = sf::Keyboard::Z;
	input.axis = true;
	input.axisValue = -1.f;
	inputs->CreateInput("WalkForward", input);
	input.key = sf::Keyboard::S;
	input.axis = true;
	input.axisValue = 1.f;
	inputs->CreateInput("WalkBackward", input);
	input.key = sf::Keyboard::Q;
	input.axis = true;
	input.axisValue = -1.f;
	inputs->CreateInput("WalkLeft", input);
	input.key = sf::Keyboard::D;
	input.axis = true;
	input.axisValue = 1.f;
	inputs->CreateInput("WalkRight", input);

	// All normals inputs
	input.key = sf::Keyboard::Space;
	input.axis = false;
	input.axisValue = 0.f;
	inputs->CreateInput("Jump", input);

	input.key = sf::Keyboard::G;
	input.axis = false;
	input.axisValue = 0.f;
	inputs->CreateInput("DEBUG", input);

	input.key = sf::Keyboard::E;
	input.axis = false;
	input.axisValue = 0.f;
	inputs->CreateInput("Interact", input);


	//editor mode
	input.key = sf::Keyboard::C;
	input.axis = false;
	input.axisValue = 0.f;
	inputs->CreateInput("ZoomUp", input);

	input.key = sf::Keyboard::W;
	input.axis = false;
	input.axisValue = 0.f;
	inputs->CreateInput("ZoomDown", input);

}
