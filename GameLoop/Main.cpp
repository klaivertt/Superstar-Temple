#include "GameData.hpp"

#include "Code/Core/Load.hpp"
#include "Code/Core/Update.hpp"
#include "Code/Core/PollEvents.hpp"
#include "Code/Core/Draw.hpp"

#include "Tools/SceneManager.hpp"
#include "Tools/Miscellaneous/Inputs.hpp"

//#include <Windows.h>

void LoadWindow(sf::RenderWindow* _render);

#pragma region Main

int main(void)
{
	srand((unsigned int)time(NULL));

	GameData* data = GameData::GetInstance();

	Logger::SetLogLevel(LogLevel::DEBUG);
	data->manager = new SceneManager(data);
	data->render = new sf::RenderWindow();
	data->renderTexture = new sf::RenderTexture();
	data->inputs = new Inputs(data);

	if (FULLSCREEN)
	{
		data->render->create(sf::VideoMode(SCREEN_W, SCREEN_H), GAME_NAME, sf::Style::None);
	}
	else
	{
		data->render->create(sf::VideoMode(SCREEN_W, SCREEN_H), GAME_NAME, sf::Style::Close);
	}

	LoadWindow(data->render);
	data->guiManager = new Debug::GuiManager(data->render);
	Load(data);

	while (data->render->isOpen())
	{
		data->dt = data->clock.restart().asSeconds();

		if (data->dt <= 0.f)
		{
			data->dt = 0.01f;
		}

		sf::Event event;
		while (data->render->pollEvent(event))
		{
			PollEvents(data, event);
		}

		Update(data, data->dt);

		data->render->clear(sf::Color::Blue);
		Draw(data);
		data->render->display();
	}

	return EXIT_SUCCESS;
}

void LoadWindow(sf::RenderWindow* _render)
{
	_render->setVerticalSyncEnabled(VSYNC);
	_render->setFramerateLimit(MAX_FPS);
	_render->setKeyRepeatEnabled(KEY_REPEAT);
}

#pragma endregion