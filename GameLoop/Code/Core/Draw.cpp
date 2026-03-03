#include "Draw.hpp"

#include "../../Tools/SceneManager.hpp"
#include "../../GameData.hpp"
#include "../../Tools/Debug/DebugViewer.hpp"

void Draw(GameData* _game)
{
	_game->renderTexture->clear(sf::Color::Blue);

	_game->manager->Draw(_game->render);


	_game->renderTexture->display();
	sf::Sprite sprite;
	sprite.setTexture(_game->renderTexture->getTexture());
	//_game->render->draw(sprite);

	_game->debugViewer->Draw(*_game->render);

	_game->guiManager->Render();


}
